#pragma once
#define SYSCALL_H
#include <stdint.h>

#define SYSCALL_VECT 2
#define CREATE_PROCESS 0x00
#define DELETE_PROCESS 0x01

void _syscall(uint8_t id, void* args);
void _init_syscall_vect(void);

