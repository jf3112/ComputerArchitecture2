.section .data
SYSEXIT = 1
SYSCALL = 0x80
EXIT_SUCCESS = 0
SYSWRITE = 4
STDOUT = 1
SYSREAD = 3
STDIN = 0

number0_f: .float 0.0
number1_f: .float 1.5
number2_f: .float -1.5
number3_f: .float 10.999999
number4_f: .float -5.9999999

nearest: 
    .short 0x000 # round control: 00

down: 
    .short 0x400 # round control: 01

up: 
    .short 0x800 # round control: 10

truncation: 
    .short 0xC00 # round control: 11

single_precision: 
    .short 0x000 # precision control: 00

double_precision: 
    .short 0x200 # precision control: 10

control_word: 
    .short 0x103F 

message1: 
    .ascii "0. Nearest rounding, 1. Rounding down, 2. Rounding up, 3. Rounding by truncation:"
    message1_len = . - message1

message2: 
    .ascii "0. Single precision, 1. Double precision"
    message2_len= . - message2

buffer: 
    .int 0
    buffer_len = . - buffer

.section .text
.globl _start

_start:

mov $SYSWRITE, %eax
mov $STDOUT, %ebx
mov $message1, %ecx
mov $message1_len, %edx
int $0x80

mov $SYSREAD, %eax
mov $STDIN, %ebx
mov $buffer, %ecx
mov $buffer_len, %edx
int $0x80

mov buffer(,%edi,1), %al

cmp $'0', %al
je round_nearest
cmp $'1', %al
je roung_down
cmp $'2', %al
je round_up
cmp $'3', %al
je round_truncation

round_nearest:
    
    mov nearest, %eax
    jmp precision

roung_down:

    mov down, %eax
    jmp precision

round_up:

    mov up, %eax
    jmp precision

round_truncation:

    mov truncation, %eax
    jmp precision

precision:

    mov $SYSWRITE, %eax
    mov $STDOUT, %ebx
    mov $message2, %ecx
    mov $message2_len, %edx
    int $0x80

    mov $SYSREAD, %eax
    mov $STDIN, %ebx
    mov $buffer, %ecx
    mov $buffer_len, %edx
    int $0x80

    mov buffer(,%edi,1), %al

    cmp $'0', %al
    je single_precision
    cmp $'1', %al
    je double_precision

single_precision:

    mov single_precision, %ebx 
    jmp word

double_precision:

    mov double_precision, %ebx 

word:

    mov control_word, %ecx
    add %eax, %ebx 
    add %ebx, %ecx 
    mov %ecx, control_word 
    fldcw control_word 

#exception + 0 / number = + 0
fldz
fdiv number1_f

#exception - 0 / number = - 0
fldz
fdiv number2_f

#exception NaN 0 / 0 = NaN
fldz
fdiv number0_f

#exception + number / 0 = + inf
flds number1_f
fdiv number0_f

#exception - number / 0 = - inf
flds number2_f
fdiv number0_f

#adding numbers
flds number3_f
flds number4_f
faddp

#subtracting numbers
flds number3_f
flds number4_f
fsubp

exit:
mov $SYSEXIT, %eax
mov $EXIT_SUCCESS, %ebx
int $SYSCALL
