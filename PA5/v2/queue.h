#ifndef QUEUE_H
#define QUEUE_H

struct Node {
	Order order;
	char* value;
	struct Node* next;
	struct Node* prev;
};

typedef struct Node Node;

struct Queue {
	struct Node* head;
	struct Node* tail;
	int itemCount;
	char* category;
	pthread_cond_t hasData;
	int queueID;
};

typedef struct Queue* Queue;


Order createOrder(char* title, char* category, double price, int customerID);
void printQueue(Queue queue);
void killQueue(Queue queue);
Order nextOrder(Queue queue);
void enqueue(Order order, Queue queue);
Node* createNode(Order order);


#endif
