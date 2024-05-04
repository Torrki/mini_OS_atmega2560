#define SCHEDULER_H
#define BAD_ALLOC -1
#define CODA -2
#define MAX_PROC 15

#include "process.h"
#include <stdint.h>

struct _simple_scheduler{
/*
Semplice scheduler basato su arraylist
*/
	uint8_t current_pid;
	uint8_t active_process;
	int start_arraylist;
	int proc_arraylist[MAX_PROC];		//contiene il PID successivo al PID k-esimo
};

void _init_scheduler();
void* _add_process_to_scheduler(void* f, uint8_t *pid);
int _remove_process_from_scheduler(uint8_t pid);
struct process* _get_current_process();
struct process* _next_process(uint8_t *pid);
uint8_t _get_current_pid();
void _set_current_pid(uint8_t pid);
void print_scheduler();
