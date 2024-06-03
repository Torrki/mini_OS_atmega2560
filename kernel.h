#pragma once

#define KERNEL_H
#define MAX_PROC 12

#include "process.h"

struct kernel_structure{
	void* last_sp;
	struct process procList[MAX_PROC];
	
	struct process* (*add_process_to_scheduler)(void* f, pid_t *pid);
	int (*remove_process_from_scheduler)(pid_t pid);
	struct process* (*get_current_process)();
	struct process* (*get_process)(pid_t pid);
	struct process* (*next_process)(pid_t *pid);
	pid_t (*get_current_pid)();
	void (*set_current_pid)(pid_t pid);
};

void _init_kernel();

