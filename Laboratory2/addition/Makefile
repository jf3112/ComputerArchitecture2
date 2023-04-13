all: addition

addition: addition.o
		ld -m elf_i386 addition.o -o addition

addition.o: addition.s
		as --gstabs -g --32 addition.s -o addition.o