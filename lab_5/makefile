main: main.c graph.a
	gcc $^ -Wall -Wextra -o $@ -lreadline
	
graph.a: graph.c graph.h
	gcc -c $< -Wall -Wextra -o graph.o
	ar rcs $@ graph.o

all: main

clean:
	rm -f graph.a graph.o main

.PHONY: all сlean
