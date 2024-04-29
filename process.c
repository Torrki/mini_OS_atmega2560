#include <avr/io.h>
#include <avr/interrupt.h>
#include "scheduler.h"
#include "process.h"
#include "core.h"

#ifdef DEBUG
#include <stdio.h>
#endif

#define MASK_TMMODE 0x00
#define MASK_TMPS 0x01
#define MASK_TMI 0x02

ISR(TIMER0_COMPA_vect){
}

void _create_process(void* f){
	uint8_t pid=0;
	struct process *proc=_add_process_to_scheduler(f,&pid);
	if(proc==(void*)0x00) printf("Abort: no free slot\n");
	else{
		uint16_t offset=SP-(START_RAM-DIM_STACK_KERNEL);
		SP -= offset+DIM_PROC*pid; 	//mi sposto nello stack del processo
		_prepare_process(proc);
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

