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
	pid_t next_pid;
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
	pid_t new_pid=0;	
	struct process *proc=_add_process_to_scheduler(f, &new_pid);
	if(proc==(void*)0x00) return -1;
	
	_lock_page(proc->page);
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

int _delete_process(pid_t pid){
/*
Funzione che elimina un processo e lo rimuove dallo scheduler.
Torna 0 se corretto, altrimenti -1.
*/
	struct process *p=_get_process(pid);
	_unlock_page(p->page);
	if(_remove_process_from_scheduler(pid)==-1) return -1;
	return 0;
}

void _end_process(){
	cli();
	_stop_timer_process();
	_reset_timer_process();
	pid_t cpid=_get_current_pid(), next_pid;
	struct process *next=_next_process(&next_pid);
	void (*sw)(struct context*);
	
	_delete_process(cpid);
	
	_set_current_pid(next_pid);
	if(next->stato==CREATED) sw=_end_first_switch;
	else sw=_end_switch;	
	next->stato=RUN;
	_start_timer_process();
	sw(&(next->contesto));
}

void* _malloc(uint32_t size){
	struct process *current=_get_current_process();
	if(current->heap_alloc==0x00ffffff) return (void*)0x00; //out of memory
	else{
		uint8_t temp=current->heap_alloc, i=0;
		while(temp > 0) {
			temp >>= 1;
			i++;
		}
		for(int k=0; k <= size/((uint32_t)MIN_HEAP_UNIT); k++) current->heap_alloc |= ((uint32_t)1)<<(i+k);
		printf("heap: %08X\n", current->heap_alloc);
		return START_RAM-(DIM_PAGE*(current->page)+DIM_STACK_PROC+MIN_HEAP_UNIT*(N_UNIT_HEAP-i))+1;
	}
}

void _free(void* addr){
	struct process *current=_get_current_process();
	uint16_t addr_norm= START_RAM-(DIM_PAGE*(current->page)+DIM_STACK_PROC)-(uint16_t)addr;
	uint8_t k=addr_norm/MIN_HEAP_UNIT;
	
	current->heap_alloc &= ~( ((uint32_t)1)<<(N_UNIT_HEAP-k-1) );
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

