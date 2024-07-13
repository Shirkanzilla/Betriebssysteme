CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c11 -D_XOPEN_SOURCE=700
.PHONY = test test-ref clean

test: test.o halde.o
	$(CC) $(CFLAGS) -o test test.o halde.o

halde.o: halde.c
	$(CC) $(CFLAGS) -c halde.c

test-ref: test.o halde-ref.o
	$(CC) $(CFLAGS) -o test-ref test.o halde-ref.o

test.o:	test.c halde.h
	$(CC) $(CFLAGS) -c test.c

clean:
	rm -f test
	rm -f halde.o
	rm -f test-ref
	rm -f test.o

