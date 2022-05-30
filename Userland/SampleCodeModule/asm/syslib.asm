GLOBAL sys_write
GLOBAL sys_read
GLOBAL sys_exit
GLOBAL sys_execve
GLOBAL sys_time

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