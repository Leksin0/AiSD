#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include "tree.h"

int main(){
	int res, key, b;
	Node * elem;
	char * input;
	char * buffer;
	char name[256];
	Tree * tree = mktree();
	while((input = readline("1 - Insert\n2 - Delete\n3 - Traversal\n4 - Search\n"
				"5 - Special search\n6 - Graphviz\n7 - Load from file\n")) != NULL){
		switch(input[0] + input[1]){
			case 49:
				buffer = readline("Enter key and info\n");
				if(sscanf(buffer, "%d %255[^\n]", &key, buffer) != 2){
					printf("Incorrect format\n");
					free(buffer);
						break;
				}
				free(buffer);
				if(key < 1){
					printf("Incorrect format\n");
					break;
				}
				insert(tree, key, buffer);
				printf("OK\n");
				break;
			case 50:
				buffer = readline("Enter key\n");
				if(sscanf(buffer, "%d", &key) != 1){
					printf("Incorrect format\n");
					free(buffer);
					break;
				}
				free(buffer);
				if(key < 1){
					printf("Incorrect format\n");
					break;
				}
				if(delete(tree, key) == -1){
					printf("No such key\n");
					break;
				}
				printf("OK\n");
				break;
			case 51:
				buffer = readline("Enter min and max keys\n");
				if(sscanf(buffer, "%d %d", &key, &b) != 2){
					printf("Incorrect format\n");
					free(buffer);
					break;
				}
				if(key < 0 || b < 1 || key >= b){
					printf("Incorrect format\n");
					break;
				}
				traversal(tree, key, b);
				break;
			case 52:
				buffer = readline("Enter key\n");
				if(sscanf(buffer, "%d", &key) != 1){
					printf("Incorrect format\n");
					break;
				}
				if(key < 1){
					printf("Incorrect format\n");
					break;
				}
				res = search(tree, key, &elem);
				if(res == 0)		
					printf("key:%d info:%s\n", key, *(elem->info));
				else
					printf("not found");
				break;
			case 53:
				buffer = readline("Enter key\n");
				if(sscanf(buffer, "%d", &key) != 1){
					printf("Incorrect format\n");
					free(buffer);
					break;
				}
				if(key < 1){
					printf("Incorrect format\n");
					break;
				}
				res = specsearch(tree, key, &elem);
				if(res == 0)		
					printf("key:%d info:%s\n", elem->key, *(elem->info));
				else
					printf("not found");
				break;
			case 54:
				exportdotfile(tree);
				printf("saved to graph.dot\n");
				break;
			case 55:
				buffer = readline("Enter file name\n");
				if(sscanf(buffer, "%255s", name) != 1){
					printf("Incorrect format\n");
					break;
				}
				if(importfile(tree, name) == 2){
					printf("bad file\n");
					break;
				}
				printf("OK\n");
				break;
			default:
				printf("No such option\n");
				break;
		}
		printf("\n");
		input[0] = '\0';
		buffer[0] = '\0';
	}
	end:
	if(input)
		free(input);
	if(tree)
		rmtree(tree);
	return 0;
}



