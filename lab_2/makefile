# LIB = "list" OR "vec" TYPE = "dyn" OR "stat"
LIB = dflt
TYPE = dflt
all: main

ifeq ($(LIB), vec)
    LIB_SRC = queue_vec.c
else ifeq ($(LIB), list)
    LIB_SRC = queue_list.c
else
    $(error Invalid lib flag. Use LIB=list or LIB=vec)
endif

ifeq ($(TYPE), stat)
queue.a: $(LIB_SRC)
	gcc -c $< -o queue.o
	ar rcs $@ queue.o

main: main_stat.c auxiliary.c queue.a
	gcc $^ -lreadline -o $@

else ifeq ($(TYPE), dyn)
queue.dylib: $(LIB_SRC)
	gcc -fPIC -dynamiclib $< -o $@

main: auxiliary.c main_dyn.c queue.dylib
	gcc $^ -ldl -lreadline -o $@
else
    $(error Invalid type flag. Use TYPE=dyn or TYPE=stat)
endif

clean:
	rm -f *.o *.a *.dylib main

.PHONY: all clean
