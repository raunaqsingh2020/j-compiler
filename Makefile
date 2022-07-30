token.o: token.c token.h
	clang -Wall -g -c token.c

stack.o: stack.c stack.h
	clang -Wall -g -c stack.c

jc.o: jc.c
	clang -Wall -g -c jc.c

jc: jc.o stack.o token.o
	clang -Wall -g -o jc jc.o stack.o token.o

clean:
	rm jc *.o