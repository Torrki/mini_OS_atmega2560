#define NPAGES 16
#define DIM_STACK_PROC 200
#define DIM_STACK_KERNEL 150
#define START_RAM 0x2200
#define END_RAM 0x0200
#define DIM_PAGE (START_RAM-END_RAM)/NPAGES
#define N_UNIT_HEAP 24
#define MIN_HEAP_UNIT (DIM_PAGE-DIM_STACK_PROC)/N_UNIT_HEAP
#include <stdint.h>

void _init_memory();
void print_memory();
void* _get_page(uint8_t k);
void _lock_page(uint8_t k);
void _unlock_page(uint8_t k);

