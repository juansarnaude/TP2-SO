
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _endhaltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

EXTERN irqDispatcher
EXTERN contextSwitch

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	mov rsi, rsp	; pointer a backup registros
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro


%macro exceptionHandler 1
	pushState

	;rdi, rsi, rdx, rcx, r8 y r9
	mov rdi, %1 ; pasaje de parametro
	mov rsi, [rsp+15*8]	; Position of the original RIP in the stack
	mov rdx, [rsp+18*8]	; Position of the original RSP in the stack
	mov rcx, rsp
	;Should we call haltcpu? _hlt? go to the return of the main function?
	;Should we call or modify the RIP value in the stack?
	call haltcpu

	popState
	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out		0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
    pushState

    mov rdi,0
    mov rsi,rsp
    call irqDispatcher
    
    mov rdi, rsp
    call contextSwitch
    mov rsp, rax

    ; signal pic EOI (End of Interrupt)
    mov al, 20h
    out 20h, al

    popState
    iretq

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

haltcpu:
	sti
	hlt
_endhaltcpu:
	jmp haltcpu

