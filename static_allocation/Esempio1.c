#include <stdio.h>
#include "OS.h"

void somma();
void dormi();

void TX_1();
void TX_2();
void TX_3();
void RX_1();
void RX_2();

int main(int argc, char* argv[]){
	_init_OS();
	
	int resCreate;
	void* args[]={&TX_1};
	syscall(CREATE_PROCESS, args, &resCreate);
	if(resCreate==0)
		printf("Creato istanza di %04X\n", args[0]);
	
	args[0]=&TX_2;	
	syscall(CREATE_PROCESS, args, &resCreate);
	if(resCreate==0)
		printf("Creato istanza di %04X\n", args[0]);
		
	args[0]=&TX_3;	
	syscall(CREATE_PROCESS, args, &resCreate);
	if(resCreate==0)
		printf("Creato istanza di %04X\n", args[0]);
		
	args[0]=&dormi;	
	syscall(CREATE_PROCESS, args, &resCreate);
	if(resCreate==0)
		printf("Creato istanza di %04X\n", args[0]);
		
	args[0]=&RX_1;	
	syscall(CREATE_PROCESS, args, &resCreate);
	if(resCreate==0)
		printf("Creato istanza di %04X\n", args[0]);
		
	args[0]=&RX_2;	
	syscall(CREATE_PROCESS, args, &resCreate);
	if(resCreate==0)
		printf("Creato istanza di %04X\n", args[0]);
		
	args[0]=&somma;	
	syscall(CREATE_PROCESS, args, &resCreate);
	if(resCreate==0)
		printf("Creato istanza di %04X\n", args[0]);	
	
	_start_OS();
	return 0;
}



void dormi(){
	while(1);
}

void somma(){
	int a=2,b=5;
	printf("%d\n", a+b);
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
	char c;
	while(1){
		c=getChar();
		if(c=='L')
			led_on();
		else if(c=='O')
			led_off();
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

