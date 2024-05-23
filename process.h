#pragma once

#define PROCESS_H

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
	uint8_t page;
	struct context contesto;
	uint32_t heap_alloc;
};

typedef int8_t pid_t;

void _context_switch(struct context *old, struct context *new);
void _first_switch(struct context *old, struct context *new);
void _end_first_switch(struct context *new);
void _end_switch(struct context *new);
void _init_timer_process();
void _start_timer_process();
void _stop_timer_process();
void _reset_timer_process();

int _create_process(void* f);
int _delete_process(pid_t pid);
void _end_process();
void* _malloc(uint32_t size);
void _free(void* addr);

