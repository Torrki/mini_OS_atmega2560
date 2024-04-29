#include <avr/io.h>
#include <avr/interrupt.h>
#include "scheduler.h"
#include "process.h"
#include "core.h"

#ifdef DEBUG
#include <stdio.h>
#endif

#define MASK_TMMODE 0x00
#define MASK_TMPS 0x05
#define MASK_TMI 0x02

ISR(TIMER0_COMPA_vect){
	cli();			//context switch safe
	uint8_t pid;
	struct process *current=_get_current_process();
	struct process *next=_next_process(&pid);
	_set_current_pid(pid);
	current->stato=STOP;
	if(next->stato==CREATED){
		next->stato=RUN;
		_first_switch(&(current->contesto), &(next->contesto));
	}
	else {
		next->stato=RUN;
		_context_switch(&(current->contesto), &(next->contesto));
	}
}

void _create_process(void* f){
	uint8_t pid=0;
	struct process *proc=_add_process_to_scheduler(f,&pid);
	if(proc==(void*)0x00) printf("Abort: no free slot\n");
	else{
		uint16_t offset=SP-(START_RAM-DIM_STACK_KERNEL);
		SP -= offset+DIM_PROC*pid; 	//mi sposto nello stack del processo
		uint8_t *sp=SP;												//scrivo la miccia
		uint8_t bl= ((uint16_t)f)&0x00ff;			//byte basso
		uint8_t bh= ((uint16_t)f)&0xff00;			//byte alto
		*sp=bl;
		*(sp-1)=bh;
		*(sp-2)=0x00;
		proc->contesto.sp=sp-3;
		SP += offset+DIM_PROC*pid; 	//torno nello stack del kernel
	}	
}

void _delete_process(uint8_t pid){
	_remove_process_from_scheduler(pid);
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

