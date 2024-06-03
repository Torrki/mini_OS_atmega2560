#include "kernel.h"
#include "simple_scheduler.h"

struct kernel_structure kernel;

void _init_kernel(){
	_init_scheduler();
	
	kernel.last_sp=(void*)0x00;
	kernel.add_process_to_scheduler= &_add_process_to_scheduler;
	kernel.remove_process_from_scheduler= &_remove_process_from_scheduler;
	kernel.get_current_process=&_get_current_process;
	kernel.get_process=&_get_process;
	kernel.next_process=&_next_process;
	kernel.get_current_pid=&_get_current_pid;
	kernel.set_current_pid=&_set_current_pid;
}

