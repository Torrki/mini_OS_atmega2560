#include "syscall.h"
#include "core.h"
#include "avr_common/uart.h"
#include <stdio.h>

void somma();
void diff();
void mul();

int main(int argc, char *argv[]){
	_init_OS();
	printf_init();
	
	void* args[]={&somma};
	
	_syscall(CREATE_PROCESS, args, (void*)0x00);
	args[0]=&diff;
	_syscall(CREATE_PROCESS, args, (void*)0x00);
	args[0]=&mul;
	_syscall(CREATE_PROCESS, args, (void*)0x00);
	args[0]=&somma;
	_syscall(CREATE_PROCESS, args, (void*)0x00);
	
	_start_OS();
	return 0;
}

void somma(){
	int a=2, b=5;
	while(1) printf("%d\n", a+b);
}

void diff(){
	int a=2, b=5;
	printf("%d\n", a-b);
}

void mul(){
	int a=2, b=5;
	while(1) printf("%d\n", a*b);
}

