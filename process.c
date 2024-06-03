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

ISR(TIMER0_COMPA_vect){
/*Interrupt per il context switch*/
	_stop_timer_process();
	pid_t next_pid;
	struct process *current=get_current_process();
	struct process *next=next_process(&next_pid);
	void (*sw)(struct context*, struct context*);
	current->stato=STOP;
	
	kernel.set_current_pid(next_pid);
	
	if(next->stato==CREATED) sw=_first_switch;
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
		kernel.procList[new_pid].func_addr=f;
		kernel.procList[new_pid].stato=CREATED;
		kernel.procList[new_pid].page=process_page;
		kernel.procList[new_pid].contesto.sp=sp;
		
		_lock_page(process_page);
		
		uint16_t addr_f=f, addr_end=_end_process;						//scrittura miccia
		uint8_t bl=addr_f & 0x00ff;													//byte basso
		uint8_t bh=addr_f>>8;																//byte alto
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
	struct process *p=get_process(pid);
	_unlock_page(p->page);
	if(kernel.remove_process_from_scheduler(pid)==-1) return -1;
	
	kernel.procList[pid].func_addr=(void*)0x00;
	kernel.procList[pid].stato=FINISH;
	kernel.procList[pid].page=0;
	kernel.procList[pid].contesto.sp=0;
	return 0;
}

void _end_process(){
/*Procedura per la terminazione di un processo*/
	cli();
	_stop_timer_process();
	_reset_timer_process();
	pid_t cpid=kernel.get_current_pid(), next_pid;
	struct process *next=next_process(&next_pid);
	void (*sw)(struct context*);
	
	_delete_process(cpid);
	
	kernel.set_current_pid(next_pid);
	if(next->stato==CREATED) sw=_end_first_switch;
	else sw=_end_switch;	
	next->stato=RUN;
	
	if(kernel.get_active_process() > 1)
		_start_timer_process();
		
	sw(&(next->contesto));
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

