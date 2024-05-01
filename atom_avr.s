#include <avr/io.h>

.section .text

.global _end_switch
_end_switch:
//r24:r25 indirizzo new
	mov r28,r24 	//new in Y
	mov r29,r25
	ld r30,Y+			//sp di new
	ld r31,Y
	out 0x3d,r30 	//nuovo stack
	out 0x3e,r31
	
	pop r2
	out 0x3c,r2
	pop r2
	out 0x3b,r2
	pop r2
	out 0x3f,r2
//	pop r31
//	pop r30
//	pop r29
//	pop r28
//	pop r27
//	pop r26
//	pop r25
//	pop r24
//	pop r23
//	pop r22
//	pop r21
//	pop r20
//	pop r19
//	pop r18
//	pop r17
//	pop r16
//	pop r15
//	pop r14
//	pop r13
//	pop r12
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
//	pop r1
//	pop r0
	ret

.global _end_first_switch
_end_first_switch:
//r24:r25 indirizzo new
	mov r28,r24 	//new in Y
	mov r29,r25
	ld r30,Y+			//sp di new
	ld r31,Y
	out 0x3d,r30 	//nuovo stack
	out 0x3e,r31
	
	reti

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

.global _first_switch
_first_switch:
//r22:r23 indirizzo new
//r24:r25 indirizzo old
//push ISR: r1,r0,r0,r0,r12-r27,r30,r31,r28,r29,r1
/*memorizzo general purpuse*/
//	push r0
//	push r1
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
//	push r12
//	push r13
//	push r14
//	push r15
//	push r16
//	push r17
//	push r18
//	push r19
//	push r20
//	push r21
//	push r22
//	push r23
//	push r24
//	push r25
//	push r26
//	push r27
//	push r28
//	push r29
//	push r30
//	push r31
/*memorizzo sreg, rampz, eind*/
	in r2,0x3f
	push r2
	in r2,0x3b
	push r2
	in r2,0x3c
	push r2
	
	in r30,0x3d 	//stack in Z
	in r31,0x3e
	mov r28,r24 	//old in Y
	mov r29,r25
	st Y+,r30			//modifico old
	st Y,r31
	mov r28,r22 	//new in Y
	mov r29,r23
	ld r30,Y+			//sp di new
	ld r31,Y
	out 0x3d,r30 	//nuovo stack
	out 0x3e,r31
	
	reti

.global _context_switch
_context_switch:
//r22:r23 indirizzo new
//r24:r25 indirizzo old
//push ISR: r1,r0,r0,r0,r14-r27,r30,r31,r28,r29,r1
/*memorizzo general purpuse*/
//	push r0
//	push r1
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
//	push r12
//	push r13
//	push r14
//	push r15
//	push r16
//	push r17
//	push r18
//	push r19
//	push r20
//	push r21
//	push r22
//	push r23
//	push r24
//	push r25
//	push r26
//	push r27
//	push r28
//	push r29
//	push r30
//	push r31
/*memorizzo sreg, rampz, eind*/
	in r2,0x3f
	push r2
	in r2,0x3b
	push r2
	in r2,0x3c
	push r2
	
	in r30,0x3d 	//stack in Z
	in r31,0x3e
	mov r28,r24 	//old in Y
	mov r29,r25
	st Y+,r30			//modifico old
	st Y,r31
	mov r28,r22 	//new in Y
	mov r29,r23
	ld r30,Y+			//sp di new
	ld r31,Y
	out 0x3d,r30 	//nuovo stack
	out 0x3e,r31
	
	pop r2
	out 0x3c,r2
	pop r2
	out 0x3b,r2
	pop r2
	out 0x3f,r2
//	pop r31
//	pop r30
//	pop r29
//	pop r28
//	pop r27
//	pop r26
//	pop r25
//	pop r24
//	pop r23
//	pop r22
//	pop r21
//	pop r20
//	pop r19
//	pop r18
//	pop r17
//	pop r16
//	pop r15
//	pop r14
//	pop r13
//	pop r12
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
//	pop r1
//	pop r0
	ret
	
