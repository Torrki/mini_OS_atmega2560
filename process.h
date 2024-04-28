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
	void *func_addr;
	uint8_t stato;
	struct context contesto;
};

void _create_process(void* f);
void _prepare_process(struct process *p); //funzione miccia
void _delete_process(uint8_t pid);
void _init_timer_process(void);
void _start_timer_process(void);
void _stop_timer_process(void);
#ifdef DEBUG
void printProcess(uint8_t pid);
#endif

