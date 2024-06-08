CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c11 -D_XOPEN_SOURCE=700
.PHONY = all clean

all: clash

clash: clash.o plist.o plist.h
	$(CC) $(CFLAGS) -o clash clash.o plist.o

plist.o: plist.c plist.h
	$(CC) $(CFLAGS) -c plist.c

clash.o: clash.c plist.h
	$(CC) $(CFLAGS) -c clash.c

clean: 
	rm -f clash 
