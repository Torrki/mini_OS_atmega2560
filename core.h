#define CORE_H

#define MAX_PROC 10
#define DIM_PROC 150
#define DIM_STACK_KERNEL 250
#define START_RAM 0x2200

void _init_OS();																												//funzione inizializzazione OS
void _start_OS();																												//funzione per far partire l'OS dal pid 0
void _context_switch(struct context *old, struct context *new);					//funzione asm per il context switch
void _first_switch(struct context *old, struct context *new);						//funzione asm per il primo switch
void _end_first_switch(struct context *new);														//funzione asm per switch tra processo terminato e uno creato
void _end_switch(struct context *new);																	//funzione asm per switch tra processo terminato e uno stoppato

