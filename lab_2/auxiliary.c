#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <readline/readline.h>
#include "auxiliary.h"
#include "queue.h"

void twodesks(int * d1, int * d2, int total){
	*d1 = rand() % total;
	do{
		*d2 = rand() % total;
	}while(*d1 == *d2);
}

int stoui(const char * src){
        for(int i = 0; i < (int)strlen(src); i++){
                if(!isdigit(src[i]))
                        return -1; 
        }   
        return atoi(src);
}

int getinfo(int * total, Queue * queue){
	srand(time(NULL));
        Passenger * man;
        int res;
        printf("<число стоек(минимум 2)> <имя>/<время прибытия>/<время обслуживания>\n");
	char * input = readline("");
        char * info, * svinput, * svpas;
	char * pas = strtok_r(input, " ", &svinput);
	res = stoui(pas);
	if(res == -1 || res < 2) goto errflag;
	*total = res;
	pas = strtok_r(NULL, " ", &svinput);
        while(pas != NULL){
                man = malloc(sizeof(Passenger));
		//name
                info = strtok_r(pas, "/", &svpas);
		if(info == NULL) goto errflag;
                man->name = malloc((strlen(info)+1) * sizeof(char));
                strcpy(man->name, info);
		//ta
                info = strtok_r(NULL, "/", &svpas);
		if(info == NULL) goto errflag;
                res = stoui(info);
		if(res == -1) goto errflag;
		man->ta = res;
                //ts
                info = strtok_r(NULL, "/", &svpas);
		if(info == NULL) goto errflag;
                res = stoui(info);
		if(res == -1) goto errflag;
		man->ts = res;
		//
		put(queue, man);
		pas = strtok_r(NULL, " ", &svinput);
	}
	free(input);
        return 0;

	errflag:
        delete(queue);
	free(input);
        printf("Неверный формат ввода!\n");
        return -1; 
}

