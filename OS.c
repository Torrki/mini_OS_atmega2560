#include "syscall.h"
#include "core.h"
#include "avr_common/uart.h"
#include <stdio.h>
#ifdef DEBUG
#include "scheduler.h"
#endif

void somma();
void diff();
void mul();
char* stampa(char* str);

int main(int argc, char *argv[]){
	_init_OS();
	printf_init();
	
	void* args[]={&somma};
	uint8_t i[]={1};
	
	_syscall(CREATE_PROCESS, args);
	args[0]=&diff;
	_syscall(CREATE_PROCESS, args);
	args[0]=&mul;
	_syscall(CREATE_PROCESS, args);
	args[0]=&somma;
	_syscall(CREATE_PROCESS, args);
	
	_start_OS();
	return 0;
}

void somma(){
	int a=2, b=5;
	char* addr_1=(char*)malloc(40);
	printf("%X\n", addr_1);
	addr_1="Ciao malloc";
	printf("%s\n", stampa(addr_1));
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

char* stampa(char* str){
	printf("%s\n", str);
	char *risposta=(char*)malloc(20);
	risposta="Salve main";
	return risposta;
}

