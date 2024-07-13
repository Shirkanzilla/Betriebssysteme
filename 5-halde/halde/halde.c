#include "halde.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/// Magic value for occupied memory chunks.
#define MAGIC ((void*)0xbaadf00d)

/// Size of the heap (in bytes).
#define SIZE (1024*1024*1)

/// Memory-chunk structure.
struct mblock {
	struct mblock *next;
	size_t size;
	char memory[];
};

/// Heap-memory area.
static char memory[SIZE];

/// Pointer to the first element of the free-memory list.
static struct mblock *head;

/// Helper function to visualise the current state of the free-memory list.
void printList(void) {
	struct mblock *lauf = head;

	// Empty list
	if (head == NULL) {
		char empty[] = "(empty)\n";
		write(STDERR_FILENO, empty, sizeof(empty));
		return;
	}

	// Print each element in the list
	const char fmt_init[] = "(off: %7zu, size:: %7zu)";
	const char fmt_next[] = " --> (off: %7zu, size:: %7zu)";
	const char * fmt = fmt_init;
	char buffer[sizeof(fmt_next) + 2 * 7];

	while (lauf) {
		size_t n = snprintf(buffer, sizeof(buffer), fmt
			, (uintptr_t) lauf - (uintptr_t)memory, lauf->size);
		if (n) {
			write(STDERR_FILENO, buffer, n);
		}

		lauf = lauf->next;
		fmt = fmt_next;
	}
	write(STDERR_FILENO, "\n", 1);
}

void *malloc (size_t size) {
	if (size == 0) return NULL;
	if (head == NULL){
		head = (struct mblock *)memory;
		head->next = NULL;
		head->size = SIZE - sizeof(struct mblock);	
	}
	struct mblock *prev = head;
	struct mblock *current = head;
	while (current != NULL){
		if (current->size < size){
			prev = current;
		       	current = current->next;
		}
		else break;
	}
	if (current == NULL && head != NULL){
	       	errno = ENOMEM;
		return NULL;
	}
	if (current->size > size + sizeof(struct mblock)){
		struct mblock *mblockNew;
		current->size -= (size + sizeof(struct mblock));
		mblockNew = (struct mblock *)(((unsigned char*)current) + sizeof(struct mblock) + current->size);
		mblockNew->size = size;
		mblockNew->next = MAGIC;
		return (void *)((unsigned char*)mblockNew + sizeof(struct mblock));
	}
	else{
		prev->next = current->next;
		current->next = MAGIC;
		return (void *)((unsigned char*)current + sizeof(struct mblock));
	}
}

void free (void *ptr) {
	if (ptr == NULL) return;
	struct mblock *delElement = (struct mblock *)((unsigned char*)ptr - sizeof(struct mblock));
	if (delElement == NULL) return;
	if (delElement->next != MAGIC) abort();
	delElement->next = head;
	head = delElement;
}

void *realloc (void *ptr, size_t size) {
	if (size == 0) free(ptr);
	if (ptr == NULL) return malloc(size);
	void *newElement = malloc(size);
	if (newElement == NULL) return NULL;
	memcpy(newElement, ptr, size);
	free(ptr);
	return newElement;
}

void *calloc (size_t nmemb, size_t size) {
	void *element = malloc(nmemb * size);
	if (element == NULL) return NULL;
	memset(element, 0x0, nmemb*size);
	return element;
}
