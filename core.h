#pragma once
#include "process.h"
#include "syscall.h"
#define CORE_H

#define MAX_PROC 10
#define DIM_PROC 150
#define DIM_STACK_KERNEL 250
#define START_RAM 0x2200

struct _OS_context{
	uint8_t num_active_process;
	struct process procList[MAX_PROC];
};

void _init_OS(void);
void _context_switch(struct context *old, struct context *new);

