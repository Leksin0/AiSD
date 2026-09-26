#ifndef TABLE_H
#define TABLE_H

#include <stdbool.h>

#define Key unsigned int

typedef struct{
	bool busy;
	Key key;
	Key rel;
	char * info;
} KeySpace;

typedef struct{
        KeySpace *ks;
        size_t msize;
} Table;

Table * mktab(size_t size);

void droptab(Table * table);

int insert(Table * table, Key key, char * info);

int delete(Table * table, Key key);

int findall(Table * table, Key key, KeySpace ** res, int * total);

int find(Table * table, Key key, Key rel, KeySpace ** res);

int importfile(Table ** table, char * filename);

int exportfile(Table * table, char * filename);

void printtab(Table * table);
#endif

