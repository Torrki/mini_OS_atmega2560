#include <avr/interrupt.h>
#include <avr/io.h>
#include "syscall.h"
#include "process.h"
#ifdef DEBUG
#include <stdio.h>
#endif

#define TRAP_MASK 0x20
uint8_t __id_syscall;
void* __args_syscall;

ISR(INT5_vect){
	if((__id_syscall & ~CREATE_PROCESS)==0){
		void* start=*((void**)__args_syscall);
		_create_process(start);
	}
	else if((__id_syscall & ~DELETE_PROCESS)==0){
		uint8_t pid= *((uint8_t*)__args_syscall);
		_delete_process(pid);
	}
	else printf("Abort: no syscall\n");
}

void _init_syscall(){	
/*abilitazione trap delle syscall*/
	cli();
	EICRB |= 0x0C;
	EIMSK |= TRAP_MASK;
	DDRE |= TRAP_MASK;
	sei();
}

void _syscall(uint8_t id, void* args){
	__id_syscall=id;
	__args_syscall=args;
	PORTE |= TRAP_MASK; 			//attivazione trap
	PORTE &= ~TRAP_MASK; 			//disattivazione per le chiamate successive
}

void* malloc(uint32_t size){
	return _malloc(size);
}

void free(void* addr){
	_free(addr);
}

