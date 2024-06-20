#include <avr/interrupt.h>
#include <avr/io.h>
#include "syscall.h"
#include "kernel.h"

#ifdef DEBUG
#include <stdio.h>
#endif

#define TRAP_MASK 0x20

uint8_t __id_syscall;
void** 	__args_syscall;
void* 	__ret_syscall;
uint16_t SP_CALLING_PROCESS;

extern struct _kernel_structure kernel;

ISR(INT5_vect){
/*Interrupt delle syscall*/	
	if((__id_syscall & ~CREATE_PROCESS)==0){
		void* start=__args_syscall[0];
		*((int*)__ret_syscall)=_create_process(start);
	}
	else if((__id_syscall & ~DELETE_PROCESS)==0){
		pid_t* pid= __args_syscall[0];
		
		if(*pid != _get_current_pid())
			*((int*)__ret_syscall)=_delete_process(*pid);
	}
	else if((__id_syscall & ~SLEEP_PROCESS)==0){
		pid_t* pid= __args_syscall[0];
		
		if(*pid != _get_current_pid())
			*((int*)__ret_syscall)=_sleep_process(*pid);
	}
	else if((__id_syscall & ~WAKE_PROCESS)==0){
		pid_t* pid= __args_syscall[0];
		*((int*)__ret_syscall)=_wake_process(*pid);
	}
}

void _init_syscall(){	
/*abilitazione trap delle syscall*/
	cli();
	EICRB |= 0x0C;
	EIMSK |= TRAP_MASK;
	DDRE |= TRAP_MASK;
	sei();
}

void syscall(uint8_t id, void** args, void* ret){
/*Interfaccia per le syscall, se non tornano dei risultati ret può essere NULL*/
	SP_CALLING_PROCESS=SP;
//Se non sono nello stack del kernel, ci vado
	if(kernel.last_SP){		//viene impostato solo quando parte il sistema operativo
		SP=kernel.last_SP;
	}
	__id_syscall=id;
	__args_syscall=args;
	__ret_syscall=ret;
	
	PORTE |= TRAP_MASK; 			//attivazione trap
	PORTE &= ~TRAP_MASK; 			//disattivazione per le chiamate successive
	
	SP=SP_CALLING_PROCESS;
}

inline void	led_on(){
	PORTB |= 0x80;
}

inline void	led_off(){
	PORTB &= ~0x80;
}

