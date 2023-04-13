.code32
.section .data

SYSEXIT = 1
SYSCALL = 0x80
EXIT_SUCCESS = 0

number1:
    .long 0x00000000, 0x0000000F, 0x10000000, 0x00000011
    number1_len =(.-number1)
    number1_size = number1_len / 4
number2:
    .long 0xF0000, 0x20000000, 0x00000002
    number2_len =(.-number2)
    number2_size = number2_len / 4

.section .bss

carry:
    .space number1_size

result:
    .space number1_len 

.section .text
.globl _start
_start:

clc
pushf

movl $number1_size, %ebx
movl $number2_size, %edi

subtraction:

    popf
    dec %ebx
    dec %edi
    movl number1(, %ebx, 4), %eax
    sbbl number2(, %edi, 4), %eax
    pushf
    movl %eax, result(, %ebx, 4)
    cmp $0, %edi
    jz subtraction_edi
    cmp $0, %ebx
    jz end
    jmp subtraction

subtraction_edi:
    cmp $0, %ebx
    jz end
    dec %ebx
    popf
    movl number1(, %ebx, 4), %eax
    sbbl $0, %eax
    pushf
    movl %eax, result(, %ebx, 4)
    jmp subtraction_edi

end:
    popf
    movl $0, carry  

    jc adding_carry
    jmp exit

adding_carry:
    movl $1, carry 

exit:

mov $SYSEXIT, %eax
mov $EXIT_SUCCESS, %ebx
int $SYSCALL
