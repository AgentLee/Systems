/*
 * sorted-list.c
 */

/*
 * Jonathan Lee (jl1424) and Niharika Nagar (nn162)
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


int main(int argc, char **argv)
{
	int choice = atoi(argv[1]);
	int x;
	double y;
	char* z;

	if(choice == 1){
		printf("*****INTEGERS*****\n");
		SortedListPtr list = SLCreate(compareInts, destroyBasicTypeNoAlloc);
		NODE *ptr;

		int a = 15;
		int b = 15;
		int c = 13985932;
		int d = 18;
		int e = 188;
		int f = 43;
		int g = 10;
		int h = 2;
		int i = 221;

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
			printf("List item: %d\n",*((int*)ptr->value));
			ptr= ptr->next;
		}

		SLRemove(list, &a);
		SLRemove(list, &c);
		SLRemove(list, &i);

		x = SLGetItem(iter);
		SLNextItem(iter);
		printf("get item: %d\n",*((int*)x));
		x = SLGetItem(iter);
		printf("get item: %d\n",*((int*)x));

		SLDestroy(list);
		SLDestroyIterator(iter);
	}else if(choice == 2){
		printf("*****STRINGS*****\n");
		SortedListPtr list = SLCreate(compareStrings, destroyBasicTypeNoAlloc);
		NODE *ptr;
	    
		char *q = (char *)malloc(sizeof(char));
		char *r = (char *)malloc(sizeof(char));
		char *s = (char *)malloc(sizeof(char));
		char *t = (char *)malloc(sizeof(char));
		char *u = (char *)malloc(sizeof(char));

	    q = "banana";
	    r = "zucchini";
	    s = "apple";
	    t = "orange";
	    u = "grapefruit";

		SLInsert(list, q);
		SLInsert(list, r);
		SLInsert(list, s);
		SLInsert(list, t);
		SLInsert(list, u);
		SortedListIteratorPtr iter = SLCreateIterator(list);

		ptr = list->head;
		while(ptr != NULL){
			printf("List item: %s\n",*((char*)ptr->value);
			ptr= ptr->next;
		}

		SLRemove(list, q);
	    SLRemove(list, s);
	    SLRemove(list, u);

	    ptr = list->head;
		while(ptr != NULL){
			printf("List item: %s\n", *((char*)ptr->value);
			ptr= ptr->next;
		}

		z = SLGetItem(iter);
		SLNextItem(iter);
		printf("get item: %s\n",*((char*)z));
		z = SLGetItem(iter);
		printf("get item: %s\n",*((char*)z));

		SLDestroy(list);
		SLDestroyIterator(iter);
	}else if(choice == 3){
		SortedListPtr list = SLCreate(compareStrings, destroyBasicTypeNoAlloc);
		NODE *ptr;

		printf("*****DOUBLES*****\n");

		list = SLCreate(compareDoubles, destroyBasicTypeNoAlloc);
		
		double j = 214;
		double k = 73;
		double l = 8999;
		double m = 9000;
		double n = 113;

		SLInsert(list, &j);
		SLInsert(list, &k);
		SLInsert(list, &l);
		SLInsert(list, &m);
		SLInsert(list, &n);
		SortedListIteratorPtr iter = SLCreateIterator(list);

		ptr = list->head;
		while(ptr != NULL){
			printf("List item: %d\n",*((int*)ptr->value));
			ptr= ptr->next;
		}

		SLRemove(list, &m);
		SLRemove(list, &j);

		y =SLGetItem(iter);
		SLNextItem(iter);
		printf("get item: %d\n",*((int*)y));
		y =SLGetItem(iter);
		printf("get item: %d\n",*((int*)y));

		SLDestroy(list);
		SLDestroyIterator(iter);
	}else {
		printf("1 for Integers\n2 for Strings\n3 for Doubles\n");
	}

	return 0;	
}

