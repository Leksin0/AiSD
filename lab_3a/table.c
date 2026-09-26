#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "table.h"

Table * mktab(size_t size){
	Table * table = malloc(sizeof(Table));
	table->ks = malloc(size * sizeof(KeySpace));
	for(int i = 0; i < size; i++){
		table->ks[i].key = -1; // not busy
		table->ks[i].par = 0;
		table->ks[i].info = NULL;
	}
	table->msize = size;
	table->csize = 0;
	return table;
}


void deltab(Table * table){
	for(int i = 0; i < table->msize; i++){
		if(table->ks[i].info)
			free(table->ks[i].info);
	}
	free(table->ks);
	free(table);
}


int readfromfile(Table ** table, char * filename){ // filename must be \0 terminated
	FILE * file = fopen(filename, "r");
	if(!file)
		return 3; // bad file
	Key key, par;
	char * info = malloc(1024 * sizeof(char));// fscanf automatically add \0
	int num;
	if(fscanf(file, "%d%*[^\n]%*c", &num) != 1)
		return -1; // incorrect data
	if(num < 1)
		return -1;
	*table = mktab(num);
	while(fscanf(file, "%d%*[ ]%d%*[ ]%1000[^\n]%*c", &key, &par, info) == 3){
		if(key < 1 || par < 0)
			return -1;
		info[strlen(info)] = '\0';
		if(insert(*table, key, par, info) != 0)
			return -1;
	}
	free(info);
	fclose(file);
	return 0;	
}


int insert(Table * table, Key key, Key par, char * info){
	if(table->msize == table->csize)
		return 3; // table full
	bool isok = (par == 0);
	int id = 0;
	for(int i = 0; i < table->msize; i++){
		if(table->ks[i].key == key)
			return 1; // key alr exist
		if(table->ks[i].key == par)
			isok = true;
		if(table->ks[i].key == -1)
			id = i;
	}
	if(!isok)
		return 2; // no such parent
	table->ks[id].key = key;
	table->ks[id].par = par;
	table->ks[id].info = malloc((strlen(info) + 1) * sizeof(char));
	strcpy(table->ks[id].info, info);
	table->csize++;
	return 0; // ok
}


int safedel(Table * table, Key key){
	int id = -1;
	for(int i = 0; i < table->msize; i++){
		if(table->ks[i].par == key)
			return 2; // key has child
		if(table->ks[i].key == key)
			id = i;
	}
	if(id == -1)
		return 1; // no such key
	table->csize--;
	table->ks[id].key = -1;
	table->ks[id].par = 0;
	if(table->ks[id].info)
		free(table->ks[id].info);
	return 0; // ok
}


int findkey(Table * table, Key key, Key * par, char * data){
        for(int i = 0; i < table->msize; i++)
                if(table->ks[i].key == key){
			*par = table->ks[i].par;
			data = realloc(data, (strlen(table->ks[i].info) + 1) * sizeof(char));
                	strcpy(data, table->ks[i].info);
			return 0;} // ok	
	return 1; // no such key
}


int selbypar(Table * table, Key min, Key max){
	Table * res = mktab(table->csize);
	for(int i = 0; i < table->csize; i++){
		if(table->ks[i].par >= min && table->ks[i].par <= max)
			insert(res, table->ks[i].key, 0, table->ks[i].info);
	}
	showtab(res);
	deltab(res);
	return 0; //ok
}

void showtab(Table * table){
	if(table->csize == 0){
		printf("TABLE IS EMPTY\n");
		// return;
	}
	printf("key\tparent\tinfo\n");
	for(int i = 0; i < table->msize; i++)
		if(table->ks[i].key != -1)
			printf("%d\t%d\t%s\n", table->ks[i].key, table->ks[i].par, table->ks[i].info);
	return;
}





