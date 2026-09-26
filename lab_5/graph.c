#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "graph.h"

Network * mknet(){
	Network * net = malloc(sizeof(Network));
	net->count = 0;
	net->comps = NULL;
	return net;
}

void rmnet(Network * net){
	Computer * comp;
	Connection * conn, *old;
	for(int i = 0; i < net->count; i++){
		comp = net->comps[i];
		conn = comp->adjacent;
		while(conn != NULL){
			free(conn->ports);
			old = conn;
			conn = conn->next;
			free(old);
		}
		free(comp->name);
		free(comp);
	}
	free(net->comps);
	free(net);
}

int updatecomputer(Network * net, char * name, unsigned int service){
	for(int i = 0; i < net->count; i++){
		if(strcmp(net->comps[i]->name, name) == 0){
			net->comps[i]->service = service;
			return 0;
		}
	}
	Computer * comp = malloc(sizeof(Computer));
	comp->name = strdup(name);
	comp->service = service;
	comp->adjacent = NULL;
	net->comps = realloc(net->comps, sizeof(Computer*) * (net->count + 1));
	net->comps[net->count] = comp;
	net->count++;
	return 0;
}

int updateconnection(Network * net, char * nameA, char * nameB, unsigned int delay, size_t count, unsigned int * ports){
	Computer * compA = NULL, * compB = NULL;
	Connection *connAB, *connBA;
	for(int i = 0; i < net->count; i++){
		if(strcmp(net->comps[i]->name, nameA) == 0)
			compA = net->comps[i];
		if(strcmp(net->comps[i]->name, nameB) == 0)
			compB = net->comps[i];
		if(compA != NULL && compB != NULL)
			break;
	}
	if(compA == NULL || compB == NULL)
		return 1; // no such identificators

	connAB = compA->adjacent;
	while(connAB != NULL){
		if(connAB->target == compB){ // connection already exists
			free(connAB->ports);
			connAB->count = count;
			connAB->ports = malloc(sizeof(unsigned int) * count);
			memmove(connAB->ports, ports, count * sizeof(unsigned int));
			connBA = compB->adjacent;
			while(connBA->target != compA){
				connBA = connBA->next;
				if(connBA == NULL)
					return -1; // damned error
			}
			free(connBA->ports);
			connBA->count = count;
			connBA->ports = malloc(sizeof(unsigned int) * count);
			memmove(connBA->ports, ports, count * sizeof(unsigned int));
			return 0;
		}
		connAB = connAB->next;
	}
	connAB = malloc(sizeof(Connection));
	connAB->count = count;
	connAB->ports = malloc(sizeof(unsigned int) * count);
	memmove(connAB->ports, ports, count * sizeof(unsigned int));
	connAB->delay = delay;
	connAB->target = compB;
	connAB->next = compA->adjacent;
	compA->adjacent = connAB;
	
	connBA = malloc(sizeof(Connection));
	connBA->count = count;
	connBA->ports = malloc(sizeof(unsigned int) * count);
	memmove(connBA->ports, ports, count * sizeof(unsigned int));
	connBA->delay = delay;
	connBA->target = compA;
	connBA->next = compB->adjacent;
	compB->adjacent = connBA;
	return 0; // ok
}

int deletecomputer(Network * net, char * name){
	Computer * comp = NULL;	
	for(int i = 0; i < net->count; i++){
		if(strcmp(net->comps[i]->name, name) == 0){
			comp = net->comps[i];
			if(i < net->count - 1)
				memmove(&(net->comps[i]), &(net->comps[i+1]), (net->count - i - 1) * sizeof(Computer *));
			net->count--;
			break;
		}
	}
	if(comp == NULL)
		return 1; // not found
	while(comp->adjacent != NULL)
		deleteconnection(net, name, comp->adjacent->target->name);
	free(comp->name);
	free(comp);
	return 0;
}

int deleteconnection(Network * net, char * nameA, char * nameB){	
	Computer * compA = NULL, * compB = NULL;
	for(int i = 0; i < net->count; i++){
		if(strcmp(net->comps[i]->name, nameA) == 0)
			compA = net->comps[i];
		if(strcmp(net->comps[i]->name, nameB) == 0)
			compB = net->comps[i];
		if(compA != NULL && compB != NULL)
			break;
	}
	if(compA == NULL || compB == NULL)
		return 1; // no such identificators
	Connection * conn = compA->adjacent;
	Connection ** prev = &(compA->adjacent);
	while(strcmp(conn->target->name, nameB) != 0){
		prev = &(conn->next);
		conn = conn->next;
		if(conn == NULL)
			return -1; // damned error
	}
	*prev = conn->next;
	free(conn->ports);
	free(conn);
	conn = compB->adjacent;
	prev = &(compB->adjacent);
	while(strcmp(conn->target->name, nameA) != 0){
		prev = &(conn->next);
		conn = conn->next;
		if(conn == NULL)
			return -1; // damned error
	}
	*prev = conn->next;
	free(conn->ports);
	free(conn);
	return 0;
}

int BFS(Network * net, char * start, unsigned int service, char ** res){
	int startid = -1, curid, targid, tempid, front = 0, rear = 0;
	for (int i = 0; i < net->count; i++) {
		if (strcmp(net->comps[i]->name, start) == 0) {
			startid = i;
			break;
		}
	}
	if (startid == -1)
		return 2; // error 404
    bool * visited = malloc(net->count * sizeof(bool));
    Computer ** previous = malloc(net->count * sizeof(Computer*));
	Computer ** queue = malloc(net->count * sizeof(Computer*));
    for (int i = 0; i < net->count; i++) {
        visited[i] = false;
        previous[i] = NULL;
    }
    visited[startid] = true;
    queue[rear++] = net->comps[startid];
    while (front < rear) {
        Computer *current = queue[front++];
        for (int i = 0; i < net->count; i++) {
            if (net->comps[i] == current) {
                curid = i;
                break;
            }
        }
        if (current->service == service && curid != startid){
			*res = strdup(current->name);
        	free(visited);
        	free(previous);
        	free(queue);
            return 0; //ok
        }
        Connection * conn = current->adjacent;
        while(conn != NULL) {
            bool portok = false;
            for(int j = 0; j < conn->count; j++) {
                if (conn->ports[j] == service) {
                    portok = true;
                    break;
                }
            }
            if(portok) {
                for(int k = 0; k < net->count; k++) {
                    if(net->comps[k] == conn->target) {
                        targid = k;
                        break;
                    }
                }
                if (!visited[targid]) {
                    visited[targid] = true;
                    previous[targid] = current;
                    queue[rear++] = conn->target;
                }
            }
            conn = conn->next;
        }
    }
    free(visited);
    free(previous);
    free(queue);
    return 1; // not found
}

int dijkstra(Network * net, char * nameA, char * nameB, unsigned int port, Path ** result, int * totaldelay){
	Computer *comp,  *compA=NULL, *compB=NULL;
	Connection * conn;
	Path *nextpt, *path;
	int curind, destind, mindel, tarind;
	for(int i = 0; i < net->count; i++){
		if(strcmp(net->comps[i]->name, nameA) == 0){
			compA = net->comps[i];
			curind = i;
		}
		if(strcmp(net->comps[i]->name, nameB) == 0){
			compB = net->comps[i];
			destind = i;
		}
	}
	if (compA == NULL || compB == NULL)
		return 2; // error 404
	int * delay = malloc(net->count * sizeof(int));
	int * prev = malloc(net->count * sizeof(int));
	bool * visited = malloc(net->count * sizeof(bool));
	for (int i = 0; i < net->count; i++) {
		delay[i] = INT_MAX;
		prev[i] = -1;
		visited[i] = false;
	}
	delay[curind] = 0;
	while(true){
		mindel = INT_MAX;
		for(int i = 0; i < net->count; i++){
			if(!visited[i] && delay[i] < mindel){
				mindel = delay[i];
				curind = i;
			}
		}
		if(mindel == INT_MAX)
			break;  // no more reachable nodes
		visited[curind] = true;
		comp = net->comps[curind];
		conn = comp->adjacent;	
		while(conn != NULL){
			for(int i = 0; i < conn->count; i++)
				if(conn->ports[i] == port)
					goto proccon;
			goto nextcon;
			proccon:
			for(int i = 0; i < net->count; i++){
				if(net->comps[i] == conn->target){
					tarind = i;
					break;
				}
			}
			if(delay[curind] + conn->delay < delay[tarind]){
				delay[tarind] = delay[curind] + conn->delay;
				prev[tarind] = curind;
			}
			nextcon:
			conn = conn->next;
		}
	}
	if(delay[destind] == INT_MAX){
		free(delay);
		free(prev);
		free(visited);
		return 1; // no way
	}
	curind = destind;
	nextpt = NULL;
	while(curind != -1){
		path = malloc(sizeof(Path));
		path->next = nextpt;
		path->name = strdup(net->comps[curind]->name);
		curind = prev[curind];
		nextpt = path;
	}
	*totaldelay = delay[destind];
	*result = path;
	free(delay);
	free(prev);
	free(visited);
	return 0;
}

void graphcomp(FILE * file, Computer * comp){
	fprintf(file, "\"%s\" [label=\"{{<n>%s}|{<p>%d}}\"];\n",
		comp->name, comp->name, comp->service);
	Connection * conn = comp->adjacent;
	while(conn != NULL){
		fprintf(file, "\"%s\" -- \"%s\" [label=\"%d\"];\n",
			comp->name, conn->target->name, conn->delay);
		conn = conn->next;
	}
	return;
}

int graphviz(Network * net){
	if (net->count == 0)
		return 1;
	FILE * file = fopen("graph.dot", "w");
	fprintf(file, "graph A {\n");
	fprintf(file, "node[shape=record];\n");
	for(int i = 0; i < net->count; i++)
		graphcomp(file, net->comps[i]);
	fprintf(file, "}\n");
	fclose(file);
	return 0;
}

void printlists(Network * net){
	Computer * comp;
	Connection * conn;
	for(int i = 0; i < net->count; i++){
		comp = net->comps[i];
		conn = comp->adjacent;
		printf("%s: [", comp->name);
		while(conn != NULL){
			printf("%s", conn->target->name);
			if(conn->next != NULL)
				printf(", ");
			conn = conn->next;
		}
		printf("]\n");
	}
}

int netminimize(Network * net, unsigned int service) {
	if(net->count == 0)
		return 0;
	int id = 0;
	Computer *targ, *comp;
	Connection * conn;
	bool * visited = malloc(sizeof(bool) * net->count);
	bool * todelete = malloc(sizeof(bool) * net->count);
	for (int k = 0; k < net->count; k++){
		visited[k] = false;
		todelete[k] = false;
	}

	for (int i = 0; i < net->count; i++) {
		comp = net->comps[i];
		if (comp->service != service) {
			todelete[i] = true;
			visited[i] = true;
			continue;
		}
		visited[i] = true;
		conn = comp->adjacent;
		while (conn != NULL) {
			targ = conn->target;
			for(int j = 0; j < net->count; j++) {
				if(net->comps[j] == targ) {
					id = j;
					break;
				}
			}
			conn = conn->next;
			if(visited[id])
				deleteconnection(net, comp->name, targ->name);
			else
				visited[id] = true;
		}
	}
	for (int i = net->count - 1; i >= 0; i--)
		if (todelete[i])
			deletecomputer(net, net->comps[i]->name);
	free(todelete);
	free(visited);
	return 0;
}

