#pragma once

#include "memory.h"
#include "process.h"
#include "simple_scheduler.h"

#define DIM_STACK_KERNEL ( DIM_PAGE*2 )

struct _kernel_structure{
	Process		ProcList[MAX_PROC];
	arrayListSched	scheduler;
};

int		_create_process(void* f);
int		_delete_process(pid_t p);
int		_sleep_process(pid_t p);
int		_wake_process(pid_t p);
int		_sleep(pid_t p);

void 	_init_timer_process();
void 	_start_timer_process();
void 	_stop_timer_process();
void 	_reset_timer_process();

int		_add_pid_to_scheduler(pid_t pid);
int		_remove_pid_from_scheduler(pid_t pid);
pid_t	_next_pid();
void	_set_current_pid(pid_t pid);
pid_t	_get_current_pid();

Process* _get_process(pid_t pid);

void _init_kernel();

