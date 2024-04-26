#include <avr/interrupt.h>
#include <avr/io.h>
#include "syscall.h"
#include "process.h"
#ifdef DEBUG
#include <stdio.h>
#endif

#define TRAP_MASK 0x20

void* syscall_vect[SYSCALL_VECT];

ISR(INT5_vect){
	asm("push r24\npush r24\npush r3\npush r2\npush r4"); //allocazione spazio per le variabili
	uint8_t *id=SP+1;
	void **args=SP+2;
	void **addr=SP+4;
	*addr=syscall_vect[*id];
	if((*id & ~CREATE_PROCESS)==0){
		void* start=*args;
		void (*sys)(void*)=*addr;
		sys(start);
	}
	else if((*id & ~DELETE_PROCESS)==0){
		uint8_t *pid=*args;
		void (*sys)(uint8_t)=*addr;
		sys(*pid);
	}
	else printf("Abort: no syscall\n");
	asm("pop r4\npop r2\npop r3\npop r24\npop r24");
}

void _init_syscall_vect(void){
	//popolare il vettore delle syscall
	syscall_vect[CREATE_PROCESS]=_create_process;
	syscall_vect[DELETE_PROCESS]=_delete_process;
	
	//abilitazione trap syscall
	cli();
	EICRB |= 0x0C;
	EIMSK |= TRAP_MASK;
	DDRE |= TRAP_MASK;
	sei();
}

void _syscall(uint8_t id, void* args){
	//r24: id
	//r23-r22: indirizzo array argomenti
	asm("mov r2,r22\nmov r3,r23\nmov r4,r24\n");
	PORTE |= TRAP_MASK; //attivazione trap
	PORTE &= ~TRAP_MASK; //disattivazione per le chiamate successive
}

