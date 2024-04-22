#include <stdio.h>
#include <stdlib.h>

// TODO: structs, global variables, etc.
struct listelement {
	int value;
	struct listelement *next;
};
struct listelement *head;

static int insertElement(int value) {
	if(head == NULL){ //initialises the head if not already done
		head = malloc(sizeof(struct listelement));
		if(head == NULL) return -1;
		head->value = -2;
		head->next = NULL;
	}
	if(value<0) return -1;
	struct listelement *tmp=head;
	while(1){//loops through the list until the last element and concats the new one unless value is already in the list 
		if(tmp->value == value) break;
		if(tmp->next == NULL){
			tmp->next = malloc(sizeof(struct listelement));
			if(tmp->next == NULL) break;
			tmp->next->value=value;
			tmp->next->next= NULL;
			return value;
		}
		tmp = tmp->next;
	}	
	return -1;
}

static int removeOne(struct listelement *elem){
	int value = elem->value;
	free(elem);
	return value;
}

static int removeElement(void) {
	// TODO: implement me!
	if(head == NULL) return -1;
	struct listelement *tmp = head;
	if(tmp->next == NULL) return removeOne(tmp);
	while(1){	
		struct listelement *next = tmp->next;
		if(next->next==NULL) {
			tmp->next=NULL;
			return removeOne(next);
		}
		tmp = next;
		
	}
	return -1;
}

int main (int argc, char* argv[]) {
	printf("insert 47: %d\n", insertElement(47));	
	printf("insert 11: %d\n", insertElement(11));
	printf("insert 23: %d\n", insertElement(23));
	printf("insert 11: %d\n", insertElement(11));
	
	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());

	// TODO: add more tests
	exit(EXIT_SUCCESS);
}
