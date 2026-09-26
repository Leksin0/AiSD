#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "auxiliary.h"
#include "queue.h"
#include <dlfcn.h>

int main(){
        //dlfcn
        void * handle = dlopen("queue.dylib", RTLD_LAZY);
       	Queue * (*create)() = dlsym(handle, "create");
        void (*put)() = dlsym(handle, "put");
        int (*get)() = dlsym(handle, "get");
        int (*queuelength)() = dlsym(handle, "queuelength");
        void (*delete)() = dlsym(handle, "delete");

	// init
	Passenger *man, *phantom;
	int res, temp, total, d1, d2, cdpnl, time = 1, dsallen = 0;
	Queue * queue;
	do{
		queue = create();
		res = getinfo(&total, queue);
	}while(res != 0);
	Queue ** desks = calloc(total, sizeof(Queue *));
	int * dstimeleft = calloc(total, sizeof(int));
	char ** dscurpas = calloc(total, sizeof(char *));
	for(int i = 0; i < total; i++){
		dstimeleft[i] = 0;
		desks[i] = create();
		dscurpas[i] = calloc(16, sizeof(char));
		dscurpas[i][0] = '\0';
	}

	// main

	res = get(queue, &man);
	while(res == 0 || dsallen != 0){
		//arrival
		while(time >= man->ta && res == 0){
			twodesks(&d1, &d2, total);
			if(queuelength(desks[d2]) < queuelength(desks[d1]))
				put(desks[d2], man);
			else
				put(desks[d1], man);
			res = get(queue, &man);
		}
		//update desks
		dsallen = 0;
		for(int i = 0; i < total; i++){
			if(dstimeleft[i] == 0){ //desk is empty or served
				*(dscurpas[i]) = '\0';
				temp = get(desks[i], &phantom);
				if(temp == 0){
					dstimeleft[i] = phantom->ts;
					strcpy(dscurpas[i], phantom->name);
					dsallen++;
				}
			}
			else{
				dstimeleft[i]--;
				dsallen++;
			}
		}
		//print current state
		printf("%d\t", time);
		for(int i = 0; i < total; i++){
			cdpnl = strlen(dscurpas[i]) + 5;
			printf("№%d: %s-", i+1, dscurpas[i]);
				for(int k = queuelength(desks[i]); k > 1; k--){
					temp = get(desks[i], &phantom);
					cdpnl += strlen(phantom->name) + 1;
					printf("%s", phantom->name);
					printf(",");
					put(desks[i], phantom);
				}
				for(int s = cdpnl / 8; s < 3; s++)
					printf("\t");
		}
		printf("\n");
		//update time
		time++;
	}

	// memclear
	for(int i=0;i<total;i++)
		delete(desks[i]);
	delete(queue);
	free(desks);
	free(dstimeleft);
	dlclose(handle);
	return 0;
}






