#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define malloc(x) myMalloc(x, __FILE__, __LINE__)
#define minalloc(x) minMalloc(x, __FILE__, __LINE__)
#define calloc(x, y) myCalloc(x, y, __FILE__, __LINE__)
#define free(x) myFree(x, __FILE__, __LINE__)
#define realloc(x, y) myRealloc(x, y, __FILE__, __LINE__)

struct mementry {
	struct mementry *prev, *succ;
	int isfree;
	int size;	
};

#endif
