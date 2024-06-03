#include "kernel.h"
#include "simple_scheduler.h"

struct kernel_structure kernel;

void _init_kernel(){
	_init_scheduler();
	
	kernel.last_sp=(void*)0x00;
	kernel.add_process_to_scheduler= &_add_process_to_scheduler;
	kernel.remove_process_from_scheduler= &_remove_process_from_scheduler;
	kernel.next_pid=&_next_pid;
	kernel.get_current_pid=&_get_current_pid;
	kernel.set_current_pid=&_set_current_pid;
	kernel.get_active_process=_get_active_process;
}

struct process* get_current_process(){
	return &( kernel.procList[_get_current_pid()] );
}

struct process* get_process(pid_t pid){
	if(kernel.procList[pid].func_addr == (void*)0x00) return (void*)0x00;
	else{
		return &( kernel.procList[pid] );
	}
}

struct process* next_process(pid_t *next){
	pid_t next_pid;
	_next_pid(&next_pid);
	*next=next_pid;
	
	return &( kernel.procList[next_pid] );
}

