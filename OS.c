#include "core.h"
#ifdef DEBUG
#include <avr/io.h>
#include "avr_common/uart.h"
#include <stdio.h>
#endif

void somma(void);
void diff(void);

extern struct _OS_context OS_state;

int main(int argc, char *argv[]){
	_init_OS();
	printf_init();
	_syscall(CREATE_PROCESS, &somma);
	_syscall(CREATE_PROCESS, &diff);
	_context_switch( &(OS_state.procList[1].contesto), &(OS_state.procList[0].contesto) );
	
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

