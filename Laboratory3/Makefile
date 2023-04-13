all: float_exception_calculator

float_exception_calculator.o: float_exception_calculator.s
	as --gstabs -g --32 float_exception_calculator.s -o float_exception_calculator.o

float_exception_calculator: float_exception_calculator.o
	ld -m elf_i386 float_exception_calculator.o -o float_exception_calculator	