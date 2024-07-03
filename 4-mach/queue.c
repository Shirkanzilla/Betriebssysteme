#include "queue.h"
#include "sem.h"
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct node {
	char *cmd;
	char *out;
	int flags;
	struct node *next;
} node_t;

node_t *head;
bool init = false;
SEM *lock;
SEM *availableEntries;

int queue_init(void) {
	head = (node_t *)malloc(sizeof(node_t));
	if (head == NULL) return -1;
	head->next = NULL;
	lock = semCreate(1);
	if (lock == NULL){
		free(head);
		return -1;
	}
	availableEntries = semCreate(0);
	if (availableEntries == NULL){
		free(head);
		semDestroy(lock);
		return -1;
	}
	init = true;
	return 0;
}

void queue_deinit(void) {
    free(head);
    semDestroy(lock);
    semDestroy(availableEntries);
}

int queue_put(char *cmd, char *out, int flags) {
	if (!init) return -1;
    	P(lock);
    	//Critical Section
    	node_t *current = head;
	while (current->next != NULL){
		current = current->next;
	}
	node_t *newElement = (node_t *)malloc(sizeof(node_t));
	if (newElement == NULL){
		errno = ENOMEM;
		return -1;
	}
	newElement->cmd = cmd;
	newElement->out = out;
	newElement->flags = flags;
	current->next = newElement;
	V(availableEntries);
	V(lock);
    	return 0;
}

int queue_get(char **cmd, char **out, int *flags) {
    	if (!init) return -1;
	P(lock);
	P(availableEntries);
	node_t *retNode = head->next;
	cmd = &retNode->cmd;
	out = &retNode->out;
	flags = &retNode->flags;
	head->next = retNode->next;
	free(retNode);
	V(lock);
	return 0;
	
}
