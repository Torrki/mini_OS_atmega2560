#include "kernel.h"

#ifdef DEBUG
#include <stdio.h>
#endif

extern struct _kernel_structure kernel;

int		_add_pid_to_scheduler(pid_t pid){

	if(kernel.scheduler.startPid==CODA){
		kernel.scheduler.startPid=pid;
		kernel.scheduler.arrayList[pid]=CODA;
		kernel.scheduler.currentPid=pid;
	}else{
		pid_t tmp=kernel.scheduler.startPid;
		while(kernel.scheduler.arrayList[tmp] != CODA) tmp=kernel.scheduler.arrayList[tmp];
		kernel.scheduler.arrayList[tmp]=pid;
		kernel.scheduler.arrayList[pid]=CODA;
	}
	
	return 0;
	
}

int		_remove_pid_from_scheduler(pid_t pid){

	if(kernel.scheduler.startPid==pid){
		kernel.scheduler.startPid=kernel.scheduler.arrayList[pid];
	}else{
		pid_t tmp=kernel.scheduler.startPid;
		
		while(tmp != CODA && kernel.scheduler.arrayList[tmp] != pid) tmp=kernel.scheduler.arrayList[tmp];
		if(tmp == CODA) return -1;
		
		kernel.scheduler.arrayList[tmp]=kernel.scheduler.arrayList[pid];
	}
	kernel.scheduler.arrayList[pid]=BAD_ALLOC;
	return 0;
}

pid_t	_next_pid(){
	pid_t cp=kernel.scheduler.currentPid;
	return kernel.scheduler.arrayList[cp]==CODA ? kernel.scheduler.startPid : kernel.scheduler.arrayList[cp];
}

void _set_current_pid(pid_t pid){
	kernel.scheduler.currentPid=pid;
}

pid_t	_get_current_pid(){
	return kernel.scheduler.currentPid;
}

#ifdef DEBUG
void printScheduler(){
	pid_t tmp=kernel.scheduler.startPid;
	while(tmp != CODA){
		printf("%hd-->", tmp);
		tmp=kernel.scheduler.arrayList[tmp];
	}
	printf("-1\n");
}
#endif

