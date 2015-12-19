#ifndef QUEUE_H
#define QUEUE_H
#include "bookorder.h"
#include <pthread.h>
typedef struct Queue *Queue;
typedef struct Node *Node;


/* Nodes for Queues holding orders */
struct Node {
	Order order;
	char* value;
	Node next;
	Node prev;
};


/* For consumer queue */
struct Queue {
	Node head;
	Node tail;
	int itemCount;
	int consumerNum;
	char* category;
	pthread_cond_t data;
};


#endif
