#include <avr/io.h>
#include <avr/interrupt.h>
#include "process.h"
#include "kernel.h"
#include "memory.h"

#ifdef DEBUG
#include <stdio.h>
#endif

#define MASK_TMMODE 0x02
#define MASK_TMPS 0x05
#define TOP_OCR0A 0xFF
#define MASK_TMI 0x02

extern struct kernel_structure kernel;

void _context_switch(struct context *old, struct context *new);
void _first_switch(struct context *old, struct context *new);
void _end_first_switch(struct context *new);
void _end_switch(struct context *new);

ISR(TIMER0_COMPA_vect){
/*Interrupt per il context switch*/
	_stop_timer_process();
	struct process *current=kernel.get_current_process();
	struct process *next=kernel.next_process();
	void (*sw)(struct context*, struct context*);
	
	current->stato=STOP;
	kernel.set_current_pid(next->PID);
	
	if(next->stato==CREATED){
		sw=_first_switch;
	}
	else sw=_context_switch;
	
	next->stato=RUN;
	_start_timer_process();
	sw(&(current->contesto), &(next->contesto));
}

int _create_process(void* f){
/*
Funzione che crea un nuovo processo in memoria e lo inizializza per il context switch.
Torna 0 se è stato eseguito correttamente, altrimenti -1.
*/
	page_t process_page;
	pid_t new_pid=0;	
	_request_page(&process_page); //creo il processo solo se ho le pagine disponibili
	
	if(process_page > -1){
		int res=kernel.add_process_to_scheduler(&new_pid);
		if(res == -1) return -1;
		
		uint8_t *sp=START_RAM-(DIM_PAGE*(process_page)+6);
		
		struct process *new_process=&( kernel.procList[new_pid] );
		new_process->func_addr=f;
		new_process->stato=CREATED;
		new_process->page=process_page;
		new_process->contesto.sp=(uint16_t)sp;
		new_process->PID=new_pid;

		_lock_page(process_page);
		
		uint16_t addr_f=(uint16_t)f;
		uint16_t addr_end=(uint16_t)_end_process;
		uint8_t bl=addr_f & 0x00ff;													//scrittura indirizzo prima istruzione del processo
		uint8_t bh=addr_f>>8;
		*(sp+1)=0x00;
		*(sp+2)=bh;
		*(sp+3)=bl;
		bl=addr_end & 0x00ff;				//scrittura ritorno per la procedura di terminazione
		bh=addr_end>>8;
		*(sp+4)=0x00;
		*(sp+5)=bh;
		*(sp+6)=bl;
		return 0;
		
	}else return -1;
}

int _delete_process(pid_t pid){
/*
Funzione che elimina un processo e lo rimuove dallo scheduler.
Torna 0 se corretto, altrimenti -1.
*/
	struct process* p=kernel.get_process(pid);
	_unlock_page(p->page);
	if(kernel.remove_process_from_scheduler(pid)==-1) return -1;
	
	p->func_addr=(void*)0x00;
	p->stato=FINISH;
	p->page=0;
	p->contesto.sp=0;
	p->PID=-1;
	
	return 0;
}

void _end_process(){
/*Procedura per la terminazione di un processo*/
	cli();
	_stop_timer_process();
	_reset_timer_process();
	pid_t cpid=kernel.get_current_pid();
	struct process *next=kernel.next_process();
	void (*sw)(struct context*);
	
	_delete_process(cpid);
	
	kernel.set_current_pid(next->PID);
	if(next->stato==CREATED){
		sw=_end_first_switch;
	}
	else sw=_end_switch;	
	next->stato=RUN;
	
	if(kernel.get_active_process() > 1)
		_start_timer_process();
		
	sw(&(next->contesto));
}

int _sleep_process(pid_t pid){
	struct process* p= kernel.get_process(pid);
	if(p != (struct process*)0x00){
		p->stato=SLEEP;
		kernel.sleepProcess[pid]=p;
		kernel.remove_process_from_scheduler(pid);
		return 0;
	}else return -1;
}

void _init_timer_process(){
/*Inizializzazione timer per time slot della CPU per i processi*/
	cli();
	TCCR0A |= MASK_TMMODE;
	OCR0A = TOP_OCR0A;
	TIMSK0 |= MASK_TMI;
	sei();
}

void _start_timer_process(){
	TCCR0B |= MASK_TMPS;
}

void _stop_timer_process(){
	TCCR0B &= ~MASK_TMPS;
}

void _reset_timer_process(){
	TCNT0=0x00;
}

