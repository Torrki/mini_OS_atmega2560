#include "core.h"
#include "syscall.h"
#include "scheduler.h"

void _init_OS(){
	_init_timer_process();
	_init_syscall_vect();
	_init_scheduler();
}

