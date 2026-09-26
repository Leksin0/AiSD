#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <readline/readline.h>
#include "tree.h"

int main(){
	int res, num;
	char *operation = NULL, *buffer = NULL, *bufkey = NULL, *bufinfo = NULL;
	Tree * tree = mktree();
	while((operation = readline("1 - Insert\n2 - Delete\n3 - Traversal\n4 - Search\n"
						"5 - Special search\n6 - Graphviz\n7 - Load from file\n")) != NULL){
		switch(operation[0] + operation[1]){
			case 49: // insert
				bufkey = readline("Enter key:\n");
				if(bufkey == NULL) // EOF
					goto end;
				if(bufkey[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				bufinfo = readline("Enter info:\n");
				if(bufinfo == NULL) // EOF
					goto end;
				if(bufinfo[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				insert(tree, bufkey, bufinfo);
				printf("ok\n");
				break;

			case 50: // delete
				bufkey = readline("Enter key:\n");
				if(bufkey == NULL) // EOF
					goto end;
				if(bufkey[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				buffer = readline("Enter ordinal number of duplicate:\n");
				if(buffer == NULL) // EOF
					goto end;
				if(sscanf(buffer, "%d", &num) != 1){
					printf("Incorrect format\n");
					break;
				}
				res = delete(tree, bufkey, num);
				if(res == 0)
					printf("ok\n");
				else if(res == 1)
					printf("Not so many duplicates, deleted last\n");
				else
					printf("Key not found\n");
				break;

			case 51: //                                                 TRAVERSAL
				bufkey = readline("Enter key or nothing for full traversal:\n");
				if(bufkey == NULL) // EOF
					goto end;
				if(bufkey[0] == '\0'){
					free(bufkey);
					bufkey = NULL;
				}
				traversal(tree->root, bufkey);
				break;

			case 52: //                                                  SEARCH
				bufkey = readline("Enter key:\n");
				if(bufkey == NULL) // EOF
					goto end;
				if(bufkey[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				buffer = readline("Enter ordinal number of duplicate:\n");
				if(buffer == NULL) // EOF
					goto end;
				if(sscanf(buffer, "%d", &num) != 1){
					printf("Incorrect format\n");
					break;
				}
				res = search(tree, bufkey, num, &bufinfo);
				if(res == 0)
					printf("%s:  %s\n", bufkey, bufinfo);
				else if(res == 1)
					printf("%s:  %s, not so many duplicates, found last\n", bufkey, bufinfo);
				else
					printf("Key found key");
				break;

			case 53:  //                                              SPECIAL SEARCH
				bufkey = readline("Enter key:\n");
				if(bufkey == NULL) // EOF
					goto end;
				if(bufkey[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				buffer = readline("Enter ordinal number of duplicate:\n");
				if(buffer == NULL) // EOF
					goto end;
				if(sscanf(buffer, "%d", &num) != 1){
					printf("Incorrect format\n");
					break;
				}
				free(buffer);
				res = specsearch(tree, bufkey, num, &buffer, &bufinfo);
				if(res == 0)
					printf("%s:  %s\n", buffer, bufinfo);
				else if(res == 1)
					printf("%s:  %s, not so many duplicates, found last\n", buffer, bufinfo);
				else
					printf("Key not found");
				break;

			case 54: //                                            GRAPHVIZ
				res = graphviz(tree);
				if(res == 0)
					printf("Saved to graph.dot\n");
				else
					printf("Tree is empty");
				break;

			case 55: //                                             IMPORT
				buffer = readline("Enter file name\n");
				if(buffer == NULL) // EOF
					goto end;
				if(buffer[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				res = importfile(tree, buffer);
				if(res == 0)
					printf("ok\n");
				else
					printf("Can't read that file");
				break;
			default:
				printf("No such option\n");
				break;
		}
		printf("\n");
		if(operation != NULL) {
			free(operation);
			operation = NULL;
		}
		if(buffer != NULL) {
			free(buffer);
			buffer = NULL;
		}
		if(bufkey != NULL) {
			free(bufkey);
			bufkey = NULL;
		}
		if(bufinfo != NULL) {
			free(bufinfo);
			bufinfo = NULL;
		}
	}
	end:
	if(operation != NULL)
		free(operation);
	if(buffer != NULL)
		free(buffer);
	if(bufkey != NULL)
		free(bufkey);
	if(bufinfo != NULL)
		free(bufinfo);
	if(tree)
		rmtree(tree);
	return 0;
}



