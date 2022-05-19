GLOBAL cpuVendor
GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHours
<<<<<<< HEAD
GLOBAL read_port
=======
GLOBAL getDay
GLOBAL getMonth
GLOBAL getYear
>>>>>>> 34ed40acc7d813660a1cc199d793757afa898ecf
section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

setBinaryBitRTC:
	push rbp
	mov rbp,rsp
	mov al,0xB
	out 70h,al
	in al,71h
	OR al,0b00000100; dejo en 1 el bit 2 del registro que es el que quiro cambiar 
	out 71h,al
	mov rsp,rbp
	pop rbp
	ret

getSeconds:
    push rbp
    mov rbp,rsp
    
	call setBinaryBitRTC
    mov al,0
    out 70h,al
    in al,71h
    mov rsp,rbp
    pop rbp
    ret

getMinutes:
    push rbp
    mov rbp,rsp
	call setBinaryBitRTC
    mov al,2
    out 70h,al
    in al,71h
    mov rsp,rbp
    pop rbp
    ret

getHours:
    push rbp
    mov rbp,rsp
	call setBinaryBitRTC
    mov al,04
    out 70h,al
    in al,71h
    mov rsp,rbp
    pop rbp
    ret	

<<<<<<< HEAD
read_port:
    push rbp
    mov rbp, rsp
    
    mov dx, di
    in al, dx

    mov rsp, rbp
    pop rbp 
    ret
=======
getDay:
    push rbp
    mov rbp,rsp
	call setBinaryBitRTC
    mov al,07
    out 70h,al
    in al,71h
    mov rsp,rbp
    pop rbp
    ret		

getMonth:
    push rbp
    mov rbp,rsp
	call setBinaryBitRTC
    mov al,08
    out 70h,al
    in al,71h
    mov rsp,rbp
    pop rbp
    ret	

getYear:
	push rbp
    mov rbp,rsp
	call setBinaryBitRTC
    mov al,09
    out 70h,al
    in al,71h
    mov rsp,rbp
    pop rbp
    ret	
>>>>>>> 34ed40acc7d813660a1cc199d793757afa898ecf
