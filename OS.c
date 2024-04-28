#include "syscall.h"
#ifdef DEBUG
#include "scheduler.h"
#include <avr/io.h>
#include "avr_common/uart.h"
#include <stdio.h>
#endif

void somma();
void diff();

int main(int argc, char *argv[]){
	_init_OS();
	printf_init();
	_syscall(CREATE_PROCESS, &somma);
	_syscall(CREATE_PROCESS, &diff);
	uint8_t next_pid;
	struct process *proc=_get_current_process();
	struct process *next=_next_process(&next_pid);
	_context_switch( &(proc->contesto), &(next->contesto) );
	return 0;
}

void somma(){
	int a=2, b=5;
	while(1) printf("%d\n", a+b);
}

void diff(){
	int a=2, b=5;
	while(1) printf("%d\n", a-b);
}

