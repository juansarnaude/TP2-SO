GLOBAL _defaultExit

section .text

_defaultExit:
    mov rdi, rax
    mov rax, 60
    int 80h
