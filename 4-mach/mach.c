#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include "sem.h"
#include "queue.h"
#include "run.h"

static SEM *print_signal;
static SEM *group_mutex;

static void die(const char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}

static int parse_positive_int_or_die(char *str) {
    errno = 0;
    char *endptr;
    long x = strtol(str, &endptr, 10);
    if (errno != 0) {
        die("invalid number");
    }
    // Non empty string was fully parsed
    if (str == endptr || *endptr != '\0') {
        fprintf(stderr, "invalid number\n");
        exit(EXIT_FAILURE);
    }
    if (x <= 0) {
        fprintf(stderr, "number not positive\n");
        exit(EXIT_FAILURE);
    }
    if (x > INT_MAX) {
        fprintf(stderr, "number too large\n");
        exit(EXIT_FAILURE);
    }
    return (int)x;
}

int main(int argc, char **argv) {
    // TODO: implement me
	if (argc < 3) {
    		fprintf(stderr, "Usage: %s <number of threads> <mach-file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int nthreads = parse_positive_int_or_die(argv[1]);
	if (nthreads == 0) return 1;
	print_signal = semCreate(0);
	if (print_signal == NULL) die("Creating Semaphore failed\n");
	group_mutex = semCreate(1);
	if (group_mutex == NULL) die("Creating Semaphore failed\n");

	FILE *machfile = fopen(argv[2], "r");
	if (machfile == NULL) die("file not found");
	
	char command[4096];
	P(group_mutex);
	while(1){
		if (fgets(command, sizeof(command), machfile) == NULL){
			if (feof(machfile)) break;
			die("error reading next line");
		}
		if (command[0] == '\n'){
		       	//TODO Neue Threadgruppe;
		}
		else{
			//TODO commands in threads ausführen
			//pthread_create(
		       	fputs(command, stdout);
		}
	}
	
	fflush(stdout);
	fclose(machfile);
	return 0;
}
