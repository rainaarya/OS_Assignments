;----------------------------------------------------------------------------
; 64-bit helloworld in assembly, using printf() from C
;
; Equivalent to:
;       #include <stdio.h>
;       char msg[] = "Hello, world";
;       int main() {
;               printf("%s\n", msg);
;               return 0;
;       }
;
; Demonstrates basic text output using C printf(), in 64-bit mode.
;----------------------------------------
; Assemble in 64-bit:   nasm  -f elf64 -o hp64.o -l hp64.lst  hello-printf-64.asm
;
; Link:         ld hp64.o  -o hp64  -lc  --dynamic-linker /lib/ld-2.7.so
;   or maybe    ld hp64.o  -o hp64  -lc  --dynamic-linker /lib/ld-linux-x86-64.so.2
;       (the "-lc" option is needed to resolve "printf")
;
;   or else     gcc -o hp64  hp64.o
;       (but remove the "_start" label in this case, or gcc will complain)
;
; run:          ./hp64 
; output is:    Hello world,
;               ...and goodbye!
;----------------------------------------
; 2009-12-10 -bob,mon.
;----------------------------------------------------------------------------
        SECTION .data
msg:    db "Hello, world,", 0
msg2:   db "...and goodbye!", 0
fmt:    db "%s", 10, 0

        SECTION .text
        extern printf
        global main
main:
        mov rax, 0x1234567812345678
        xor ax, 0x11
        mov rdi, ax
        call printf
        xor rax, 0x11
        mov rdi, rax
        call printf
;----------------------------------------------------------------------------