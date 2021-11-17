all:
	gcc -E q1pthread.c -o q1pthread.i
	gcc -S q1pthread.i -o q1pthread.s
	gcc -c q1pthread.s -o q1pthread.o
	gcc -pthread q1pthread.o -o q1pthread
	./q1pthread
preprocess:
	gcc -E q1pthread.c -o q1pthread.i
compile:
	gcc -S q1pthread.i -o q1pthread.s
assemble:
	gcc -c q1pthread.s -o q1pthread.o
link:
	gcc -pthread q1pthread.o -o q1pthread
run:
	./q1pthread
clean:
	rm -f q1pthread.i q1pthread.s q1pthread.o q1pthread