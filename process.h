#pragma once

#include "OS_types.h"

enum stato_processo{
	RUN=0,
	STOP,
	CREATED,
	SLEEP
};

typedef struct _process_context{
	uint16_t sp;
} PContext;

typedef struct _process_structure{
	uint16_t		func_addr;
	pid_t				PID;
	page_t			page;
	uint8_t			stato;
	PContext		contesto;
} Process;

#ifdef DEBUG
void printProcess(Process p);
#endif
