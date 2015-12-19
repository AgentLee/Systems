#ifndef QUEUE_H
#define QUEUE_H
#include <pthread.h>
#include "order.h"

struct Node{
	struct Node *front;
	struct Node *back;
	Order order;
};
typedef struct Node *Node;

struct Queue{
	struct Node *head;
	struct Node *tail;
	int itemCount;
	int queueID;
	char *category;
	pthread_cond_t hasData;
};
typedef struct Queue *Queue;

#endif
