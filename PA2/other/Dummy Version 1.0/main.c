/*
 * sorted-list.c
 */

#include	<string.h>
#include	"sorted-list.h"

int compareInts(void *p1, void *p2)
{
	int i1 = *(int*)p1;
	int i2 = *(int*)p2;

	return i1 - i2;
}

int compareDoubles(void *p1, void *p2)
{
	double d1 = *(double*)p1;
	double d2 = *(double*)p2;

	return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2)
{
	char *s1 = p1;
	char *s2 = p2;

	return strcmp(s1, s2);
}

//Destructor functions
void destroyBasicTypeAlloc(void *p){
	//For pointers to basic data types (int*,char*,double*,...)
	//Use for allocated memory (malloc,calloc,etc.)
	free(p);
}

void destroyBasicTypeNoAlloc(void *p) {
	//For pointers to basic data types (int*,char*,double*,...)
	//Use for memory that has not been allocated (e.g., "int x = 5;SLInsert(mylist,&x);SLRemove(mylist,&x);")
	return;
}


int main()
{
	int compare;
	int *x;
	SortedListPtr list = SLCreate(compareInts, destroyBasicTypeNoAlloc);
	NODE *ptr;
	NODE *ptr2;

	/*
	double *a = malloc(sizeof(double));
	double *b = malloc(sizeof(double));
	double *c = malloc(sizeof(double));
	double *d = malloc(sizeof(double));
	double *e = malloc(sizeof(double));
	double *f = malloc(sizeof(double));
	double *g = malloc(sizeof(double));
	double *h = malloc(sizeof(double));
	double *i = malloc(sizeof(double));
	*a = 15;
	*b = 15;
	*c = 13985932;
	*d = 18;
	*e = 188;
	*f = 43;
	*h = 2;
	*i = 221;
	SLInsert(list, a);
	SLInsert(list, b);
	SLInsert(list, c);
	SLInsert(list, d);
	SLInsert(list, e);
	SLInsert(list, f);
	SLInsert(list, g);
	SLInsert(list, h);
	SLInsert(list, i);
	char *x = malloc(sizeof(char*));
	char *y = malloc(sizeof(char*));
	char *z = malloc(sizeof(char*));
	x = "sladkjf;aklsdjfklsd";
	y = "c";
	z = "a";
	SLInsert(list, x);
	SLInsert(list, y);
	SLInsert(list, z);
	*/

	/*int *a = malloc(sizeof(int));
	int *b = malloc(sizeof(int));
	int *c = malloc(sizeof(int));
	int *d = malloc(sizeof(int));
	
	int *a = 15;
	int *b = 15;
	int *c = 13985932;
	int *d = 18;
	int *e = 188;
	int *f = 43;
	int *g = 10;
	int *h = 2;
	int *i = 221;

	SLInsert(list, &a);
	SLInsert(list, &b);
	SLInsert(list, &c);
	SLInsert(list, &d);
	SLInsert(list, &e);
	SLInsert(list, &f);
	SLInsert(list, &g);
	SLInsert(list, &h);
	SLInsert(list, &i);
	SortedListIteratorPtr iter = SLCreateIterator(list);

	ptr = list->head;
	while(ptr != NULL){
		printf("The list contains: %d\n",*((int*)ptr->value));
		printf("Has Iterator: %d\n", ptr->hasIter);
		printf("RefCount for %d: %d\n\n",*((int*)ptr->value), ptr->refCount);
		ptr= ptr->next;
	}
printf("*********************\n\n");
	SLRemove(list, &a);
	SLRemove(list, &c);
	SLRemove(list, &i);
	ptr = list->head;
	while(ptr != NULL){
		printf("The list contains: %d\n",*((int*)ptr->value));
		printf("Has Iterator: %d\n", ptr->hasIter);
		printf("RefCount for %d: %d\n\n",*((int*)ptr->value), ptr->refCount);
		ptr= ptr->next;
	}

	x = SLGetItem(iter);
	SLNextItem(iter);

	printf("get item: %d\n",*((int*)x));

	x = SLGetItem(iter);
		printf("next item: %d\n",*((int*)x));
*/

		SLDestroy(list);

		list = SLCreate(compareDoubles, destroyBasicTypeNoAlloc);

		double *a = 214;
		double *b = 73;
		double *c = 8999;
		double *d = 9000;
		double *e = 113;

		SLInsert(list, &a);
		SLInsert(list, &b);
		SLInsert(list, &c);
		SLInsert(list, &d);
		SLInsert(list, &e);

		SortedListIteratorPtr iter = SLCreateIterator(list);

	ptr = list->head;
	while(ptr != NULL){
		printf("The list contains: %d\n",*((int*)ptr->value));
		printf("Has Iterator: %d\n", ptr->hasIter);
		printf("RefCount for %d: %d\n\n",*((int*)ptr->value), ptr->refCount);
		ptr= ptr->next;
	}

	SLRemove(list, d);
	SLRemove(list, a);

	ptr = list->head;
	while(ptr != NULL){
		printf("The list contains: %d\n",*((int*)ptr->value));
		printf("Has Iterator: %d\n", ptr->hasIter);
		printf("RefCount for %d: %d\n\n",*((int*)ptr->value), ptr->refCount);
		ptr= ptr->next;
	}

	x = SLGetItem(iter);
	SLNextItem(iter);
	printf("get item: %f\n",*((double*)x));

	x = SLGetItem(iter);
	printf("get item: %f\n",*((double*)x));



/*
	ptr = list->head;
	printf("******************Initial: \n");
	while(ptr != NULL){
		printf("The list contains: %d\n",*((int*)ptr->value));
		printf("Has Iterator: %d\n", ptr->hasIter);
		printf("RefCount for %d: %d\n\n",*((int*)ptr->value), ptr->refCount);
		ptr= ptr->next;
	}
	*/

	SLDestroy(list);
	SLDestroyIterator(iter);

/************************************************print head*************************
	
	SLRemove(list, d);
	x = SLGetItem(iter);
	printf("SLGetItem for %d\n\n",*((int*)x));

	printf("**********************************\n");

****************************************************************************/
/*
	ptr = list->head;
	printf("******************Initial: \n");
	while(ptr != NULL){
		printf("The list contains: %d\n",*((int*)ptr->value));
		printf("Has Iterator: %d\n", ptr->hasIter);
		printf("RefCount for %d: %d\n\n",*((int*)ptr->value), ptr->refCount);
		ptr= ptr->next;
	}

	ptr2 = SLNextItem(iter);
	printf("SLNextItem: \n");
	if(ptr2 != NULL){
		printf("Has Iterator: %d\n", ptr2->hasIter);
		printf("RefCount for %d: %d\n\n",*((int*)ptr2->value), ptr2->refCount);
	}*/
/**************REMOVE C**************************/
	/*SLRemove(list, c);

	ptr = list->head;
	printf("******************Remove 3: \n");
	while(ptr != NULL){
		printf("The list contains: %d\n",*((int*)ptr->value));
		printf("Has Iterator: %d\n", ptr->hasIter);
		printf("RefCount for %d: %d\n\n",*((int*)ptr->value), ptr->refCount);
		ptr= ptr->next;
	}

	printf("SLNextItem after remove 3: \n");
	if(ptr2 != NULL){
		printf("Has Iterator: %d\n", ptr2->hasIter);
		printf("RefCount for %d: %d\n\n",*((int*)ptr2->value), ptr2->refCount);
	}

	printf("*****************iter stuff: \n");
	ptr = ptr2;
	while(ptr != NULL){
		printf("The list contains: %d\n",*((int*)ptr->value));
		printf("Has Iterator: %d\n", ptr->hasIter);
		printf("RefCount for %d: %d\n\n",*((int*)ptr->value), ptr->refCount);
		ptr= ptr->next;
	}

	x = SLGetItem(iter);
	printf("SLGetItem for %d\n\n",*((int*)x));*/

/**************REMOVE B**************************/
/*	SLRemove(list, b);

	ptr = list->head;
	printf("******************Remove 2: \n");
	while(ptr != NULL){
		printf("The list contains: %d\n",*((int*)ptr->value));
		printf("Has Iterator: %d\n", ptr->hasIter);
		printf("RefCount for %d: %d\n\n",*((int*)ptr->value), ptr->refCount);
		ptr= ptr->next;
	}

	printf("*****************iter stuff: \n");
	ptr = ptr2;
	while(ptr != NULL){
		printf("The list contains: %d\n",*((int*)ptr->value));
		printf("Has Iterator: %d\n", ptr->hasIter);
		printf("RefCount for %d: %d\n\n",*((int*)ptr->value), ptr->refCount);
		ptr= ptr->next;
	}

	x = SLGetItem(iter);
	printf("SLGetItem for %d\n\n",*((int*)x));

*/




	/*
	x = SLGetItem(iter);
	printf("SLGetItem = %d\n", *((int*)x));
	ptr2 = SLNextItem(iter);
	printf("ptr2 = %d\n", *((int*)ptr2->value));
	printf("RefCount for %d: %d\n\n",*((int*)ptr2->value), ptr2->refCount);

	x = SLGetItem(iter);
	printf("SLGetItem = %d\n", *((int*)x));
	ptr2 = SLNextItem(iter);
	printf("ptr2 = %d\n", *((int*)ptr2->value));
	printf("RefCount for %d: %d\n\n",*((int*)ptr2->value), ptr2->refCount);

	x = SLGetItem(iter);
	printf("SLGetItem = %d\n", *((int*)x));
	ptr2 = SLNextItem(iter);
	printf("ptr2 = %d\n", *((int*)ptr2->value));
	printf("RefCount for %d: %d\n\n",*((int*)ptr2->value), ptr2->refCount);

	x = SLGetItem(iter);
	printf("SLGetItem = %d\n", *((int*)x));
	ptr2 = SLNextItem(iter);
	
	if(ptr2 != NULL){
		printf("RefCount for %d: %d\n\n",*((int*)ptr2->value), ptr2->refCount);
		printf("ptr2 = %d\n", *((int*)ptr2->value));
	}
	
	ptr = list->head;
	while(ptr != NULL){
		printf("The list contains: %d\n",*((int*)ptr->value));
		printf("RefCount for %d: %d\n\n",*((int*)ptr->value), ptr->refCount);
		ptr= ptr->next;
	}
	*/
	/*
	 * i, 	e, 	 f,  h
	 * 221, 188, 43, 2
	 */


	/*
	 * c, i, e, f, d, a
	 */

/*
	SLRemove(list, e);
	SLRemove(list, f);

	ptr2 = SLNextItem(iter);
	printf("ptr2 = %d\n\n", *((int*)ptr2->value));
	printf("RefCount for %d: %d\n\n",*((int*)ptr2->value), ptr2->refCount);
	ptr2 = SLNextItem(iter);
	printf("ptr2 = %d\n\n", *((int*)ptr2->value));
	printf("RefCount for %d: %d\n\n",*((int*)ptr2->value), ptr2->refCount);

	ptr = list->head;
	while(ptr != NULL){
		printf("The list contains: %d\n",*((int*)ptr->value));
		printf("RefCount for %d: %d\n\n",*((int*)ptr->value), ptr->refCount);
		ptr= ptr->next;
	}
	*/
/*
	SLDestroyIterator(iter);
	SLDestroy(list);
*/
	return 0;	
}

