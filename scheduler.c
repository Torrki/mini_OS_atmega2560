#include "scheduler.h"
#include "memory.h"

#ifdef DEBUG
#include <stdio.h>
#endif

struct process procList[MAX_PROC];
typedef struct _simple_scheduler scheduler;
scheduler s;

void _init_scheduler(){
	s.start_arraylist=CODA;
	for(uint8_t i=0; i<MAX_PROC; i++) s.proc_arraylist[i]=BAD_ALLOC;
}

void* _add_process_to_scheduler(void *f, uint8_t *pid){
/*
Aggiunge un nuovo PID nello scheduler s.
Torna un indirizzo valido se corretto, altrimenti NULL.
*/
	uint8_t new_pid;
	if(s.start_arraylist==CODA){ //se non è presente la testa
		new_pid=0;
		s.start_arraylist=0;
		s.proc_arraylist[0]=CODA;
	}else{
		//trovo il primo slot libero
		uint8_t i=0;
		while(i<MAX_PROC && s.proc_arraylist[i] != BAD_ALLOC) i++;
		if(i==MAX_PROC) return (void*)0x00;
		
		//arrivo alla coda
		uint8_t k=s.start_arraylist;
		while(s.proc_arraylist[k] != CODA) k=s.proc_arraylist[k];
		s.proc_arraylist[k]=i;
		s.proc_arraylist[i]=CODA;
		new_pid=i;
	}
	procList[new_pid].func_addr=f;
	procList[new_pid].stato=CREATED;
	procList[new_pid].page=new_pid+1;
	procList[new_pid].contesto.sp=START_RAM-(DIM_PAGE*(new_pid+1)+6);
	procList[new_pid].heap_alloc=(uint32_t)0;
	return &(procList[new_pid]);
}

int _remove_process_from_scheduler(uint8_t pid){
/*
Rimuove un PID dallo scheduler e riorganizza la collezione
Torna 0 se corretto, altrimenti -1.
*/
	int succ;
	if(s.start_arraylist==pid){
		succ=s.proc_arraylist[s.start_arraylist];
		s.start_arraylist=succ;
	}
	else{
		int k=s.start_arraylist;
		succ=s.proc_arraylist[pid];
		
		//arrivo all'elemento precedente da eliminare
		while(s.proc_arraylist[k] != pid){
			k=s.proc_arraylist[k];
			if(k==CODA) return -1;
		}
		s.proc_arraylist[k]=succ;
		s.proc_arraylist[pid]=BAD_ALLOC;
	}
	
	procList[pid].func_addr=(void*)0x00;
	procList[pid].stato=FINISH;
	procList[pid].page=0;
	procList[pid].contesto.sp=0;
	procList[pid].heap_alloc=(uint32_t)0;
	return 0;
}

struct process* _get_current_process(){
	return &(procList[s.current_pid]);
}

struct process* _next_process(uint8_t *pid){
	uint8_t next_pid=s.proc_arraylist[s.current_pid]==CODA ? s.start_arraylist : s.proc_arraylist[s.current_pid];
	*pid=next_pid;
	return &(procList[next_pid]);
}

uint8_t _get_current_pid(){
	return s.current_pid;
}

void _set_current_pid(uint8_t pid){
	s.current_pid=pid;
}

void print_scheduler(){
	int k=s.start_arraylist;
	if(k==CODA) printf("Lista vuota\n");
	else{
		while(s.proc_arraylist[k] != CODA){
			printf("elem: %hu\n", k);
			k=s.proc_arraylist[k];
		}
		printf("elem: %hu\n", k);
	}
}

