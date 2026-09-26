#include <stdio.h>
#include <time.h>
#include "tree.c"

int main(){
	Tree * tree;
	Node * res;
	clock_t start, end;
	double cputime;
		
	tree = mktree();
	importfile(tree, "t1k");
	start = clock();
	specsearch(tree, 854, &res);
	end = clock();
	cputime = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time taken for 1000: %lf\n", cputime);
	rmtree(tree);

	tree = mktree();
	importfile(tree, "t2k");
	start = clock();
	specsearch(tree, 854, &res);
	end = clock();
	cputime = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time taken for 2000: %lf\n", cputime);
	rmtree(tree);

	tree = mktree();
	importfile(tree, "t3k");
	start = clock();
	specsearch(tree, 854, &res);
	end = clock();
	cputime = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time taken for 3000: %lf\n", cputime);
	rmtree(tree);

	tree = mktree();
	importfile(tree, "t4k");
	start = clock();
	specsearch(tree, 854, &res);
	end = clock();
	cputime = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time taken for 4000: %lf\n", cputime);
	rmtree(tree);

	return 0;
}
