section .data
message:
    db 'Hello, world', 10

section .text
global _start

_start:
    mov rax, 1
    mov rdi, 2
    mov rsi, message
    mov rdx, 12
    syscall

    mov rax, 60
    xor rdi, rdi
    syscall