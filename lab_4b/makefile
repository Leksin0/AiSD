main: main.c tree.a
	gcc $^ -Wall -Wextra -o $@ -lreadline

tree.a: tree.c tree.h
	gcc -c $< -Wall -Wextra -o tree.o
	ar rcs $@ tree.o

all: main

clean:
	rm -f tree.a tree.o main

.PHONY: all сlean
