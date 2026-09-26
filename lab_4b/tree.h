#ifndef TREE_H
#define TREE_H

// EVERYTHING MUST HAVE \0 TERMINATOR

typedef struct Node Node;

typedef struct Node{
	char * key[3]; // {*text1, *text2, *text3}
	char * info[3];
	Node * child[4];
	Node * parent;
} Node;

typedef struct Tree{
	Node * root;
} Tree;

Tree * mktree();

void rmtree(Tree * tree);

void split(Node * node);

void merge(Node * node);

int insert(Tree * tree, char * key, char * info);

int delete(Tree * tree, char * key, int rel);

void traversal(Node * node, char * key); // not Tree * tree

int search(Tree * tree, char * key, int rel, char ** res);

int specsearch(Tree * tree, char * key, int rel, char ** reskey, char ** resinfo);

int importfile(Tree * tree, char * filename);

int graphviz(Tree * tree);

#endif
