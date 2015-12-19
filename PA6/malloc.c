/*
 * CS214 Assignment 6
 *
 * Jonathan Lee (jl1424)
 * Niharika Nagar (nn162)
 *
 * Extra features include leak detector, calloc, and realloc
 * calloc(unsigned int numelements, unsigned int elementsize)
 * realloc(void *ptr, unsigned int size)
 * leakDetector()
 */
 
#include "malloc.h"

/* For malloc stuff */
#define memsize sizeof(struct mementry)
#define blocksize (5000)

/* For cool colors */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"

static char bigblock[blocksize];

/* For leak detection */
int freeCount = 0;
int mallocCount = 0;
int hasLeak = 0;

void *myMalloc(unsigned int size, char *file, int line){
	static int initialized = 0;
	static struct mementry *root=0, *last=0;
	struct mementry *p, *succ;


	if(size == 0){
		printf(ANSI_COLOR_RED "Error: Can't malloc size 0\n" ANSI_COLOR_RESET);
		return 0;
	}

	//checks to see if the block was already attempted to be chopped up
	if(!initialized){
		root = (struct mementry *)bigblock;			
		root->prev = root->succ = 0;
		root->size = blocksize - memsize;
		root->isfree = 1;
		initialized = 1;
	}

	p = root;

	do {
		if(p->size < size)
			p = p->succ; 	 												//block is too small
		else if(!p->isfree)
			p = p->succ;													//block has been allocated
		else if(p->size < (size + memsize)){
			p->isfree = 0;		
			mallocCount++;
														//there's space but too small to chop up
			return (char *)p + memsize;
		}else {
			succ = (struct mementry*)((char*)p + sizeof(struct mementry) + size);
			succ->prev = p;
			succ->succ = p->succ;

			if(p->succ != 0)
				p->succ->prev = succ;
	
			p->succ = succ;
			succ->size = p->size - memsize - size;
			succ->isfree = 1;
			p->size = size;
			p->isfree = 0;
				
			//printf(ANSI_COLOR_CYAN "Successful malloc\n" ANSI_COLOR_RESET);
			mallocCount++;

			return (char *)p + memsize;
		}
	}while(p != 0);



	//just not enough space
	printf(ANSI_COLOR_RED "Error: Unable to allocate %d bytes in" ANSI_COLOR_YELLOW " %s:%d\n" ANSI_COLOR_RESET, size, file, line);

	return 0;
}

void *myCalloc(unsigned int numelements, unsigned int elementsize, char *file, int line){
	int size = numelements * elementsize;
	void *p = malloc(size);

	if(!p)
		return 0;

	p = memset(p, 0, size);		
//	printf(ANSI_COLOR_CYAN "Successful calloc\n" ANSI_COLOR_RESET);

	return 0;
}

int findMyiBlock(void *p, char *file, int line){
	struct mementry *tmp;
	tmp = p;
	int flag = 0;

	tmp = (struct mementry *)bigblock;

	while(tmp != 0){
		char *tmp2 = (char *)tmp;

		if(tmp2 + sizeof(struct mementry) == (char *)p){
			flag = 1;
		}

		tmp = tmp->succ;
	}

	if(flag)
		return 1;
	else
		printf(ANSI_COLOR_RED "Error: Block not found in " ANSI_COLOR_YELLOW "%s:%d\n" ANSI_COLOR_RESET, file, line);

	return 0;
}


void myFree(void *p, char *file, int line){
	//trying to free uninitialized block of memory

	if(!findMyiBlock(p, file, line))
		return;

	if(p == NULL){
		printf(ANSI_COLOR_RED "Error: Uninitialized pointer in" ANSI_COLOR_YELLOW " %s:%d\n" ANSI_COLOR_RESET, file, line);
		return;
	}
	
	struct mementry *ptr, *pred, *succ;

	ptr = (struct mementry *)((char *)p - memsize);

	if(ptr->isfree == 1){
		printf(ANSI_COLOR_RED "Error: Already freed block in " ANSI_COLOR_YELLOW "%s:%d\n" ANSI_COLOR_RESET, file, line);
		return;
	}

	if((pred = ptr->prev) != 0 && pred->isfree){
		//is there a pred at all
		pred->size += memsize + ptr->size;			//size of struct and size you're freeing
		pred->succ = ptr->succ;

		if(ptr->succ != 0)							//if successor is initialized 
			ptr->succ->prev = pred;
	}else {
		ptr->isfree = 1;
		pred = ptr;
	}
	
	if((succ = ptr->succ) != 0 && succ->isfree){
		pred->size += memsize + succ->size;
		pred->succ = succ->succ;

		if(succ->succ != 0)
			succ->succ->prev = pred;
	}
	
	freeCount++;
//	printf("freecount: %d\n", freeCount);
//	printf(ANSI_COLOR_CYAN "Successful free\n" ANSI_COLOR_RESET);
}

void *myRealloc(void *ptr, unsigned int size, char *file, int line){
	char *p;

	if(ptr == NULL){
		printf(ANSI_COLOR_RED "Null Pointer %s:%d\n" ANSI_COLOR_RESET, file, line);
		return malloc(size);
	}

	if(size == 0){
		free(ptr);
		printf(ANSI_COLOR_CYAN "Reallocated size to 0\n" ANSI_COLOR_RESET);

		return NULL;
	}

	p = malloc(size);

	if(p == NULL){
		printf(ANSI_COLOR_RED "Error in malloc %s:%d\n" ANSI_COLOR_RESET, file, line);
		return NULL;
	}

	memcpy(p, ptr, size);

	free(ptr);

//	printf(ANSI_COLOR_CYAN "Successful realloc\n" ANSI_COLOR_RESET);
	return p;
}

void leakCheck(){
	//out to file
	//print in main
	printf("======================LEAK DETECTION=====================\n");
	printf("%d mallocs, %d frees\n", mallocCount, freeCount);
	if(freeCount == mallocCount)
		printf(ANSI_COLOR_CYAN "No memory leaks\n" ANSI_COLOR_RESET);
	else {
		hasLeak = 1;
		if(freeCount > mallocCount)
			printf(ANSI_COLOR_RED "%d too many frees\n" ANSI_COLOR_RESET, (freeCount - mallocCount));
		else if(freeCount < mallocCount)
			printf(ANSI_COLOR_RED "%d too many mallocs\n" ANSI_COLOR_RESET, (mallocCount - freeCount));
	}

	printf("=========================================================\n");
}

int main(int argc, char **argv){
	//test cases
	void *p = (char *)malloc(200);
	free(p + 10);
	void *t = malloc(5);
	free(t);
	/*void *x;
	char d[100];
	free(p + 100);
	free(x);
	free(p);
	free(p);	

	free(&d);
	int n;
	free(&n);

	x = calloc(1, sizeof(int));
	free(x);
	free(x);

	p = realloc(p, 0);
	//printf("%d\n", sizeof(p));
//	free(d);
	free(p);
*/


	char *c = malloc(5); //calloc(10, sizeof(char));

	c = realloc(c, 10);

	atexit(leakCheck);

	return 0;	
}
