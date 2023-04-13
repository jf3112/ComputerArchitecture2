all: dodawanie

dodawanie.o: dodawanie.s
	as -g --32 dodawanie.s -o dodawanie.o

dodawanie: dodawanie.o
	ld -m elf_i386 dodawanie.o -o dodawanie		