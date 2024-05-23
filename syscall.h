#define SYSCALL_H

#include <stdint.h>

#define CREATE_PROCESS 0x00
#define DELETE_PROCESS 0x01

void _syscall(uint8_t id, void* args, void* ret);			//funzione per interfacciamento syscall
void _init_syscall();																	//funzione per inizializzare le syscall
void* malloc(uint32_t size);
void free(void* addr);

