#ifndef LIB_H
#define LIB_H
#include "queue.h"

typedef struct Passenger{
	char * name;
	int ta;
	int ts;
} Passenger;

void twodesks(int * d1, int * d2, int total);

int stoui(const char * src);

int getinfo(int * total, Queue * queue);

#endif
