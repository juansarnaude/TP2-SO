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
GLOBAL sys_waitpid
GLOBAL sys_kill
GLOBAL sys_block
GLOBAL sys_unblock
GLOBAL sem_open
GLOBAL sem_close
GLOBAL sem_post
GLOBAL sem_wait
GLOBAL sys_yieldProcess
GLOBAL sys_nice
GLOBAL sys_pipe
GLOBAL sys_dup2
GLOBAL sys_open
GLOBAL sys_close
GLOBAL sys_ps
GLOBAL sys_changeProcessStatus
GLOBAL sys_getCurrentPid
GLOBAL sys_secondsElapsed

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

sys_waitpid:
    push rbp
    mov rbp,rsp
    mov rax,10
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_kill:
    push rbp
    mov rbp,rsp
    mov rax,11
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_block:
    push rbp
    mov rbp,rsp
    mov rax,12
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_unblock:
    push rbp
    mov rbp,rsp
    mov rax,13
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sem_open:
    push rbp
    mov rbp,rsp
    mov rax,14
    int 0x80
    mov rsp,rbp
    pop rbp
    ret


sem_close:
    push rbp
    mov rbp,rsp
    mov rax,15
    int 0x80
    mov rsp,rbp
    pop rbp
    ret


sem_post:
    push rbp
    mov rbp,rsp
    mov rax,16
    int 0x80
    mov rsp,rbp
    pop rbp
    ret


sem_wait:
    push rbp
    mov rbp,rsp
    mov rax,17
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_yieldProcess:
    push rbp
    mov rbp,rsp
    mov rax,18
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_nice:
    push rbp
    mov rbp,rsp
    mov rax,19
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_pipe:
    push rbp
    mov rbp,rsp
    mov rax,20
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_dup2:
    push rbp
    mov rbp,rsp
    mov rax,21
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_open:
    push rbp
    mov rbp,rsp
    mov rax,22
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_close:
    push rbp
    mov rbp,rsp
    mov rax,23
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_ps:
    push rbp
    mov rbp,rsp
    mov rax,24
    int 0x80
    mov rsp,rbp
    pop rbp
    ret    

sys_changeProcessStatus:
    push rbp
    mov rbp,rsp
    mov rax,25
    int 0x80
    mov rsp,rbp
    pop rbp
    ret     

sys_getCurrentPid:
    push rbp
    mov rbp,rsp
    mov rax,26
    int 0x80
    mov rsp,rbp
    pop rbp
    ret  

sys_secondsElapsed:
    push rbp
    mov rbp,rsp
    mov rax,27
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
