#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "table.h"

int hash(Key key, int i, size_t m){
	return ((key % m) + i) % m;}

Table * mktab(size_t size){
	Table * table = malloc(sizeof(Table));
	table->ks = malloc(size * sizeof(KeySpace));
	for(int i = 0; i < size; i++){
		table->ks[i].busy = false;
		table->ks[i].key = 0;
		table->ks[i].rel = 0;
		table->ks[i].info = NULL;}
	table->msize = size;
	return table;}

void droptab(Table * table){
	if(table->ks){
		for(int i = 0; i < table->msize; i++)
			if(table->ks[i].info)
				free(table->ks[i].info);
		free(table->ks);}
	free(table);}

int insert(Table * table, Key key, char * info){ // info must be  \0-terminated
	Key rel = 1;
	int id, insid = -1;
	for(int i = 0; i < table->msize; i++){
		id = hash(key, i, table->msize);
		if(table->ks[id].key == key && table->ks[id].busy)
			rel = (rel > table->ks[id].rel ? rel : table->ks[id].rel + 1);
		if(table->ks[id].busy == false && insid == -1)
			insid = id;}
	if(insid == -1)
		return 3; // table full
	table->ks[insid].busy = true;
	table->ks[insid].key = key;
	table->ks[insid].rel = rel;
	table->ks[insid].info = malloc(sizeof(char) * (strlen(info) + 1));
	strcpy(table->ks[insid].info, info); // id -> insid    !!!!    bugfix
	return 0;} // ok

int delete(Table * table, Key key){
	int id;
	for(int i = 0; i < table->msize; i++){
		id = hash(key, i, table->msize);
		if(table->ks[id].busy && table->ks[id].key == key){
			if(table->ks[id].info)
				free(table->ks[id].info);
			table->ks[id].busy = false;
			table->ks[id].key = 0;
			table->ks[id].rel = 0;
			return 0;}} // ok
	return 1;} // no such key

int findall(Table * table, Key key, KeySpace ** res, int * total){
	int id;
	*total = 0;
        *res = malloc(table->msize * sizeof(KeySpace));
	for(int i = 0; i < table->msize; i++){
		id = hash(key, i, table->msize);
                if(table->ks[id].busy && table->ks[id].key == key){
			(*res)[*total].busy = true;
			(*res)[*total].key = key;
			(*res)[*total].rel = table->ks[id].rel;
			(*res)[*total].info = malloc(sizeof(char) * strlen(table->ks[id].info) + 1);
			strcpy((*res)[*total].info, table->ks[id].info);
			(*total)++;}}
	if(*total == 0){
		free(*res);
		return 1;} // no such key
	return 0;} // ok

int find(Table * table, Key key, Key rel, KeySpace ** res){
	int id;
        for(int i = 0; i < table->msize; i++){
		id = hash(key, i, table->msize);
                if(table->ks[id].busy && table->ks[id].key == key && table->ks[id].rel == rel){
			*res = malloc(sizeof(KeySpace));
			(*res)->busy = true;
			(*res)->key = key;
			(*res)->rel = rel;
			(*res)->info = malloc(sizeof(char) * strlen(table->ks[id].info) + 1);
			strcpy((*res)->info, table->ks[id].info);
			return 0;}} // ok
	return 1;} // no such key, rel

// CHECK IMPORT FUNCTION IT'S CRINGE
int importfile(Table ** table, char * filename){ // filename must be \0 terminated
	FILE * file = fopen(filename, "r");
	if(!file)
		return 3; // bad file
	Key key;
	char * info = malloc(1024 * sizeof(char));// fscanf automatically add \0
	int num;
	if(fscanf(file, "%d%*[^\n]%*c", &num) != 1)
		return -1; // incorrect data
	if(num < 1)
		return -1;
	*table = mktab(num);
	while(fscanf(file, "%d%*[ ]%1000[^\n]%*c", &key, info) == 2){
		if(key < 1)
			return -1;
		info[strlen(info)] = '\0';
		if(insert(*table, key, info) != 0)
			return -1;
	}
	free(info);
	fclose(file);
	return 0;
}

int exportfile(Table * table, char * filename){ // filename must be \0 terminated
	if(access(filename, F_OK) == 0)
		return -1; // file already exists
	FILE * file = fopen(filename, "w");
	if(!file)
		return 3; // unknown error
	fputs("key\trel\tinfo\n", file);
	for(int i = 0; i < table->msize; i++)
		if(table->ks[i].busy){
			fprintf(file, "%d\t%d\t%s\n", table->ks[i].key, table->ks[i].rel, table->ks[i].info);}
	fclose(file);
	return 0;} // ok

void printtab(Table * table){
	printf("key\trel\tinfo\n");
	for(int i = 0; i < table->msize; i++)
		if(table->ks[i].busy)
			printf("%d\t%d\t%s\n", table->ks[i].key, table->ks[i].rel, table->ks[i].info);
	return;}

