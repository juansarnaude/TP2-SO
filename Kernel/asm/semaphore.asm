global block
global unblock

block:
    mov rax, 0
    mov al, 1
  
    xchg al, [rdi]
    cmp al, 0
    jne block

    ret

unblock:
    mov byte [rdi], 0
    ret