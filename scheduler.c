#include "scheduler.h"
#ifdef DEBUG
#include <stdio.h>
#endif

struct process procList[MAX_PROC];
struct _simple_scheduler scheduler;

void _init_scheduler(){
	scheduler.current_pid=0;
	scheduler.active_process=0;
	scheduler.start_arraylist=BAD_ALLOC;
	for(int i=0; i<MAX_PROC; i++) scheduler.arrayListProc[i]=BAD_ALLOC;
}

struct process* _add_process_to_scheduler(void* f, uint8_t *pid){
	uint8_t i=0;
	struct process *p=(void*)0x00;
	while(i<MAX_PROC && procList[i].func_addr!=(void*)0x00) i++;
	if(i < MAX_PROC){
		procList[i].func_addr=f;
		procList[i].stato=CREATED;
		procList[i].contesto.sp=0;
		scheduler.active_process++;
		
		if(scheduler.start_arraylist==BAD_ALLOC) scheduler.start_arraylist=0;
		else{
			uint8_t idx=scheduler.start_arraylist;
			while(scheduler.arrayListProc[idx] != BAD_ALLOC) idx=scheduler.arrayListProc[idx];
			scheduler.arrayListProc[idx]=idx+1;
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
		procList[pid].contesto.sp=0x00;
		scheduler.active_process--;
		
		if(pid==scheduler.start_arraylist){
			scheduler.start_arraylist=scheduler.arrayListProc[pid];
			scheduler.arrayListProc[pid]=BAD_ALLOC;
		}
		else{
			uint8_t idx=scheduler.start_arraylist;
			while(scheduler.arrayListProc[idx] != pid) idx=scheduler.arrayListProc[idx];
			scheduler.arrayListProc[idx]=scheduler.arrayListProc[pid];
			scheduler.arrayListProc[pid]=BAD_ALLOC;
		}
	}
}

struct process* _next_process(uint8_t *pid){
	uint8_t next = scheduler.arrayListProc[scheduler.current_pid]==BAD_ALLOC ? scheduler.start_arraylist : scheduler.arrayListProc[scheduler.current_pid];
	*pid=next;
	return &(procList[next]);
}

struct process* _get_current_process(){
	return &(procList[scheduler.current_pid]);
}

uint8_t _get_current_pid(){
	return scheduler.current_pid;
}

void _set_current_pid(uint8_t p){
	scheduler.current_pid=p;
}

uint8_t _get_active_process(){
	return scheduler.active_process;
}

