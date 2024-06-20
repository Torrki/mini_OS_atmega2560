#include "OS.h"
#include "kernel.h"
#include "avr_common/uart.h"
#include <avr/io.h>

#ifdef DEBUG
#include <stdio.h>
#endif

extern struct _kernel_structure kernel;

void _init_OS(){
	DDRB |= 0x80;
	printf_init();
	_init_kernel();	
}

void _start_OS(){
	Process proc0=kernel.ProcList[0];
	if(proc0.PID != -1){
		kernel.last_SP=SP;
		SP=proc0.contesto.sp;
		_start_timer_process();
		asm("ret");
	}
}

