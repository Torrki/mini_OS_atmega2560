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
	asm("push r24\npush r24\npush r29\npush r28\npush r24"); //allocazione spazio per le variabili
	uint8_t *id=SP+1;							//id syscall
	void* *args_sp=SP+2;					//riprendo gli argomenti
	void* *args= (*args_sp)+1;
	void* *addr=SP+4;							//prendo l'indirizzo della syscall
	*addr=syscall_vect[*id];
	if((*id & ~CREATE_PROCESS)==0){
		void* start=*args;
		void (*sys)(void*)=*addr;
		sys(start);
		printf("Creato\n");
	}
	else if((*id & ~DELETE_PROCESS)==0){
		uint8_t *pid=*args;
		void (*sys)(uint8_t)=*addr;
		sys(*pid);
	}
	else printf("Abort: no syscall\n");
	asm("pop r24\npop r28\npop r29\npop r24\npop r24");
}

void _init_syscall_vect(){
/*popolazione vettore delle syscall*/
	syscall_vect[CREATE_PROCESS]=_create_process;
	syscall_vect[DELETE_PROCESS]=_delete_process;
	
/*abilitazione trap delle syscall*/
	cli();
	EICRB |= 0x0C;
	EIMSK |= TRAP_MASK;
	DDRE |= TRAP_MASK;
	sei();
}

void _syscall(uint8_t id, void* args){
	//r24	: id
	//Y+1	: indirizzo args
	PORTE |= TRAP_MASK; 			//attivazione trap
	PORTE &= ~TRAP_MASK; 			//disattivazione per le chiamate successive
}

