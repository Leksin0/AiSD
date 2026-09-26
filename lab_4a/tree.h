#ifndef TREE_H
#define TREE_H

#define Key unsigned int

typedef struct Node{
	Key key;
	size_t size;
	char ** info;
	struct Node * next;
	struct Node * parent;
	struct Node * left;
	struct Node * right;
} Node;

typedef struct{
	Node * root;
} Tree;

Tree * mktree();

void rmtree(Tree * tree);

int insert(Tree * tree, Key key, char * info);

int delete(Tree * tree, Key key);

void traversal(Tree * tree, Key a, Key b);

int search(Tree * tree, Key key, Node ** res);

int specsearch(Tree * tree, Key key, Node ** res);

void exportdotfile(Tree * tree);

int importfile(Tree * tree, char * filename);
#endif
