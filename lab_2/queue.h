#ifndef QUEUE_LIST_H
#define QUEUE_LIST_H

typedef struct Queue Queue;

Queue * create();

void put(Queue * queue, void * data);

int get(Queue * queue, void ** data);

int queuelength(Queue * queue);

void delete(Queue * queue);

#endif
