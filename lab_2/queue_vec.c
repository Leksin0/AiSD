#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "queue.h"

struct Queue{
	void ** data;
	int fill;
	int size;
};

Queue * create(){
	Queue * queue = malloc(sizeof(Queue));	
	queue->data = malloc(sizeof(void **));
	queue->fill = 0;
	queue->size = 1;
	return queue;
}

void put(Queue * queue, void * data){
	if(queue->fill == queue->size){ // full
		queue->size *= 2;
		queue->data = realloc(queue->data, queue->size * sizeof(void *));	
	}
	queue->data[queue->fill++] = data;
}

int get(Queue * queue, void ** data){
	if(queue->fill == 0)
		return -1;
	*data = queue->data[0];
	queue->fill--;
	if(queue->fill > 0)
		memmove(queue->data, queue->data + 1, queue->fill * sizeof(void *));
	return 0;
}

int queuelength(Queue * queue){
	return queue->fill;
}

void delete(Queue * queue){
	for (int i = 0; i < queue->fill; i++)
		free(queue->data[i]);
	free(queue->data);
	free(queue);
}
