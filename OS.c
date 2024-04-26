#include "core.h"
#ifdef DEBUG
#include "avr_common/uart.h"
#include <stdio.h>
#endif

void somma(void);
void diff(void);

int main(int argc, char *argv[]){
	_init_OS();
	_syscall(CREATE_PROCESS, &somma);
	_syscall(CREATE_PROCESS, &diff);
	_start_process(0);
	_start_process(1);
	return 0;
}

void somma(){
	int a=2, b=5;
	printf("%d\n", a+b);
}

void diff(){
	int a=2, b=5;
	printf("%d\n", a-b);
}

