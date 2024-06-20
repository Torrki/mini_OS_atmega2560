#include "OS.h"
#include "kernel.h"
#include "avr_common/uart.h"
#include <avr/io.h>

#define LED_ON 	PORTB |= 0x80
#define LED_OFF	PORTB &= ~0x80

#ifdef DEBUG
#include <stdio.h>
#endif

void somma();
void dormi();
void diff();

void TX_1();
void TX_2();
void TX_3();
void RX_1();
void RX_2();

extern struct _kernel_structure kernel;

void _init_OS(){
	DDRB |= 0x80;
	printf_init();
	_init_memory();
	_init_kernel();
	_init_timer_process();
	
	_create_process(&TX_1);
	_create_process(&dormi);
	_create_process(&TX_3);
	_create_process(&RX_1);
	_create_process(&RX_2);
	_create_process(&somma);
	
	//printScheduler();
	
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
	while(1);
}

void somma(){
	int a=2,b=5;
	printf("%d\n", a+b);
}

void diff(){
	int a=2,b=5;
	printf("PID: %hd\n", _get_current_pid());
	_wake_process(0);
	while(1) printf("%d\n", a-b);
}

void TX_1(){
	char c='A';
	while(1) putChar(c);
}

void TX_2(){
	char c='L';
	while(1) putChar(c);
}

void TX_3(){
	char c='C';
	while(1) putChar(c);
}

void RX_1(){
	char c='v';
	while(1){
		c=getChar();
		if(c=='L')
			LED_ON;
		else if(c=='O')
			LED_OFF;
	}
}

void RX_2(){
	char c;
	while(1){
		c=getChar();
		if(c=='L')
			printf("Peffo\n");
		else if(c=='O')
			printf("A lettoh\n");
	}
}


