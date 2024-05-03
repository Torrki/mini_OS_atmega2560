#include <avr/io.h>
#include <avr/interrupt.h>
#include "process.h"
#include "scheduler.h"
#include "memory.h"
#include "core.h"

#ifdef DEBUG
#include <stdio.h>
#endif

#define MASK_TMMODE 0x00
#define MASK_TMPS 0x05
#define MASK_TMI 0x02

ISR(TIMER0_COMPA_vect){
	_stop_timer_process();
	uint8_t pid;
	struct process *current=_get_current_process();
	struct process *next=_next_process(&pid);
	current->stato=STOP;
	void (*ctx)(struct context*, struct context*);
	
	if(next->stato==CREATED) ctx=_first_switch;
	else ctx=_context_switch;
	
	next->stato=RUN;
	_set_current_pid(pid);
	_start_timer_process();
	ctx(&(current->contesto), &(next->contesto));
}

void _create_process(void* f){
	uint8_t pid=0;
	struct process *proc=_add_process_to_scheduler(f,&pid);
	if(proc==(void*)0x00) printf("Abort: no free slot\n");
	else{
		uint16_t offset=SP-(START_RAM-DIM_PAGE), addr_f=f, addr_end=_end_process;
		_lock_page(pid+1);
		
		SP -= offset+DIM_PAGE*pid; 				//mi sposto nello stack del processo
		uint8_t *sp=SP;										//scrivo la miccia
		uint8_t bl= addr_end&0x00ff;			//byte basso
		uint8_t bh= addr_end>>8;					//byte alto
		*sp=bl;
		*(sp-1)=bh;
		*(sp-2)=0x00;
		bl=addr_f&0x00ff;									//byte basso
		bh=addr_f>>8;											//byte alto
		*(sp-3)=bl;
		*(sp-4)=bh;
		*(sp-5)=0x00;
		proc->contesto.sp=sp-6;
		SP += offset+DIM_PAGE*pid; 				//torno nello stack del kernel
	}	
}

void _delete_process(uint8_t pid){
	_remove_process_from_scheduler(pid);
	_unlock_page(pid+1);
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

void _end_process(){
	cli();
	_stop_timer_process();
	_reset_timer_process();
	uint8_t cpid=_get_current_pid(), npid;
	struct process* next=_next_process(&npid);
	
	_delete_process(cpid);
	
	void (*ctx)(struct context*);
	if(next->stato==CREATED) ctx=_end_first_switch;
	else ctx=_end_switch;
	
	next->stato=RUN;
	_set_current_pid(npid);
	if(_get_active_process() > 1)	_start_timer_process();
	sei();
	ctx(&(next->contesto));
}

void _reset_timer_process(){
	TCNT0=0x00;
}

