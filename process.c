#include <avr/io.h>
#include <avr/interrupt.h>
#include "process.h"
#include "scheduler.h"
#include "memory.h"

#ifdef DEBUG
#include <stdio.h>
#endif

#define MASK_TMMODE 0x00
#define MASK_TMPS 0x05
#define MASK_TMI 0x02

ISR(TIMER0_COMPA_vect){
	//switch contesto
	_stop_timer_process();
	uint8_t next_pid;
	struct process *current=_get_current_process();
	struct process *next=_next_process(&next_pid);
	void (*sw)(struct context*, struct context*);
	current->stato=STOP;
	
	_set_current_pid(next_pid);
	
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
	uint8_t new_pid=0;
	struct process *proc=_add_process_to_scheduler(f, &new_pid);
	_lock_page(proc->page);
	if(proc==(void*)0x00) return -1;
	uint8_t *sp=proc->contesto.sp;
	
	uint16_t addr_f=f, addr_end=_end_process;						//scrittura miccia
	uint8_t bl=addr_f & 0x00ff;		//byte basso
	uint8_t bh=addr_f>>8;					//byte alto
	*(sp+1)=0x00;
	*(sp+2)=bh;
	*(sp+3)=bl;
	bl=addr_end & 0x00ff;
	bh=addr_end>>8;
	*(sp+4)=0x00;
	*(sp+5)=bh;
	*(sp+6)=bl;
	return 0;
}

int _delete_process(uint8_t pid){
/*
Funzione che elimina un processo e lo rimuove dallo scheduler.
Torna 0 se corretto, altrimenti -1.
*/
	if(_remove_process_from_scheduler(pid)==-1) return -1;
	return 0;
}

void _end_process(){
	cli();
	_stop_timer_process();
	_reset_timer_process();
	uint8_t cpid=_get_current_pid(), next_pid;
	struct process *next=_next_process(&next_pid);
	struct process *current=_get_current_process();
	_unlock_page(current->page);
	void (*sw)(struct context*);
	
	_delete_process(cpid);
	
	_set_current_pid(next_pid);
	if(next->stato==CREATED) sw=_end_first_switch;
	else sw=_end_switch;	
	next->stato=RUN;
	_start_timer_process();
	sw(&(next->contesto));
}

void _init_timer_process(){
/*Inizializzazione timer per time slot della CPU per i processi*/
	cli();
	TCCR0A &= MASK_TMMODE;
	TIMSK0 |= MASK_TMI;
	sei();
}

void _start_timer_process(){
	TCCR0B |= MASK_TMPS;
}

void _stop_timer_process(){
	TCCR0B &= 0x00;
}

void _reset_timer_process(){
	TCNT0=0x00;
}

