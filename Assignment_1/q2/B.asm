extern C

section .bss
	base resb 100	

section .data
	text db "Currently inside Function B, the individual characters are below:",10,0

section .text
	global B	

B:
	push rbp        ; save previous stackbase-pointer register
	mov rbp, rsp   ; rbp = rsp
	push rdi

	mov rax, 1
	mov rdi, 1
	mov rsi, text
	mov rdx, 67
	syscall

	pop rdi

	mov rax, rdi ; the argument recieved from function A
	
	call _printASCII

	leave

	mov rax, C
	mov [rbp+8], rax ; move the address of C() where current return address is stored
	ret

	
_printASCII:
	mov r9, base
	mov rbx, 10
	mov [r9], rbx
	inc r9	

_storeASCII:
	xor rdx, rdx
	mov rbx, 256	; since 256 = 2^8 (8 bits)
	div rbx
	
	mov [r9], dl ; lower 8 bits
	inc r9
	
	cmp rax, 0
	jne _storeASCII
	
_reverseASCII:
	dec r9
	
	mov rax, 1
	mov rdi, 1
	mov rsi, r9
	mov rdx, 1
	syscall

	cmp r9, base
	jge _reverseASCII

	ret