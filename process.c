#include <avr/io.h>
#include <avr/interrupt.h>
#include "process.h"
#include "kernel.h"
#include "memory.h"

#ifdef DEBUG
#include <stdio.h>
#endif

#define MASK_TMMODE 0x02
#define MASK_TMPS 0x05
#define TOP_OCR0A 0xFF
#define MASK_TMI 0x02

extern struct kernel_structure kernel;

void _context_switch(struct context *old, struct context *new);
void _first_switch(struct context *old, struct context *new);
void _end_first_switch(struct context *new);
void _end_switch(struct context *new);

ISR(TIMER0_COMPA_vect){
/*Interrupt per il context switch*/
	_stop_timer_process();
	struct process *current=kernel.get_current_process();
	struct process *next=kernel.next_process();
	void (*sw)(struct context*, struct context*);
	
	current->stato=STOP;
	kernel.set_current_pid(next->PID);
	
	if(next->stato==CREATED){
		sw=_first_switch;
	}
	else sw=_context_switch;
	
	next->stato=RUN;
	_start_timer_process();
	sw(&(current->contesto), &(next->contesto));
}

int _create_process(void* f){
/*
Funzione che crea un nuovo processo in memoria e lo inizializza per il context switch.
Torna 0 se è stato eseguito correttamente, altrimenti -1.
*/
	page_t process_page;
	_request_page(&process_page); //creo il processo solo se ho le pagine disponibili
	
	if(process_page > -1){
		pid_t new_pid=0;
		for(; new_pid<MAX_PROC && kernel.procList[new_pid].func_addr ; new_pid++); //trovo il primo PID libero
		
		int res=kernel.add_process_to_scheduler(new_pid);
		if(res == -1) return -1;
		
		uint8_t *sp=(uint8_t*)(_get_page(process_page)+6);
		
		struct process *new_process=&( kernel.procList[new_pid] );
		new_process->func_addr=f;
		new_process->stato=CREATED;
		new_process->page=process_page;
		new_process->contesto.sp=(uint16_t)sp;
		new_process->PID=new_pid;
		new_process->heap=(uint32_t)0;
		new_process->free_list=0x00;

		_lock_page(process_page);
		
		uint16_t addr_f=(uint16_t)f;
		uint16_t addr_end=(uint16_t)_end_process;
		uint8_t bl=addr_f & 0x00ff;													//scrittura indirizzo prima istruzione del processo
		uint8_t bh=addr_f>>8;
		*(sp+1)=0x00;
		*(sp+2)=bh;
		*(sp+3)=bl;
		bl=addr_end & 0x00ff;				//scrittura ritorno per la procedura di terminazione
		bh=addr_end>>8;
		*(sp+4)=0x00;
		*(sp+5)=bh;
		*(sp+6)=bl;
		return 0;
		
	}else return -1;
}

int _delete_process(pid_t pid){
/*
Funzione che elimina un processo e lo rimuove dallo scheduler.
Torna 0 se corretto, altrimenti -1.
*/
	struct process* p=kernel.get_process(pid);
	_unlock_page(p->page);
	if(kernel.remove_process_from_scheduler(pid)==-1) return -1;
	
	p->func_addr=(void*)0x00;
	p->stato=FINISH;
	p->page=0;
	p->contesto.sp=0;
	p->PID=-1;
	
	return 0;
}

void _end_process(){
/*Procedura per la terminazione di un processo*/
	cli();
	_stop_timer_process();
	_reset_timer_process();
	pid_t cpid=kernel.get_current_pid();
	struct process *next=kernel.next_process();
	void (*sw)(struct context*);
	
	_delete_process(cpid);
	
	kernel.set_current_pid(next->PID);
	if(next->stato==CREATED){
		sw=_end_first_switch;
	}
	else sw=_end_switch;	
	next->stato=RUN;
	
	if(kernel.get_active_process() > 1)
		_start_timer_process();
		
	sw(&(next->contesto));
}

int _sleep_process(pid_t pid){
	struct process* p= kernel.get_process(pid);
	if(p != (struct process*)0x00){
		p->stato=SLEEP;
		kernel.sleepProcess[pid]=p;
		kernel.remove_process_from_scheduler(pid);
		return 0;
	}else return -1;
}

int _wake_process(pid_t pid){
	struct process* p= kernel.get_process(pid);
	
	if(p != (struct process*)0x00 && p->stato==SLEEP){
		p->stato=STOP;
		kernel.sleepProcess[pid]=(struct process*)0x00;
		kernel.add_process_to_scheduler(pid);
		return 0;
	}else return -1;
}

void _init_timer_process(){
/*Inizializzazione timer per time slot della CPU per i processi*/
	cli();
	TCCR0A |= MASK_TMMODE;
	OCR0A = TOP_OCR0A;
	TIMSK0 |= MASK_TMI;
	sei();
}

void _start_timer_process(){
	TCCR0B |= MASK_TMPS;
}

void _stop_timer_process(){
	TCCR0B &= ~MASK_TMPS;
}

void _reset_timer_process(){
	TCNT0=0x00;
}

void* _malloc(uint16_t size){
	struct process *current=kernel.get_current_process();
	uint32_t tmp=current->heap;
	float nblocchi_f=(float)(size+sizeof(heap_block)) / (float)DIM_BLOCCHI_HEAP;
	uint8_t nblocchi= (size+sizeof(heap_block)) / (uint16_t)DIM_BLOCCHI_HEAP, k=0, cont=0;
	nblocchi = ( nblocchi_f - nblocchi ) > 0 ? nblocchi+1 : nblocchi;
	
	while(tmp > 0 && cont < N_BLOCCHI_HEAP){
		while(tmp & (uint32_t)1){
			tmp >>= 1;
			cont++;
		}
		for(k=1; k < nblocchi; k++)
			if( (tmp >> k) & (uint32_t)1) break;
			
		if(k==nblocchi) break;
	}
	
	if(cont==N_BLOCCHI_HEAP) return (void*)0x00;
	
	for(k=0; k < nblocchi; k++)
		current->heap |= (uint32_t)1<<(cont+k);
		
	void* address=START_HEAP(current->page) + (cont*DIM_BLOCCHI_HEAP);
	heap_block* tmp_address=address;
	tmp_address->size=nblocchi*DIM_BLOCCHI_HEAP;
	tmp_address->next=0x00;
	
	if(current->free_list == (void*)0x00){
		current->free_list=address;
	}else{
		tmp_address=current->free_list;
		while(tmp_address->next != (heap_block*)0x00) tmp_address=tmp_address->next;
		tmp_address->next=(heap_block*)address;
	}
	
	return (void*)(address+sizeof(heap_block));
}

int _free(void* address){
	address -= sizeof(heap_block);
	
	struct process *current=kernel.get_current_process();
	heap_block* tmp_address=current->free_list;
	
	if(tmp_address==(heap_block*)address){
		current->free_list=tmp_address->next;
	}else{
		while(tmp_address != (heap_block*)0x00 && tmp_address->next != address){
			tmp_address=tmp_address->next;
		}
		
		if(tmp_address==(heap_block*)0x00) return -1;
	
		tmp_address->next = ((heap_block*)address)->next;
	}
	uint16_t k=(uint16_t)(address-START_HEAP(current->page))/(uint16_t)DIM_BLOCCHI_HEAP;
	tmp_address=(heap_block*)address;
	uint8_t nblocchi=tmp_address->size/DIM_BLOCCHI_HEAP;
	
	for(uint8_t j=0; j<nblocchi; j++)
		current->heap &= ~( (uint32_t)1<<(k+j));
		
	return 0;
}

