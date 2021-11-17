; ----------------------------------------------------------------------------------------
; Writes "Hola, mundo" to the console using a C library. Runs on Linux.
;
;     nasm -felf64 hola.asm && gcc hola.o && ./a.out
; ----------------------------------------------------------------------------------------


extern printf

section .data
		format: db "%llx",0

section .text
	global main
	

main:                                       ; This is called by the C library startup code
	push rbp
    mov rax, 0x1234567812345678
	xor ax, 0x11
	mov rdi, format
	mov rsi, rax
	call printf
	xor rax, 0x11
	mov rdi, format
	mov rsi, rax
	call printf
	pop rbp

