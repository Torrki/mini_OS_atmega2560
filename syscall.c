#include <avr/interrupt.h>
#include <avr/io.h>
#include "syscall.h"
#include "process.h"
#include "memory.h"
#include "kernel.h"
#ifdef DEBUG
#include <stdio.h>
#endif

#define TRAP_MASK 0x20
uint8_t __id_syscall;
void* __args_syscall;
void* __ret_syscall;
void* SP_CALLING_PROCESS;

extern struct kernel_structure kernel;

ISR(INT5_vect){
/*Interrupt delle syscall*/
//Se non sono nello stack del kernel, ci vado

	if(SP < START_RAM-DIM_STACK_KERNEL){
		SP_CALLING_PROCESS=SP;
		SP=kernel.last_sp;
	}
	
	if((__id_syscall & ~CREATE_PROCESS)==0){
		void* start=*((void**)__args_syscall);
		_create_process(start);
	}
	else if((__id_syscall & ~DELETE_PROCESS)==0){
		pid_t pid= *((pid_t*)__args_syscall);
		_delete_process(pid);
	}
	else printf("Abort: no syscall\n");
	
	if(SP_CALLING_PROCESS != (void*)0x00)
		SP=SP_CALLING_PROCESS;
		
	SP_CALLING_PROCESS=(void*)0x00;
}

void _init_syscall(){	
/*abilitazione trap delle syscall*/
	cli();
	EICRB |= 0x0C;
	EIMSK |= TRAP_MASK;
	DDRE |= TRAP_MASK;
	sei();
}

void _syscall(uint8_t id, void* args, void* ret){
/*Interfaccia per le syscall, se non tornano dei risultati ret può essere NULL*/
	__id_syscall=id;
	__args_syscall=args;
	__ret_syscall=ret;
	
	PORTE |= TRAP_MASK; 			//attivazione trap
	PORTE &= ~TRAP_MASK; 			//disattivazione per le chiamate successive
}

