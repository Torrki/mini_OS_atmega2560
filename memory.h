#define NPAGES 16
#define DIM_STACK_PROC 200
#define DIM_STACK_KERNEL 400
#define START_RAM 0x2200
#define END_RAM 0x0200
#define DIM_PAGE (START_RAM-END_RAM)/NPAGES
#define N_UNIT_HEAP 24
#define MIN_HEAP_UNIT (DIM_PAGE-DIM_STACK_PROC)/N_UNIT_HEAP

#include <stdint.h>

typedef int page_t;

void _init_memory();
void* _get_page(page_t page);
void _lock_page(page_t page);
void _unlock_page(page_t page);
void _request_page(page_t *page);
#ifdef DEBUG
void print_lockbits();
void print_memory();
#endif

