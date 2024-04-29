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
void mul();

int main(int argc, char *argv[]){
	_init_OS();
	printf_init();
	void* args[]={&somma};
	_syscall(CREATE_PROCESS, args);
	args[0]=&diff;
	_syscall(CREATE_PROCESS, args);
	args[0]=&mul;
	_syscall(CREATE_PROCESS, args);
	_start_OS();
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

void mul(){
	int a=2, b=5;
	while(1) printf("%d\n", a*b);
}


