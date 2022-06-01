GLOBAL sys_write
GLOBAL sys_read
GLOBAL sys_exit
GLOBAL sys_execve
GLOBAL sys_time
GLOBAL getRegisters

section .text
sys_write:
    push rbp
    mov rbp, rsp
    mov rax, 1
    int 0x80
    mov rsp, rbp
    pop rbp
    ret

sys_read:
    push rbp
    mov rbp, rsp
    mov rax, 0
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_exit:
    push rbp
    mov rbp,rsp
    mov rax,2
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_execve:
    push rbp
    mov rbp,rsp
    mov rax,3
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_time:
    push rbp
    mov rbp,rsp
    mov rax,100
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

getRegisters:
    push rbp
    mov rbp, rsp

    mov [rdi], rax
    mov [rdi+8], rbx
    mov [rdi+8*2], rcx
    mov [rdi+8*3], rdx
    mov [rdi+8*4], rsi
    mov [rdi+8*5], rdi
    mov [rdi+8*6], rbp
    mov [rdi+8*7], rsp
    mov [rdi+8*8], r8
    mov [rdi+8*9], r9
    mov [rdi+8*10], r10
    mov [rdi+8*11], r11
    mov [rdi+8*12], r12
    mov [rdi+8*13], r13
    mov [rdi+8*14], r14
    mov [rdi+8*15], r15

    mov rsp, rbp
    pop rbp
    ret
