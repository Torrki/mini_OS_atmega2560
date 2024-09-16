#include "uart.h"
// ********************************************************************************
// Includes
// ********************************************************************************
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include "../kernel.h"
// ********************************************************************************
// Macros and Defines
// ********************************************************************************
#define BAUD 19600
#define MYUBRR F_CPU/16/BAUD-1
// ********************************************************************************
// Function Prototypes
// ********************************************************************************
void usart_init(uint16_t ubrr);
char usart_getchar( void );
void usart_putchar( char data );
void usart_pstr (char *s);
unsigned char usart_kbhit(void);
int usart_putchar_printf(char var, FILE *stream);


static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);
extern struct _kernel_structure kernel;
char bufferRX[MAX_PROC];
// ********************************************************************************
// usart Related
// ********************************************************************************

ISR(USART0_TX_vect){
	pid_t procTX=kernel.startTX;
	if(procTX != CODA && kernel.ProcList[procTX].PID != -1){
		_wake_process(procTX);
		kernel.startTX=kernel.arrayListTX[procTX];
		kernel.arrayListTX[procTX]=BAD_ALLOC;
	}
}

ISR(USART0_RX_vect){
	pid_t procRX=kernel.startRX;
	if(procRX != CODA && kernel.ProcList[procRX].PID != -1){
		_wake_process(procRX);
		kernel.startRX=kernel.arrayListRX[procRX];
		kernel.arrayListRX[procRX]=BAD_ALLOC;
		
		bufferRX[procRX]=UDR0;
	}
}

void usart_init( uint16_t ubrr) {
    // Set baud rate
    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(TXCIE0) | _BV(RXCIE0);   /* Enable RX and TX */  
}
void usart_putchar(char data) {
    // Wait for empty transmit buffer
    while ( !(UCSR0A & (_BV(UDRE0))) );
    // Start transmission
    UDR0 = data; 
}
char usart_getchar(void) {
    // Wait for incoming data
    while ( !(UCSR0A & (_BV(RXC0))) );
    // Return the data
    return UDR0;
}
unsigned char usart_kbhit(void) {
    //return nonzero if char waiting polled version
    unsigned char b;
    b=0;
    if(UCSR0A & (1<<RXC0)) b=1;
    return b;
}
void usart_pstr(char *s) {
    // loop through entire string
    while (*s) { 
        usart_putchar(*s);
        s++;
    }
}
 
// this function is called by printf as a stream handler
int usart_putchar_printf(char var, FILE *stream) {
    // translate \n to \r for br@y++ terminal
    if (var == '\n') usart_putchar('\r');
    usart_putchar(var);
    return 0;
}

void putChar(char c){
	int i=0;
	while ( !(UCSR0A & (_BV(UDRE0))) && i < 1) i++;		//Serve interrogare più volte il registro per avere dei risultati stabili
	if(i==1){	//buffer ancora pieno
		pid_t cp=_get_current_pid();
		//bufferTX[cp]=c;
		if(kernel.startTX==CODA){
			kernel.startTX=cp;
			kernel.arrayListTX[cp]=CODA;
		}else{
			pid_t tmp = kernel.startTX;
			while(kernel.arrayListTX[tmp] != CODA) tmp=kernel.arrayListTX[tmp];
			kernel.arrayListTX[tmp]=cp;
			kernel.arrayListTX[cp]=CODA;
		}
		//printf("sleep: %hd\n", cp);
		sleep(cp);
	}
	UDR0 = c;
}

char getChar(){
	int i=0;
	while( !(UCSR0A & (_BV(RXC0))) && i < 1 ) i++;
	if(i == 1){		//dato ancora non presente
		pid_t cp=_get_current_pid();
		if(kernel.startRX==CODA){
			kernel.startRX=cp;
			kernel.arrayListRX[cp]=CODA;
		}else{
			pid_t tmp = kernel.startRX;
			while(kernel.arrayListRX[tmp] != CODA) tmp=kernel.arrayListRX[tmp];
			kernel.arrayListRX[tmp]=cp;
			kernel.arrayListRX[cp]=CODA;
		}
		//printf("sleep: %hd\n", cp);
		sleep(cp);
		return bufferRX[cp];
	}
	else return UDR0;
}

void printf_init(void){
  stdout = &mystdout;
  
  // fire up the usart
  usart_init ( MYUBRR );
}
