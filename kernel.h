#pragma once

#include "memory.h"
#include "process.h"
#include "simple_scheduler.h"
#include "syscall.h"

#define DIM_STACK_KERNEL ( DIM_PAGE*2 )

struct _kernel_structure{
	uint16_t	last_SP;
	Process		ProcList[MAX_PROC];
	arrayListSched	scheduler;
	pid_t arrayListTX[MAX_PROC];
	pid_t arrayListRX[MAX_PROC];
	pid_t startTX, startRX;
};

int		_create_process(void* f);
int		_delete_process(pid_t p);
void	_end_process();
int		_sleep_process(pid_t pid);	//suppone che un altro processo metta in SLEEP pid
int		_wake_process(pid_t pid);

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
void _init_syscall();

