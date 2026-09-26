#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdbool.h>
int getint(int * data);

void free_r(int ** arr, int m);

bool isin(int el, int * arr, int n);

void printarr(int * arr, int n);

int select(int * src, int * res, int n);
#endif
