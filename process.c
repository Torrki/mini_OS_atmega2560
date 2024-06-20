#include <avr/io.h>
#include <avr/interrupt.h>
#include "kernel.h"

#define MASK_TMMODE 0x02
#define MASK_TMPS 0x05
#define TOP_OCR0A 0xFF
#define MASK_TMI 0x02

#ifdef DEBUG
#include <stdio.h>
#endif

extern struct _kernel_structure kernel;

void _context_switch(PContext *old, PContext *new);		//context switch ordinari
void _first_switch(PContext *old, PContext *new);

void _end_first_switch(PContext *new);	//context switch per la terminazione
void _end_switch(PContext *new);

void push_regs();
void pop_regs();


ISR(TIMER0_COMPA_vect){
/*Interrupt per il context switch*/
	_stop_timer_process();
	Process* current=_get_process( _get_current_pid() );
	Process* next=_get_process( _next_pid() );
	void (*cw)(PContext*, PContext*)=0x0000;
	current->stato=STOP;
	
	switch(next->stato){
	case	CREATED:
		cw=_first_switch;
		break;
	case	STOP:
		cw=_context_switch;
		break;
	}
	
	_set_current_pid(next->PID);
	next->stato=RUN;
	_start_timer_process();
	cw(&(current->contesto), &(next->contesto));
}

int		_create_process(void* f){
	//vedo se ho memoria per creare un nuovo processo
	
	page_t processPage=_request_page();
	if(processPage == -1) return -1;
	
	//trovo il primo PID libero
	pid_t newPid=0;
	for(; newPid < MAX_PROC && kernel.ProcList[newPid].func_addr ; newPid++)
	
	if(newPid==MAX_PROC) return -1;
	Process* new_process= _get_process(newPid);
	_add_pid_to_scheduler(newPid);
	
	uint16_t f_addr=(uint16_t)f, end_addr= (uint16_t)&_end_process;
	uint8_t *sp= START_RAM-(processPage*DIM_PAGE + 6); //lascio i 3 byte per la prima istruzione
	new_process->func_addr=f_addr;
	new_process->PID=newPid;
	new_process->page=processPage;
	new_process->stato=CREATED;
	new_process->contesto.sp=(uint16_t)sp;

	*(sp+1)=	0x00;
	*(sp+2)= (f_addr >> 8) & 0x00FF;
	*(sp+3)=	f_addr & 0x00FF;
	*(sp+4)=	0x00;
	*(sp+5)= (end_addr >> 8) & 0x00FF;
	*(sp+6)=	end_addr & 0x00FF;
	
	_lock_page(processPage);
	
	return 0;
}

int		_delete_process(pid_t p){
	Process* proc=_get_process(p);
	if(proc->func_addr==(uint16_t)0) return -1;
	
	_unlock_page(proc->page);
	proc->func_addr=(uint16_t)0;
	proc->PID=-1;
	proc->page=-1;
	proc->stato=STOP;
	proc->contesto.sp=(uint16_t)0;
	
	_remove_pid_from_scheduler(p);
	
	return 0;
}

void	_end_process(){
/*Procedura per la terminazione di un processo*/
	cli();
	_stop_timer_process();
	_reset_timer_process();
	pid_t cp=_get_current_pid(), np=_next_pid();
	Process* next=_get_process(np);
	void (*sw)(PContext*);
	
	_delete_process(cp);
	
	_set_current_pid(np);
	if(next->stato==CREATED)	sw=_end_first_switch;
	else	sw=_end_switch;	
	next->stato=RUN;
//	printf("Terminato: %hd\n", cp);
	sw(&(next->contesto));
}

int		_sleep(pid_t p){
	if(p==_get_current_pid()){		//se metto a dormire me stesso
		_stop_timer_process();
		_reset_timer_process();
		
		Process* current=_get_process(p);		
		Process* next=_get_process( _next_pid() ); //eseguo un cw straordinario
		void (*cw)(PContext*, PContext*)=0x0000;
		current->stato=SLEEP;
	
		switch(next->stato){
		case	CREATED:
			cw=_first_switch;
			break;
		case	STOP:
			cw=_context_switch;
			break;
		default:
			return -1;
		}
	
		_set_current_pid(next->PID);
		next->stato=RUN;
		
		_remove_pid_from_scheduler(p);
		_start_timer_process();
		
		cw(&(current->contesto), &(next->contesto));
	}else{
		//chiamata syscall _sleep_process
	}
	
	return 0;
}

int		_wake_process(pid_t p){
	Process* proc=_get_process(p);
	
	if(proc->PID==-1 || proc->stato != SLEEP) return -1;
	
	proc->stato=STOP;
	_add_pid_to_scheduler(p);
	return 0;
}

inline void _init_timer_process(){
/*Inizializzazione timer per time slot della CPU per i processi*/
	cli();
	TCCR0A |= MASK_TMMODE;
	OCR0A = TOP_OCR0A;
	TIMSK0 |= MASK_TMI;
	sei();
}

inline void _start_timer_process(){
	TCCR0B |= MASK_TMPS;
}

inline void _stop_timer_process(){
	TCCR0B &= ~MASK_TMPS;
}

inline void _reset_timer_process(){
	TCNT0=0x00;
}

#ifdef DEBUG
void printProcess(Process p){
	printf("PID:\t%hd\nFunc:\t%04X\nstato:\t%d\n", p.PID, p.func_addr, p.stato);
}
#endif
