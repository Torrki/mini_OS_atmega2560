#define SYSCALL_H

#include <stdint.h>

#define CREATE_PROCESS	0x00
#define DELETE_PROCESS	0x01
#define SLEEP_PROCESS		0x02
#define WAKE_PROCESS		0x03

void _syscall(uint8_t id, void** args, void* ret);			//funzione per interfacciamento syscall
void _init_syscall();																		//funzione per inizializzare le syscall

