#include "simple_scheduler.h"

#ifdef DEBUG
#include <stdio.h>
#endif

struct _simple_scheduler s;

void _init_scheduler(){
	s.start_arraylist=CODA;
	s.current_pid=BAD_ALLOC;
	for(uint8_t i=0; i<MAX_PROC; i++) s.proc_arraylist[i]=BAD_ALLOC;
}

int _add_process_to_scheduler(pid_t pid){
/*
Aggiunge un nuovo PID nello scheduler s.
Torna 0 se corretto, altrimenti -1.
*/
	if(s.start_arraylist==CODA){ //se non è presente la testa
		s.start_arraylist=pid;
		s.current_pid=pid;
		s.proc_arraylist[pid]=CODA;
	}else{
		//arrivo alla coda
		pid_t k=s.start_arraylist;
		while(s.proc_arraylist[k] != CODA) k=s.proc_arraylist[k];
		
		s.proc_arraylist[k]=pid;
		s.proc_arraylist[pid]=CODA;
	}
	
	s.active_process++;
	return 0;
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
	
	s.active_process--;
	return 0;
}

void _next_pid(pid_t *pid){
	pid_t next_pid=s.proc_arraylist[s.current_pid]==CODA ? s.start_arraylist : s.proc_arraylist[s.current_pid];
	*pid=next_pid;
}

pid_t _get_current_pid(){
	return s.current_pid;
}

void _set_current_pid(pid_t pid){
	s.current_pid=pid;
}

uint8_t _get_active_process(){
	return s.active_process;
}

#ifdef DEBUG
void print_scheduler(){
	pid_t k=s.start_arraylist;
	if(k==CODA) printf("Lista vuota\n");
	else{
		while(s.proc_arraylist[k] != CODA){
			printf("elem: %hd\n", k);
			k=s.proc_arraylist[k];
		}
		printf("elem: %hd\n", k);
	}
}
#endif

