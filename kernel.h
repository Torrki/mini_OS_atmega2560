#pragma once

#define KERNEL_H

#include "process.h"

struct processFIFO{
	struct process* list[MAX_PROC];
	uint8_t head;
	uint8_t tail;
	uint8_t size;
};

struct kernel_structure{
	uint16_t 						last_sp;
	struct process 			procList[MAX_PROC];
	struct process*		 	sleepProcess[MAX_PROC];
	
	int 			(*add_process_to_scheduler)(pid_t *pid);
	int 			(*remove_process_from_scheduler)(pid_t pid);
	void 			(*next_pid)(pid_t *pid);
	pid_t 		(*get_current_pid)();
	void 			(*set_current_pid)(pid_t pid);
	uint8_t 	(*get_active_process)();
	
	struct process* (*get_current_process)();
	struct process* (*next_process)();
	struct process* (*get_process)(pid_t pid);
};

void _init_kernel();

