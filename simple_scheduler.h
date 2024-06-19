#pragma once

#include "OS_types.h"

#define BAD_ALLOC -1
#define CODA -2
#define MAX_PROC 10

typedef struct _arrayList_scheduler{
	pid_t		currentPid;
	pid_t		startPid;
	pid_t		arrayList[MAX_PROC];
}	arrayListSched;


#ifdef DEBUG
void printScheduler();
#endif
