#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "halde.h"

int main(int argc, char *argv[]) {
	printList();

	char *m1 = malloc(200*1024);
	printList();

	char *m2 = malloc(10);
	printList();

	char *m3 = malloc(1);
	printList();

	char *m4 = malloc(500*1024);
	printList();
	
	free(m1);
	printList();

	free(m2);
	printList();

	free(m3);
	printList();
	
	free(m4);
	printList();
	
	m1 = malloc(1);
	printList();

	//Testfall: Leeren Speicherblock anfordern
	m2 = malloc(0);
	printList();

	m3 = malloc(1337);
	printList();

	m4 = malloc(5000);
	printList();

	free(m1);
	printList();

	free(m2);
	printList();

	free(m3);
	printList();
	
	free(m4);
	printList();

	exit(EXIT_SUCCESS);
}
