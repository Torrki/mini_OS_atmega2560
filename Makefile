BINS=OS.elf 

# put here the additional .o files you want to generate
# one .c file for each .o should be present
OBJS=avr_common/uart.o syscall.o process.o core.o atom_avr.o

# put here the additional header files needed for compilation
HEADERS=avr_common/uart.h syscall.h process.h core.h

include avr_common/avr.mk	