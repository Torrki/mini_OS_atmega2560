#include "memory.h"

#ifdef DEBUG
#include <stdio.h>
#endif

uint16_t memory[N_PAGES];
uint16_t lockbits;

void		_init_memory(){
	lockbits = 0x0003;
	for(uint8_t j=0; j<N_PAGES; j++)
		memory[j]=START_RAM-(j*DIM_PAGE);
}

void		_lock_page(page_t p){
	lockbits |= 1<<(uint16_t)p;
}

void		_unlock_page(page_t p){
	lockbits &= ~(1<<(uint16_t)p);
}

page_t	_request_page(){
	//scorro i lockbit fino a trovare una pagina libera
	uint16_t tmp=lockbits;
	page_t pagina=0;
	while( tmp & 0x0001 ){
		pagina++;
		tmp >>= 1;
	}
	
	if(pagina >= N_PAGES) pagina=-1;	//out of pages
	
	return pagina;
}

uint16_t	_get_page(page_t p){
	return memory[p];
}

#ifdef DEBUG
void 	_print_lockbits(){
	printf("lockbits: %04X\n", lockbits);
}

void 	_print_memory(){
	for(uint8_t j=0; j<N_PAGES; j++)
		printf("pagina %hu: %04X\n", j, memory[j]);
}
#endif

