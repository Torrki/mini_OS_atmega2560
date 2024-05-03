#pragma once

#define PROCESS_H
#define MAX_PROC 15
#include <stdint.h>

enum _stato_processo{
	RUN=0,
	STOP,
	FINISH,
	CREATED
};

struct context{
	uint16_t sp;
};

struct process{
	void* func_addr;
	uint8_t stato;
	struct context contesto;
};

void _create_process(void* f);																					//syscall creazione processo
void _get_return(uint32_t *addr);																				//funzione asm per ottenere l'indirizzo di ritorno
void _delete_process(uint8_t pid);																			//syscall eliminazione processo
void _end_process();																										//funzione per terminazione dei processi
void _init_timer_process();																							//funzione per inizializzare il timer per ctx switch
void _start_timer_process();																						//funzione start timer ctx switch
void _reset_timer_process();																						//funzione per reset del timer
void _stop_timer_process();																							//funzione stop timer ctx switch

