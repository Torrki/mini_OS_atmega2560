#pragma once
#define PROCESS_H
#include <stdint.h>

enum _stato_processo{
	RUN=0,
	STOP,
	FINISH,
};

struct context{
	void* pc;
	uint8_t regs[32];
	uint8_t sreg;
	uint8_t spl;
	uint8_t sph;
};

struct process{
	void (*func)(void);
	uint8_t stato;
	struct context contesto;
};

void _create_process(void* f);
void _start_process(uint8_t pid);
void _delete_process(uint8_t pid);
void _context_switch(uint8_t oldpid, uint8_t newpid);
void _init_timer_process(void);
void _start_timer_process(void);
void _stop_timer_process(void);
#ifdef DEBUG
void printProcess(uint8_t pid);
#endif

