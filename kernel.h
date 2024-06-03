#pragma once

#define KERNEL_H

#include "process.h"

struct kernel_structure{
	void* last_sp;
	struct process procList[MAX_PROC];
	struct process *activeProcess[MAX_PROC];
	struct process *sleepProcess[MAX_PROC];
	
	int (*add_process_to_scheduler)(pid_t *pid);
	int (*remove_process_from_scheduler)(pid_t pid);
	void (*next_pid)(pid_t *pid);
	pid_t (*get_current_pid)();
	void (*set_current_pid)(pid_t pid);
	uint8_t (*get_active_process)();
};

void _init_kernel();
struct process* get_current_process();
struct process* next_process();
struct process* get_process(pid_t pid);

