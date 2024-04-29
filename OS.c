#include "syscall.h"
#include "core.h"
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
	void* args[]={&somma};
	_syscall(CREATE_PROCESS, args);
	args[0]=&diff;
	_syscall(CREATE_PROCESS, args);
	struct process *current=_get_current_process();
	uint8_t pid;
	struct process *next=_next_process(&pid);
	printf("%hhu\n", pid);
	_context_switch(&(current->contesto), &(next->contesto));
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

