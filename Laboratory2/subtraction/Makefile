all: subtraction

subtraction: subtraction.o
		ld -m elf_i386 subtraction.o -o subtraction

subtraction.o: subtraction.s
		as --gstabs -g --32 subtraction.s -o subtraction.o