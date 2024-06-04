#include "core.h"
#include "memory.h"
#include "syscall.h"
#include "kernel.h"
#include "process.h"
#include <avr/io.h>
#include <stdio.h>

extern struct kernel_structure kernel;

void _init_OS(){
	_init_kernel();
	_init_syscall();
	_init_memory();
	_init_timer_process();
}

void _start_OS(){	
	struct process* proc0=kernel.get_process(0);
	
	if(proc0 != (struct process*)0x00){
		proc0->stato=RUN;
		kernel.last_sp=SP;
				
		SP=proc0->contesto.sp;
		_start_timer_process();
		asm("ret");
	}
}

