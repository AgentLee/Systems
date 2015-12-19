#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sorted-list.h"
/*
*
* CS 214: Systems Programming
* Authors: Niharika Nagar (nn162) and Jon Lee (jl1424)
*
*/


/*
* Big-O Analysis: O(n^2)
*/


SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df){
	SortedListPtr list = (SortedListPtr)malloc(sizeof(struct SortedList));
	list->head = NULL;
	list->cf = cf;
	list->df = df;
	list->numItems = 0;
	return list;
}


NODE* CreateNode(void *newObj){
	NODE *temp = (NODE *)malloc(sizeof(NODE));
	temp->value = newObj;
	temp->next = NULL;
	temp->refCount = 1;
	temp->hasIter = 0;
	temp->isRemoved = 0;

	return temp;
}

int SLInsert(SortedListPtr list, void* newObj){
	NODE *newNode = CreateNode(newObj);
	NODE *ptr;
	NODE *prev;
	ptr = list->head;
	int compare;

	if(list->head == NULL){
		list->head = newNode;
		list->numItems++;
		return 1;
	}
	
	while(ptr != NULL){
		compare = list->cf(ptr->value, newObj);

		if(compare < 0){
			newNode->value = ptr->value;
			ptr->value = newObj;	
			newNode->next = ptr->next;
			ptr->next = newNode;
			list->numItems++;

			return 1;
		}else if(compare == 0){
			printf("ERROR\n");
			return 0;
		}else if(compare > 0){
			prev = ptr;
			ptr = ptr->next;
			list->numItems++;
		}
	}
	prev->next = newNode;
	return 1;
}

int SLRemove(SortedListPtr list, void *newObj){
	NODE *temp;
	NODE *curr = list->head;
	NODE *prev = curr;
	if(curr->value == newObj){
		temp = list->head->next;
		prev =list->head;
		prev->isRemoved = 1;
		prev->refCount--;
		list->head = temp;
		if(list->head->next != NULL){
			list->head->next->refCount++;
		}
		return 1;
	}else {
		curr = curr->next;
	}
	while(curr != NULL){
		if(curr->value == newObj){
			prev->next = curr->next;
			curr->refCount--;
			curr->isRemoved = 1;

		if(curr->next != NULL){
			curr->next->refCount++;
		}

			return 1;
		}else {
			prev = curr;
			curr = curr->next;
		}
	}	
	return 0;
}

SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
	SortedListIteratorPtr iter = (SortedListIteratorPtr)malloc(sizeof(struct SortedListIterator));
	iter->list = list;
	iter->prev = 0;
	if(list->head != NULL){
		iter->curr = list->head;
		iter->curr->refCount++;
		iter->curr->hasIter++;
	}
	return iter;
}

void * SLGetItem( SortedListIteratorPtr iter ){
	NODE *ptr = iter->curr;

	if(ptr->value != NULL){
		return ptr->value;
	}else {
		return NULL;
	}
}

void * SLNextItem(SortedListIteratorPtr iter){
	NODE *temp = NULL;
		if(iter == NULL){
		return temp;
	}
	else if(iter->curr->next == NULL) 
		return temp;
	else {
		while(iter->curr->isRemoved == 1){
			temp = iter->curr;
			iter->curr = iter->curr->next;
			iter->prev = temp;

			iter->prev->refCount--;
			iter->curr->refCount++;

			return iter->curr;
		}

		temp = iter->curr;
		iter->curr = iter->curr->next;
		iter->prev = temp;

		iter->prev->refCount--;
		iter->curr->refCount++;

		return iter->curr;
	}
}

void SLDestroy(SortedListPtr list){
	NODE *curr = list->head;
	NODE *temp;

	while(curr != NULL){
		temp = curr->next;
		list->df(curr->value);
		free(curr);
		list->numItems--;
		curr = temp;
	}
	free(list);
	return;
}

void SLDestroyIterator(SortedListIteratorPtr iter){
	free(iter);
	return;
}

void DestroyNode(SortedListPtr list, NODE *ptr){
	if(ptr == NULL){
		return;
	}

	list->df(ptr->value);
	free(ptr);
	return;	
}
