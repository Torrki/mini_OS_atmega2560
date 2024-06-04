#include "memory.h"

#ifdef DEBUG
#include <stdio.h>
#endif

void* memoria[NPAGES];
uint16_t lockbits;

void _init_memory(){
	lockbits=0x0003;	//blocco le pagine per il kernel
	for(uint8_t i=0; i<NPAGES; i++){
		memoria[i]=(void*)(START_RAM-DIM_PAGE*i);
	}
}

void* _get_page(page_t page){
	return memoria[page];
}

void _lock_page(page_t page){
	lockbits |= 1<<page;
}

void _unlock_page(page_t page){
	lockbits &= ~(1<<page);
}

void _request_page(page_t *page){
	/*Scorro fino a quando non trovo una pagina libera.
	Se non trovo nessuna pagina, torno -1*/
	uint16_t tmp=lockbits;
	uint8_t k=0;
	while((k < NPAGES) && (tmp & (uint16_t)1)){
		k++;
		tmp >>= 1;
	}
	if(k==NPAGES) *page=-1;		//out of page
	else *page=k;
}
#ifdef DEBUG
void print_lockbits(){
	printf("lockbits: %X\n", lockbits);
}
void print_memory(){
	for(uint8_t i=0; i<NPAGES; i++){
		printf("%hu: %04X\n", i, memoria[i]);
	}
}
#endif

