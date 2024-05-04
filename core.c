#include "core.h"
#include "memory.h"
#include "syscall.h"
#include "scheduler.h"
#include <avr/io.h>

void _init_OS(){
	_init_syscall();
	_init_scheduler();
	_init_timer_process();
	_init_memory();
}

void _start_OS(){
	struct process* proc0=_get_current_process();
	SP=proc0->contesto.sp;
	_start_timer_process();
}

