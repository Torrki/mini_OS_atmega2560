#pragma once
#include "process.h"
#include "syscall.h"
#define CORE_H

#define MAX_PROC 10

struct _OS_context{
	uint8_t current_pid;
	struct process procList[MAX_PROC];
};

void _init_OS(void);
