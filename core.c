#include "core.h"
#include "syscall.h"
#include "scheduler.h"
#include <avr/io.h>

void _init_OS(){
	_init_timer_process();
	_init_syscall_vect();
	_init_scheduler();
}

void _start_OS(){
	struct process* proc0=_get_current_process();
	proc0->stato=RUN;
	SP=proc0->contesto.sp;
	if(_get_active_process() > 1) _start_timer_process();
}

