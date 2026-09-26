#include <stdlib.h>
#include <stddef.h>
#include "queue.h"

typedef struct Item{
	void * data;
	struct Item * next;
} Item;

struct Queue{
	Item * head;
	Item * tail;
};

Queue * create(){
	Queue * queue = malloc(sizeof(Queue));
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}

void put(Queue * queue, void * data){
	Item * item = malloc(sizeof(Item));
	item->data = data;
	item->next = NULL;
	if(queue->tail != NULL)
		queue->tail->next = item;
	else
		queue->head = item;
	queue->tail = item;
}

int get(Queue * queue, void ** data){
	if(queue->head != NULL){
		*data = queue->head->data;
		Item * temp = queue->head;
		if(queue->head->next == NULL)
			queue->tail = NULL;
		queue->head = queue->head->next;	
		free(temp);
		return 0;
	}
	return -1;
}

int queuelength(Queue * queue){
	if(queue->head == NULL)
		return 0;
	int l = 1;
	Item * cit = queue->head;
	while(cit != NULL){
		l++;
		cit = cit->next;
	}
	return l;
}

void delete(Queue * queue){
	Item *cit = queue->head, *temp;
	while(cit != NULL){
		free(cit->data);
		temp = cit->next;
		free(cit);
		cit = temp;	
	}
	free(queue);
}











