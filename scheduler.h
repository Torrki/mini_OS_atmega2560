#define SCHEDULER_H

#include <stdint.h>
#include "process.h"

#define BAD_ALLOC -1

struct _simple_scheduler{
	uint8_t current_pid;
	uint8_t active_process;
	int8_t start_arraylist;
	int arrayListProc[MAX_PROC];
};

void _init_scheduler();
struct process* _add_process_to_scheduler(void* f, uint8_t *pid);				//funzione per aggiungere allo scheduler
void _remove_process_from_scheduler(uint8_t pid);												//funzione per rimuovere dallo scheduler
struct process* _next_process(uint8_t *pid);														//funzione per ottenere il processo successivo
struct process* _get_current_process();																	//funzione per ottenere il processo corrente
uint8_t _get_current_pid();																							//funzione per ottenere il pid corrente
uint8_t _get_active_process();																					//funzione per ottenere il numero dei processi attivi
void _set_current_pid(uint8_t p);																				//funzione per modificare il pid corrente

