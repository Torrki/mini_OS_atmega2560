#define SCHEDULER_H
#define BAD_ALLOC -1
#define CODA -2

#include "process.h"
#include <stdint.h>

struct _simple_scheduler{
/*
Semplice scheduler basato su arraylist
*/
	pid_t current_pid;
	uint8_t active_process;
	pid_t start_arraylist;
	pid_t proc_arraylist[MAX_PROC];		//contiene il PID successivo al PID k-esimo
};

void _init_scheduler();
int _add_process_to_scheduler(pid_t *pid);
int _remove_process_from_scheduler(pid_t pid);
void _next_pid(pid_t *pid);
pid_t _get_current_pid();
void _set_current_pid(pid_t pid);
uint8_t _get_active_process();

#ifdef DEBUG
void print_scheduler();
#endif

