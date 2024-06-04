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
	int a=2, b=5, potenza=1;
	for(int i=0; i<b; i++) potenza *= a;
	
	int8_t pid=0;
	void* args[]={&pid};
	_syscall(WAKE_PROCESS, args, (void*)0x00);
	while(1) printf("%d\n", potenza);
}

void somma(){
	int a=2, b=5;
	while(1) printf("%d\n", a+b);
}

void diff(){
	int a=2, b=5;
	printf("%d\n", a-b);
	
	int8_t pid=0;
	void* args[]={&pid};
	_syscall(SLEEP_PROCESS, args, (void*)0x00);
}

void mul(){
	int a=2, b=5;
	while(1) printf("%d\n", a*b);
}

