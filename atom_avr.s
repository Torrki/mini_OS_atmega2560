#include <avr/io.h>

.section .text

.global _get_return
_get_return:
//r4:r25 indirizzo address
	push r28
	push r29
	push r30
	push r31
	push r17
	
	mov r28,r24		//addr in Y
	mov r29,r25
	in r30,0x3d		//stack in Z
	in r31,0x3e
	ldd r17,Z+8		//scrittura in addr
	st Y,r17
	ldd r17,Z+7
	std Y+1,r17
	ldd r17,Z+6
	std Y+2,r17
	ldi r17,0x00
	std Y+3,r17
	
	pop r17
	pop r31
	pop r30
	pop r29
	pop r28
	ret

.global _context_switch
_context_switch:
//r22:r23 indirizzo new
//r24:r25 indirizzo old
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r28
	push r29
	push r30
	push r31
	
	in r30,0x3d 	//stack in Z
	in r31,0x3e
	in r27,0x3f 	//sreg
	mov r28,r24 	//old in Y
	mov r29,r25
	st Y,r30			//modifico old
	std Y+1,r31
	std Y+2,r27
	mov r28,r22 	//new in Y
	mov r29,r23
	ld r30,Y			//sp di new
	ldd r31,Y+1
	ldd r27,Y+2		//sreg di new
	out 0x3d,r30 	//nuovo stack
	out 0x3e,r31
	out 0x3f,r27	//sreg di new
	
	pop r31
	pop r30
	pop r29
	pop r28
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop r7
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	pop r0
	ret
	
.global _prepare_process
_prepare_process:
//r24:r25 indirizzo processo
/*indirizzo funzione*/
	push r28
	push r28
	push r28
/*spazio di lavoro*/
	push r28
	push r29
	push r16
	push r17
	push r18
	
	in r30,0x3d 		//stack in Z
	in r31,0x3e
	mov r28,r24 		//processo in Y
	mov r29,r25
	ld r16,Y				//indirizzo funzione
	ldd r17,Y+1
	ldi r18,0x00
	std Z+6,r18 		//inserimento miccia
	std Z+7,r17
	std Z+8,r16
	adiw r30,0x05
	sbiw r30,0x20
	std Y+3,r30 		//partenza sp del processo
	std Y+4,r31
	adiw r30,0x20
	
	pop r18
	pop r17
	pop r16
	pop r29
	pop r28
	adiw r30,0x03 	//salto indirizzo funzione
	out 0x3d,r30
	out 0x3e,r31
	ret

