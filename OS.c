#include "OS.h"
#include "kernel.h"
#include "avr_common/uart.h"
#include <avr/io.h>

#ifdef DEBUG
#include <stdio.h>
#endif

void somma();
void dormi();
void diff();

extern struct _kernel_structure kernel;

void _init_OS(){
	printf_init();
	_init_memory();
	_init_kernel();
	_init_timer_process();
	
	_create_process(&dormi);
	_create_process(&diff);
	_create_process(&somma);
	_create_process(&somma);
	
	printScheduler();
	
}

void _start_OS(){
	Process proc0=kernel.ProcList[0];
	if(proc0.PID != -1){
		SP=proc0.contesto.sp;
		_start_timer_process();
		asm("ret");
	}
}

void dormi(){
	printf("Processo che dorme\n");
	_sleep(_get_current_pid());
	while(1) printScheduler();
}

void somma(){
	int a=2,b=5;
	printf("PID: %hd\n", _get_current_pid());
	while(1) printf("%d\n", a+b);
}

void diff(){
	int a=2,b=5;
	printf("PID: %hd\n", _get_current_pid());
	_wake_process(0);
	while(1) printf("%d\n", a-b);
}

