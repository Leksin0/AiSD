#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "functions.h"

int getint(int * data){
        int res = 0;
        while(res == 0){ 
                res = scanf("%d", data);
                scanf("%*[^\n]");
                scanf("%*c");
        }   
        return res;
}

void free_r(int ** arr, int m){ 
        for(int i = 0; i < m; i++){
                free(arr[i]);
        }   
        free(arr);
        return;
}

bool isin(int el, int * arr, int n){ 
        if(n == 0){ 
                return false;
        }   
        for(int i = 0; i < n; i++){
                if(arr[i] == el){
                        return true;
                }   
        }   
        return false;
}

void printarr(int * arr, int n){ 
        for(int i = 0; i < n; i++){
                printf("%d ", arr[i]);
        }   
        printf("\n");
        return;
}

int select(int * src, int * res, int n){ 
        int dup = 0;
        for(int i = 0; i < n; i++){
                if(isin(src[i], src, i) || isin(src[i], src + i + 1, n - i - 1)){ // here might be error
                        dup++;
                        res[dup - 1] = src[i];
                }   
        }   
        return dup;
}

