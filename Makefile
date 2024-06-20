BINS=Esempio1.elf 

# put here the additional .o files you want to generate
# one .c file for each .o should be present
OBJS=avr_common/uart.o memory.o OS.o process.o kernel.o simple_scheduler.o atom_avr.o syscall.o

# put here the additional header files needed for compilation
HEADERS=avr_common/uart.h memory.h OS_types.h OS.h kernel.h process.h simple_scheduler.h syscall.h

include avr_common/avr.mk	
