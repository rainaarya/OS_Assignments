all:
	gcc E1.c -o E1
	gcc E2.c -o E2
	gcc main.c -o main
	./main

E1: E1.c
	gcc E1.c -o E1
E2: E2.c
	gcc E2.c -o E2
main: main.c
	gcc main.c -o main

kill:	
	killall -9 E1 E2 main
clean:
	rm -f E1 E2 main
