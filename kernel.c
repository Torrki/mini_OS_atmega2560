#include "kernel.h"
#include "simple_scheduler.h"

struct kernel_structure kernel;

int queueProcess(struct processFIFO* fifo, struct process* p);
struct process* dequeueProcess(struct processFIFO* fifo);

struct process* get_current_process();
struct process* next_process();
struct process* get_process(pid_t pid);

void _init_kernel(){
	_init_scheduler();
	
	kernel.add_process_to_scheduler= &_add_process_to_scheduler;
	kernel.remove_process_from_scheduler= &_remove_process_from_scheduler;
	kernel.next_pid=&_next_pid;
	kernel.get_current_pid=&_get_current_pid;
	kernel.set_current_pid=&_set_current_pid;
	kernel.get_active_process=_get_active_process;
	kernel.get_current_process=get_current_process;
	kernel.next_process=next_process;
	kernel.get_process=get_process;
	
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

struct process* next_process(){
	pid_t next;
	_next_pid(&next);
	
	return &( kernel.procList[next] );
}

int queueProcess(struct processFIFO* fifo, struct process* p){
	if(fifo->size < MAX_PROC){
		fifo->list[fifo->tail]=p;
		
		fifo->tail = fifo->tail == MAX_PROC-1 ? 0 : fifo->tail+1;		//coda circolare
		fifo->size++;	
		return 0;
	}return -1;
}

struct process* dequeueProcess(struct processFIFO* fifo){
	if(fifo->size > 0){
		struct process* p=fifo->list[fifo->head];
		fifo->list[fifo->head]=(struct process*)0x00;
		
		fifo->head = fifo->head == MAX_PROC-1 ? 0 : fifo->head+1;
		fifo->size--;
		return p;
	}else return (void*)0x00;
}

