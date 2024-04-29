#pragma once
#define PROCESS_H
#include <stdint.h>

enum _stato_processo{
	RUN=0,
	STOP,
	FINISH,
};

struct context{
	uint16_t sp;
	uint8_t sreg;
};

struct process{
	void* func_addr;
	uint8_t stato;
	struct context contesto;
};

void _create_process(void* f);																				//syscall creazione processo
void _prepare_process(struct process *p);																//funzione asm per la preparazione del processo al ctx switch
void _context_switch(struct context *old, struct context *new);					//funzione asm per il context switch
void _delete_process(uint8_t pid);																			//syscall eliminazione processo
void _init_timer_process();																							//funzione per inizializzare il timer per ctx switch
void _start_timer_process();																						//funzione start timer ctx switch
void _stop_timer_process();																							//funzione stop timer ctx switch

