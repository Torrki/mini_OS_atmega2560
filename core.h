#define CORE_H

void _init_OS();																												//funzione inizializzazione OS
void _start_OS();																												//funzione per far partire l'OS dal pid 0
void _context_switch(struct context *old, struct context *new);					//funzione asm per il context switch
void _first_switch(struct context *old, struct context *new);						//funzione asm per il primo switch
void _end_first_switch(struct context *new);														//funzione asm per switch tra processo terminato e uno creato
void _end_switch(struct context *new);																	//funzione asm per switch tra processo terminato e uno stoppato

