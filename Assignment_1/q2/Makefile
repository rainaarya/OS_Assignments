all:
	@nasm -f elf64 B.asm
	@gcc A.c B.o C.c -o executable -no-pie
	@./executable
clean:
	rm -f B.o executable