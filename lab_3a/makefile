main: main.c table.a
	gcc $^ -Wall -Wextra -o $@ -lreadline
	
table.a: table.c table.h
	gcc -c $< -o table.o
	ar rcs $@ table.o

all: main

clean:
	rm -f table.a table.o main

.PHONY: all сlean
