#pragma once

#include "OS_types.h"

#define N_PAGES 16
#define START_RAM ( (uint16_t)0x2200 )
#define END_RAM ( (uint16_t)0x0200 )
#define DIM_PAGE ( ( START_RAM-END_RAM ) / N_PAGES )

void			_init_memory();
void			_lock_page(page_t p);
void			_unlock_page(page_t p);
page_t		_request_page();
uint16_t	_get_page(page_t p);

#ifdef DEBUG
void 	_print_lockbits();
void 	_print_memory();
#endif

