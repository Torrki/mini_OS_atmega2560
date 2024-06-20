#pragma once

#define SYSCALL_H

#include "OS_types.h"

#define CREATE_PROCESS	0x00
#define DELETE_PROCESS	0x01
#define SLEEP_PROCESS		0x02
#define WAKE_PROCESS		0x03

void 	syscall(uint8_t id, void** args, void* ret);			//funzione per interfacciamento syscall
int		sleep(pid_t p);						//funzione per la chiamata sleep a se stessi
char	getChar();
void	putChar(char c);
void	led_on();
void	led_off();
