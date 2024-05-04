#define SYSCALL_H

#include <stdint.h>

#define CREATE_PROCESS 0x00
#define DELETE_PROCESS 0x01

void _syscall(uint8_t id, void* args);				//funzione per interfacciamento syscall
void _init_syscall();													//funzione per inizializzare le syscall

