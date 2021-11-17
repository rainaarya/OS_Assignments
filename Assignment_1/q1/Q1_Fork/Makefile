all:
	gcc -E q1fork.c -o q1fork.i
	gcc -S q1fork.i -o q1fork.s
	gcc -c q1fork.s -o q1fork.o
	gcc q1fork.o -o q1fork
	./q1fork
preprocess:
	gcc -E q1fork.c -o q1fork.i
compile:
	gcc -S q1fork.i -o q1fork.s
assemble:
	gcc -c q1fork.s -o q1fork.o
link:
	gcc q1fork.o -o q1fork
run:
	./q1fork
clean:
	rm -f q1fork.i q1fork.s q1fork.o q1fork