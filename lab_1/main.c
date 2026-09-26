#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "functions.h"

int main(){
	int m, n, el, res, dup; //m строк c n элементами
	printf("Введите количество строк: ");
	res = getint(&m);
	if(res == EOF){goto end;}
	int ** srcmat = calloc(m, sizeof(int*));
	int ** resmat = calloc(m, sizeof(int*));
	int * sizes = calloc(2*m, sizeof(int));
	for(int i = 0; i < m; i++){	
		printf("Введите количество элементов %d-й строки: ", i+1);
		res = getint(&n);
		if(res == EOF){goto end;}
		srcmat[i] = calloc(n, sizeof(int));
		resmat[i] = calloc(n, sizeof(int));	
		sizes[2*i] = n;
		for(int j = 0; j < n; j++){
			printf("Введите %d-й элемент: ", j+1);
			res = getint(&el);
			if(res == EOF){goto end;}
			srcmat[i][j] = el;
		}
		sizes[2*i+1] = select(srcmat[i], resmat[i], n);
	}
	printf("Исходная матрица:\n");
	for(int i = 0; i < m; i++){
		printarr(srcmat[i], sizes[i*2]);
	}
	printf("Полученная матрица:\n");
	for(int i = 0; i < m; i++){
		printarr(resmat[i], sizes[i*2+1]);
	}	
	end:
	free_r(srcmat, m);
	free_r(resmat, m);
	free(sizes);
	return 0;
}
