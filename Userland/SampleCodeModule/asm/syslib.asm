GLOBAL sys_write
GLOBAL sys_read
GLOBAL sys_getregs
GLOBAL sys_exit
GLOBAL sys_exec
GLOBAL sys_time
GLOBAL sys_copymem
GLOBAL inv_opcode
GLOBAL div_zero
GLOBAL sys_memInfo
GLOBAL sys_memMalloc
GLOBAL sys_memFree

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

sys_getregs:
    push rbp
    mov rbp, rsp
    mov rax, 2
    int 0x80
    mov rsp, rbp
    pop rbp
    ret

sys_exit:
    push rbp
    mov rbp,rsp
    mov rax,4
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_exec:
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
    mov rax,5
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_copymem:
    push rbp
    mov rbp,rsp
    mov rax,6
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_memInfo:
    push rbp
    mov rbp,rsp
    mov rax,7
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_memMalloc:
    push rbp
    mov rbp,rsp
    mov rax,8
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_memFree:
    push rbp
    mov rbp,rsp
    mov rax,9
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

inv_opcode:
    ud2
    ret

div_zero:
    mov ecx, 0
    div ecx
    ret
