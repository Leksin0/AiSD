#ifndef GRAPH_H
#define GRAPH_H

// EVERY STRING MUST HAVE \0
typedef struct Connection Connection;

typedef struct Computer Computer;

typedef struct Path Path;

typedef struct Connection{
	Computer * target;
	size_t count;
	unsigned int * ports;
	unsigned int delay;
	Connection * next; // another connection (different target) for same computer
} Connection;

typedef struct Computer{
	char * name;
	unsigned int service;
	Connection * adjacent; // linked list
} Computer;

typedef struct Network{
	size_t count;
	Computer ** comps;
} Network;

typedef struct Path{
	char * name;
	Path * next;
} Path;

Network * mknet();

void rmnet(Network * net);

int updatecomputer(Network * net, char * name, unsigned int service);

int updateconnection(Network * net, char * nameA, char * nameB, unsigned int delay, size_t count, unsigned int * ports);

int deletecomputer(Network * net, char * name);

int deleteconnection(Network * net, char * nameA, char * nameB);

int dijkstra(Network * net, char * nameA, char * nameB, unsigned int port, Path ** result, int * totaldelay);

int BFS(Network * net, char * start, unsigned int service, char ** result);

int netmininmize(Network * net, unsigned int service);

int graphviz(Network * net);

void printlists(Network * net);
#endif
