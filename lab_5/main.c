#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include "graph.h"

int main(){
	int res, delay, num, count;
	unsigned int * ports;
	char *input=NULL, *buffer=NULL, *bufA=NULL, *bufB=NULL;
	Network * net = mknet();
	Path * path;
	while((input = readline("1 - Add or edit computer\n2 - Add or edit connection\n3 - Delete computer\n4 - Delete connection\n"
				"5 - Search path (Dijkstra)\n6 - Traversal search (BFS)\n7 - Minimize connections\n"
				"8 - Print adjacency lists\n9 - Make Graphviz file\n")) != NULL){
		switch(input[0] + input[1]){
			case 49: //                              ADD   COMPUTER
				bufA = readline("Enter computer name:\n");
				if(bufA == NULL) // EOF
					goto end;
				if(bufA[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				buffer = readline("Enter service:\n");
				if(buffer == NULL) // EOF
					goto end;
				if(sscanf(buffer, "%d", &num) != 1){
					printf("Incorrect format\n");
					break;
				}
				updatecomputer(net, bufA, num);
				printf("ok\n");
				break;

			case 50: //                             ADD    CONNECTION
				bufA = readline("Enter first computer name:\n");
				if(bufA == NULL) // EOF
					goto end;
				if(bufA[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				bufB = readline("Enter second computer name:\n");
				if(bufB == NULL) // EOF
					goto end;
				if(bufB[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				buffer = readline("Enter delay:\n");
				if(buffer == NULL) // EOF
					goto end;
				if(sscanf(buffer, "%d", &delay) != 1){
					printf("Incorrect format\n");
					break;
				}
				buffer = readline("Enter amount of available ports:\n");
				if(buffer == NULL) // EOF
					goto end;
				if(sscanf(buffer, "%d", &count) != 1){
					printf("Incorrect format\n");
					break;
				}
				ports = malloc(sizeof(unsigned int) * count);
				for(int i = 0; i < count; i++){
					buffer = readline("Enter port:\n");
					if(buffer == NULL) // EOF
						goto end;
					if(sscanf(buffer, "%d", &num) != 1){
						printf("Incorrect format\n");
						break;
					}
					ports[i] = num;
					free(buffer);
				}
				updateconnection(net, bufA, bufB, delay, count, ports);
				break;

			case 51: //                              DELETE   COMPUTER
				bufA = readline("Enter computer name:\n");
				if(bufA == NULL) // EOF
					goto end;
				if(bufA[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				res = deletecomputer(net, bufA);
				if(res == 0)
					printf("ok\n");
				else
					printf("not found\n");
				break;

			case 52: //                              DELETE   CONNECTION
				bufA = readline("Enter first computer name:\n");
				if(bufA == NULL) // EOF
					goto end;
				if(bufA[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				bufB = readline("Enter second computer name:\n");
				if(bufB == NULL) // EOF
					goto end;
				if(bufB[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				res = deleteconnection(net, bufA, bufB);
				if(res == 0)
					printf("ok\n");
				else
					printf("not found\n");
				break;

			case 53: //                                  DIJKSTRA
				bufA = readline("Enter first computer name:\n");
				if(bufA == NULL) // EOF
					goto end;
				if(bufA[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				bufB = readline("Enter second computer name:\n");
				if(bufB == NULL) // EOF
					goto end;
				if(bufB[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				buffer = readline("Enter port:\n");
				if(buffer == NULL) // EOF
					goto end;
				if(sscanf(buffer, "%d", &num) != 1){
					printf("Incorrect format\n");
					break;
				}
				res = dijkstra(net, bufA, bufB, num, &path, &delay);
				if(res == 0){
					while(path != NULL){
						printf("%s\n", path->name);
						path = path->next;
					}
					printf("total delay: %d\n", delay);
				}
				else
					printf("not found\n");
				break;

			case 54: //                                     BFS
				bufA = readline("Enter computer name:\n");
				if(bufA == NULL) // EOF
					goto end;
				if(bufA[0] == '\0'){
					printf("Empty input\n");
					break;
				}
				buffer = readline("Enter port:\n");
				if(buffer == NULL) // EOF
					goto end;
				if(sscanf(buffer, "%d", &num) != 1){
					printf("Incorrect format\n");
					break;
				}
				res = BFS(net, bufA, num, &bufB);
				if(res == 0)
					printf("%s\n", bufB);
				else
					printf("not found\n");
				break;

			case 55: //                                    MINIMIZE
				printf("ok\n");
				break;

			case 56: //                                     PRINT
				printlists(net);
				break;

			case 57: //                                  GRAPHVIZ
				graphviz(net);
				printf("saved to graph.dot\n");
				break;

			default:
				printf("No such option\n");
				break;
		}
		printf("\n");
		if(input != NULL) {
			free(input);
			input = NULL;
		}
		if(buffer != NULL) {
			free(buffer);
			buffer = NULL;
		}
		if(bufA != NULL) {
			free(bufA);
			bufA = NULL;
		}
		if(bufB != NULL) {
			free(bufB);
			bufB = NULL;
		}
	}
	end:
	if(input)
		free(input);
	if(buffer)
		free(buffer);
	if(bufA)
		free(bufA);
	if(bufB)
		free(bufB);
	if(ports)
		free(ports);
	rmnet(net);
	return 0;
}



