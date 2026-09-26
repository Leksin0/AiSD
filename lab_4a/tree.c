#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tree.h"

Tree * mktree(){
	Tree * tree = malloc(sizeof(Tree));
	tree->root = NULL;
	return tree;
}

void recsubtree(Node *node) {
	if(node == NULL)
		return;
	recsubtree(node->left);
	recsubtree(node->right);
	for(int i = 0; i < node->size; i++)
		free(node->info[i]);
	free(node->info);
	free(node);
	return;
}

void rmtree(Tree * tree){
	recsubtree(tree->root);
	free(tree);
	return;
}


int search(Tree * tree, Key key, Node ** res) {
	if(tree->root == NULL)
		return -1;
	Node * current = tree->root;
	while(current!=NULL){
		if(current->key == key){
			*res = current;
			return 0;
		}
		if(key < current->key)
			current = current->left;
		else
			current = current->right;
	}
	return -1;
}

int specsearch(Tree * tree, Key key, Node ** res){ // min key larger than given
	if(tree->root == NULL)
		return -1;
	Node * current = tree->root;
	*res = current;
	while(current != NULL){
		if(current->key > key && (current->key < (*res)->key || (*res)->key <= key))
			*res = current;
		current = current->next;
	}
	if((*res)->key <= key)
		return -1;
	return 0;
}

void traversal(Tree * tree, Key a, Key b) {
	if (tree->root == NULL){
		printf("Tree is empty\n");
		return;
	}
	Node * current = tree->root;
	while(current != NULL) {
		if(a <= current->key && current->key <= b){
			printf("Key: %d; Info: %s\n", current->key, *(current->info));	
		}
		current = current->next;
	}
}

int insert(Tree * tree, Key key, char *info) {
	Node * node = malloc(sizeof(Node));
	node->left = NULL;
	node->right = NULL;
	if (tree->root == NULL) {
		tree->root = node;
		node->key = key;
		node->size = 1;
		node->info = malloc(sizeof(char *));
		node->info[0] = strdup(info);
		node->parent = NULL;
		node->left = NULL;
		node->right = NULL;
		node->next = NULL;
		return 0;
	}
	Node * current = tree->root;
	Node * parent = NULL;
	while (current != NULL){
		parent = current;
		if (key < current->key)
			current = current->left;
		else if (key > current->key)
			current = current->right;
		else{
			current->info = realloc(current->info, (current->size + 1) * sizeof(char *));
			current->info[current->size] = strdup(info);
			current->size++;
			return 0;
		}
	}
	node->key = key;
	node->size = 1;
	node->info = malloc(sizeof(char *));
	node->info[0] = strdup(info);
	node->parent = parent;
	if(key < parent->key){
		parent->left = node;
		node->next = parent->next;
		parent->next = node;
	}
	else{
		parent->right = node;
		if(parent->next == NULL){
			parent->next = node;
			node->next = NULL;
			return 0;
		}
		current = parent;
		while(current->next != NULL){
			if(current->next->key > key){
				node->next = current->next;
				current->next = node;
				return 0;
			}
			current = current->next;
		}
		current->next = node;
	}
	return 0;
}



int delete(Tree *tree, Key key) {
	Node * current = tree->root;
	while(current != NULL){
		if(key < current->key)
			current = current->left;
		else if(key > current->key)
			current = current->right;
		else{
			Node * prev = tree->root;
			while(prev->next != current)
				prev = prev->next;
			prev->next = current->next;
			free(current->info);
			free(current);
			return 0; // ok
		}
	}
	return 1; // no key	
}



int importfile(Tree *tree, char *filename){
	FILE * file = fopen(filename, "r");
	if (!file)
		return 2; // bad file
	char buf[256];
	int key, res;
	while(1){
		res = fscanf(file, "%d%*[^\n]%*c", &key);
		if(res != 1)
			break;
		res = fscanf(file, "%255s%*[^\n]%*c", buf);
		if(res != 1)
			break;
		insert(tree, key, buf);
	}
	fclose(file);
	return 0;
}

void recurnodetodot(Node* node, FILE* file) {
	if(node == NULL)
		return;
	fprintf(file, "n%d [label=\"{<k>%d|{<l>|<r>}}\"];\n", node->key, node->key);
	if (node->left != NULL) {
		fprintf(file, "n%d:l -> n%d;\n", node->key, node->left->key);
		recurnodetodot(node->left, file);
	}
	if (node->right != NULL) {
		fprintf(file, "n%d:r->n%d;\n", node->key, node->right->key);
		recurnodetodot(node->right, file);
	}
	return;
}

void exportdotfile(Tree* tree){
	if (tree == NULL || tree->root == NULL)
		return;
	FILE* file = fopen("graph.dot", "w");
	fprintf(file, "digraph G {\n");
	fprintf(file, "node[shape=record];\n");
	recurnodetodot(tree->root, file);
	fprintf(file, "}\n");
	fclose(file);
	return;
}








