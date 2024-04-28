#include "scheduler.h"
#ifdef DEBUG
#include <stdio.h>
#endif

struct process procList[MAX_PROC];
struct _simple_scheduler schedule;

void _init_scheduler(){
	schedule.current_pid=0;
	schedule.active_process=0;
	schedule.start_arraylist=BAD_ALLOC;
	for(int i=0; i<MAX_PROC; i++) schedule.arrayListProc[i]=BAD_ALLOC;
}

struct process* _add_process_to_scheduler(void* f, uint8_t *pid){
	uint8_t i=0;
	struct process *p=(void*)0x00;
	while(i<MAX_PROC && procList[i].func_addr!=(void*)0x00) i++;
	if(i < MAX_PROC){
		procList[i].func_addr=f;
		procList[i].stato=RUN;
		procList[i].contesto.sp=0;
		procList[i].contesto.sreg=0;
		schedule.active_process++;
		
		if(schedule.start_arraylist==BAD_ALLOC) schedule.start_arraylist=0;
		else{
			uint8_t idx=schedule.start_arraylist;
			while(schedule.arrayListProc[idx] != BAD_ALLOC) idx=schedule.arrayListProc[idx];
			schedule.arrayListProc[idx]=idx+1;
		}
		*pid=i;
		p=&procList[i];
	}
	return p;		
}

void _remove_process_from_scheduler(uint8_t pid){
	if(procList[pid].func_addr==(void*)0x00) printf("Abort: no process in %hhu\n", pid);
	else{
		procList[pid].func_addr=(void*)0x00;
		procList[pid].stato=FINISH;
		procList[pid].contesto.sp=0;
		procList[pid].contesto.sreg=0;
		schedule.active_process--;
		
		if(pid==schedule.start_arraylist){
			schedule.start_arraylist=schedule.arrayListProc[pid];
			schedule.arrayListProc[pid]=BAD_ALLOC;
		}
		else{
			uint8_t idx=schedule.start_arraylist;
			while(schedule.arrayListProc[idx] != pid) idx=schedule.arrayListProc[idx];
			schedule.arrayListProc[idx]=schedule.arrayListProc[pid];
			schedule.arrayListProc[pid]=BAD_ALLOC;
		}
	}
}

struct process* _next_process(uint8_t *pid){
	uint8_t next = schedule.arrayListProc[schedule.current_pid]==BAD_ALLOC ? schedule.start_arraylist : schedule.arrayListProc[schedule.current_pid];
	*pid=next;
	return &(procList[next]);
}

struct process* _get_current_process(){
	return &(procList[schedule.current_pid]);
}

uint8_t _get_current_pid(){
	return schedule.current_pid;
}

void _set_current_pid(uint8_t p){
	schedule.current_pid=p;
}

