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
	while(OS_state.procList[i].func != (void*)0x00) i++;
	OS_state.procList[i].func=f;
	OS_state.procList[i].stato=STOP;
	OS_state.procList[i].contesto.pc=f;
}

void _delete_process(uint8_t pid){
	if(OS_state.procList[pid].func == (void*)0x00) printf("Abort: no process in %hhu\n", pid);
	else{
		OS_state.procList[pid].func=(void*)0x00;
		OS_state.procList[pid].stato=FINISH;
		OS_state.procList[pid].contesto.pc=(void*)0x00;
	}
}

void _start_process(uint8_t pid){
	OS_state.procList[pid].stato=RUN;
	void (*f)()=OS_state.procList[pid].contesto.pc;
	//_start_timer_process();
	f();
	//_stop_timer_process();
	OS_state.procList[pid].stato=FINISH;
}


void _init_timer_process(void){
	//funzione che inizializza il timer per l'uso della CPU per i processi, in avr parte quando s'imposta il prescale
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
	if(OS_state.procList[pid].func == (void*)0x00) printf("Abort: no process in %hhu\n", pid);
	else printf("func: %p\n", OS_state.procList[pid].func);
}
#endif

