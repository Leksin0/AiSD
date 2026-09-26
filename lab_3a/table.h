#ifndef TABLE_H
#define TABLE_H
#define Key unsigned int
typedef struct{
        Key key; // -1 - free or unique key
        Key par; // 0 - root or existing key
        char * info; // \0 terminated string
} KeySpace;

typedef struct{
        KeySpace *ks; // ptr to spc of el-ts
        size_t msize; // max number of el-ts
        size_t csize; // cur amount of el-ts
} Table;

Table * mktab(size_t size);

void deltab(Table * table);

int readfromfile(Table ** table, char * filename);

int insert(Table * table, Key key, Key par, char * info);

int safedel(Table * table, Key key);

int findkey(Table * table, Key key, Key * par, char * data);

int selbypar(Table * table, Key min, Key max);

void showtab(Table * table);
#endif
