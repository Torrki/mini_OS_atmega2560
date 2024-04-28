#include <avr/io.h>
#include <avr/interrupt.h>
#include "process.h"
#include "core.h"

#ifdef DEBUG
#include <stdio.h>
#endif

#define MASK_TMMODE 0x00
#define MASK_TMPS 0x01
#define MASK_TMI 0x02

extern struct _OS_context OS_state;

ISR(TIMER0_COMPA_vect){
}

void _create_process(void* f){
	uint8_t i=0;
	while(i<MAX_PROC && OS_state.procList[i].func_addr!=(void*)0x00) i++;
	if(i==MAX_PROC) printf("Abort: no free slot\n");
	else{
		OS_state.procList[i].func_addr=f;
		OS_state.procList[i].stato=RUN;
		OS_state.procList[i].contesto.sp=0;
		OS_state.procList[i].contesto.sreg=0;
		OS_state.num_active_process++;
		printf("%X\n", f);
		
		uint16_t offset=SP-(START_RAM-DIM_STACK_KERNEL);
		SP -= offset+DIM_PROC*i; //mi sposto nello stack del processo
		_prepare_process( &(OS_state.procList[i]) );
		SP += offset+DIM_PROC*i; //torno nello stack del kernel
	}	
}

void _delete_process(uint8_t pid){
	if(OS_state.procList[pid].func_addr==(void*)0x00) printf("Abort: no process in %hhu\n", pid);
	else{
		OS_state.procList[pid].func_addr=(void*)0x00;
		OS_state.procList[pid].stato=FINISH;
		OS_state.procList[pid].contesto.sp=0;
		OS_state.procList[pid].contesto.sreg=0;
	}
}

void _init_timer_process(void){
/*Inizializzazione timer per time slot della CPU per i processi*/
	cli();
	TCCR0A &= MASK_TMMODE;
	TIMSK0 |= MASK_TMI;
	sei();
}

void _start_timer_process(void){
	TCCR0B |= MASK_TMPS;
}

void _stop_timer_process(void){
	TCCR0B &= 0x00;
}

#ifdef DEBUG
void printProcess(uint8_t pid){
	if(OS_state.procList[pid].func_addr == (void*)0x00) printf("Abort: no process in %hhu\n", pid);
	else printf("func: %p\nstato: %hhu\n", OS_state.procList[pid].func_addr, OS_state.procList[pid].stato);
}
#endif

