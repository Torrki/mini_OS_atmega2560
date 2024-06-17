#include "syscall.h"
#include "core.h"
#include "avr_common/uart.h"
#include <stdio.h>

void somma();
void diff();
void mul();
void pow();

int main(int argc, char *argv[]){
	_init_OS();
	printf_init();
	
	void* args[]={&somma};
	
	_syscall(CREATE_PROCESS, args, (void*)0x00);
	args[0]=&diff;
	_syscall(CREATE_PROCESS, args, (void*)0x00);
	args[0]=&mul;
	_syscall(CREATE_PROCESS, args, (void*)0x00);
	args[0]=&pow;
	_syscall(CREATE_PROCESS, args, (void*)0x00);
	
	_start_OS();
	return 0;
}

void pow(){
	void* m1=my_malloc(20);
	void* m2=my_malloc(27);
	my_free(m1);
	my_free(m2);
	while(1) printf("%d\n", 32);
}

void somma(){
	int a=2, b=5;
	printf("malloc: %02X\n", my_malloc(10));
	while(1) printf("%d\n", 2+5);
}

void diff(){
	printf("malloc: %02X\n", my_malloc(30));
}

void mul(){
	while(1) printf("%d\n", 2*5);
}

