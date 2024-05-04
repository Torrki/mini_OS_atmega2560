#include "memory.h"

#ifdef DEBUG
#include <stdio.h>
#endif

void* memoria[NPAGES];
uint16_t lockbits;

void _init_memory(){
	lockbits=0x0000;
	for(uint8_t i=0; i<NPAGES; i++){
		memoria[i]=START_RAM-DIM_PAGE*i;
	}
}

void print_memory(){
	for(uint8_t i=0; i<NPAGES; i++){
		printf("%hu: %X\n", i, memoria[i]);
	}
}

void* _get_page(uint8_t k){
	return memoria[k];
}

void _lock_page(uint8_t k){
	lockbits |= 1<<k;
}

void _unlock_page(uint8_t k){
	lockbits &= ~(1<<k);
}

