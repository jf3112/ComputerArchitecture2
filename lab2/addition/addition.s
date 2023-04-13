.code32
.section .data

SYSEXIT = 1
SYSCALL = 0x80
EXIT_SUCCESS = 0

# initializing data
number1:
   .long 0x10304000, 0x30000003, 0x20030002, 0x100F0001
   number1_len = (. - number1)
   number1_size = number1_len / 4

number2:
   .long 0xF0000000, 0x10007000, 0x10000000, 0xF000000F
   number2_len = (. - number2)
   number2_size = number2_len / 4

.section .bss

carry:
   .space number1_size
   
wynik:
   .space number1_len 

.section .text
.globl _start
_start:

# clearing carry flag
clc
pushf

# loop counter
movl $number1_size, %ebx

# the numbers have the same size

adding:

    popf
    dec %ebx
    movl number1(, %ebx, 4), %eax
    adcl number2(, %ebx, 4), %eax
    pushf
    movl %eax, result(, %ebx, 4)
    cmp $0, %ebx
    jz end
    jmp adding

end:
    popf
    movl $0, carry

    jc adding_carry
    jmp exit

adding_carry:
    movl $1, carry

exit:

movl $SYSEXIT, %eax
movl $EXIT_SUCCESS, %ebx
int $SYSCALL
