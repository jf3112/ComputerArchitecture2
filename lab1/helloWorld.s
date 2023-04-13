.section .data

SYSEXIT = 1
SYSCALL = 0x80
EXIT_SUCCESS = 0
SYSWRITE = 4
STDOUT = 1

.globl _start

message: .ascii "Hello world!\n"
message_len = . - message

.section .text

_start:

movl $SYSWRITE, %eax
movl $STDOUT, %ebx
movl $message, %ecx
movl $message_len, %edx
int $SYSCALL

movl $SYSEXIT, %eax
movl $EXIT_SUCCESS, %ebx
int $SYSCALL