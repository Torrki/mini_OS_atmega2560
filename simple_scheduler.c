#include "simple_scheduler.h"
#include "memory.h"

#ifdef DEBUG
#include <stdio.h>
#endif

extern struct kernel_structure kernel;
struct _simple_scheduler s;

void _init_scheduler(){
	s.start_arraylist=CODA;
	s.current_pid=BAD_ALLOC;
	for(uint8_t i=0; i<MAX_PROC; i++) s.proc_arraylist[i]=BAD_ALLOC;
}

struct process* _add_process_to_scheduler(void *f, pid_t *pid){
/*
Aggiunge un nuovo PID nello scheduler s.
Torna un indirizzo valido se corretto, altrimenti NULL.
*/
	page_t process_page;
	_request_page(&process_page);
	
	if(process_page > -1){	//solo se ho memoria a sufficienza creo un nuovo processo
		pid_t new_pid;
		if(s.start_arraylist==CODA){ //se non è presente la testa
			new_pid=0;
			s.start_arraylist=0;
			s.current_pid=0;
			s.proc_arraylist[0]=CODA;
		}else{
			//trovo il primo slot libero
			pid_t i=0;
			while(i<MAX_PROC && s.proc_arraylist[i] != BAD_ALLOC) i++;
			if(i==MAX_PROC) return (void*)0x00;
			
			//arrivo alla coda
			pid_t k=s.start_arraylist;
			while(s.proc_arraylist[k] != CODA) k=s.proc_arraylist[k];
			s.proc_arraylist[k]=i;
			s.proc_arraylist[i]=CODA;
			new_pid=i;
		}
		kernel.procList[new_pid].func_addr=f;
		kernel.procList[new_pid].stato=CREATED;
		kernel.procList[new_pid].page=process_page;
		kernel.procList[new_pid].contesto.sp=START_RAM-(DIM_PAGE*(process_page)+6);
		return &(kernel.procList[new_pid]);
	}else return (void*)0x00;
}

int _remove_process_from_scheduler(pid_t pid){
/*
Rimuove un PID dallo scheduler e riorganizza la collezione
Torna 0 se corretto, altrimenti -1.
*/
	pid_t succ;
	if(s.start_arraylist==pid){
		succ=s.proc_arraylist[s.start_arraylist];
		s.start_arraylist=succ;
	}
	else{
		pid_t k=s.start_arraylist;
		succ=s.proc_arraylist[pid];
		
		//arrivo all'elemento precedente da eliminare
		while(s.proc_arraylist[k] != pid){
			k=s.proc_arraylist[k];
			if(k==CODA) return -1;
		}
		s.proc_arraylist[k]=succ;
		s.proc_arraylist[pid]=BAD_ALLOC;
	}
	
	kernel.procList[pid].func_addr=(void*)0x00;
	kernel.procList[pid].stato=FINISH;
	kernel.procList[pid].page=0;
	kernel.procList[pid].contesto.sp=0;
	return 0;
}

struct process* _get_current_process(){
	if(s.current_pid==BAD_ALLOC) return (void*)0x00;
	return &(kernel.procList[s.current_pid]);
}

struct process* _get_process(pid_t pid){
	return &(kernel.procList[pid]);
}

struct process* _next_process(pid_t *pid){
	pid_t next_pid=s.proc_arraylist[s.current_pid]==CODA ? s.start_arraylist : s.proc_arraylist[s.current_pid];
	*pid=next_pid;
	return &(kernel.procList[next_pid]);
}

pid_t _get_current_pid(){
	return s.current_pid;
}

void _set_current_pid(pid_t pid){
	s.current_pid=pid;
}

#ifdef DEBUG
void print_scheduler(){
	pid_t k=s.start_arraylist;
	if(k==CODA) printf("Lista vuota\n");
	else{
		while(s.proc_arraylist[k] != CODA){
			printf("elem: %hu\n", k);
			k=s.proc_arraylist[k];
		}
		printf("elem: %hu\n", k);
	}
}
#endif

