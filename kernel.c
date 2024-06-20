#include "kernel.h"

struct _kernel_structure kernel;

void _init_kernel(){
	kernel.scheduler.currentPid=-1;
	kernel.scheduler.startPid=CODA;
	kernel.startTX=CODA;
	kernel.startRX=CODA;
	
	for(pid_t j=0; j<MAX_PROC; j++){
		kernel.ProcList[j].func_addr=(uint16_t)0;
		kernel.ProcList[j].stato=STOP;
		kernel.ProcList[j].page=-1;
		kernel.ProcList[j].PID=-1;
		
		kernel.scheduler.arrayList[j]=BAD_ALLOC;
		kernel.arrayListTX[j]=BAD_ALLOC;
		kernel.arrayListRX[j]=BAD_ALLOC;
	}
}

Process* _get_process(pid_t pid){
	return &(kernel.ProcList[pid]);
}
