#include "core.h"
#include <avr/io.h>
#ifdef DEBUG
#include "avr_common/uart.h"
#include <stdio.h>
#endif

struct _OS_context OS_state;

void _init_OS(){
	_init_timer_process();
	_init_syscall_vect();
}

