#define NPAGES 16
#define DIM_STACK_PROC 200
#define DIM_STACK_KERNEL (DIM_PAGE*2)
#define DIM_PAGE ( (START_RAM-END_RAM)/NPAGES )
#define DIM_HEAP ( DIM_PAGE-DIM_STACK_PROC )
#define START_RAM 0x2200
#define END_RAM 0x0200
#define START_HEAP(p) ( START_RAM-(( (p)+1)*DIM_PAGE) )
#define N_BLOCCHI_HEAP 24
#define DIM_BLOCCHI_HEAP ( DIM_HEAP/N_BLOCCHI_HEAP )

#include <stdint.h>

typedef int page_t;

typedef struct _heap_block{
	uint16_t size;
	struct _heap_block *next;
} heap_block;

void _init_memory();
void* _get_page(page_t page);
void _lock_page(page_t page);
void _unlock_page(page_t page);
void _request_page(page_t *page);
#ifdef DEBUG
void print_lockbits();
void print_memory();
#endif

