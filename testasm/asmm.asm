section .bss
	base resb 100	

section .data
	text db "Currently inside Function B, the individual characters are below:",10,0

section .text
	global asmm	

asmm:
	push rbp        ; save previous stackbase-pointer register
	mov rbp, rsp   ; rbp = rsp

	rdrand eax   ; get random number

    mov rsp, rbp    ; restore stackbase-pointer register
    pop rbp         ; restore previous stackbase-pointer register
	ret

	
