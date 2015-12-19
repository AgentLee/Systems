#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "bookorder.h"
#include "queue.h"
#include "customers.h"

#define BUFFER 2048

Node* createNode(Order order){
	Node* newNode = (Node*) malloc(sizeof(Node));
	newNode->order = order;
	newNode->next = NULL;
	newNode->prev = NULL;
	return newNode;
}

/* add Order to queue */
void enqueue(Order order, Queue queue){
	Node* newNode = createNode(order);

	printf("order: %s\n", order->category);

	if(!queue->head){
	
		queue->head = newNode;
		queue->tail = newNode;
		queue->itemCount=0;
	
	} else if(queue->head == queue->tail){
		
		queue->head->next = newNode;
		newNode->prev = queue->head;
		queue->head = newNode;

	} else { /* queue->head != NULL*/
	
		newNode->prev = queue->head;
		queue->head->next = newNode;
		queue->head = newNode;
	
	} 
	
	printf("adding itemcount to %s\n", queue->category);
	queue->itemCount++;
	printf("%d\n", queue->itemCount++);

}

/* returns the next order in the queue and removes it from the queue */
Order nextOrder(Queue queue){
	if(!queue) 
		return NULL;
	if(queue->itemCount == 0) 
		return NULL;

	Node* oldOrder = queue->tail;
	
	if(!queue->tail->next) 
		return NULL;

	Order next = queue->tail->next->order;

	if(queue->tail != queue->head) 
		queue->tail = queue->tail->next;

	queue->tail->prev = NULL;

	queue->itemCount--;

	if(queue->itemCount==0){
		queue->head = NULL;
		queue->tail = NULL;
	}

	free(oldOrder);			
	return next;

}


/* free all elements of Queue - not properly freeing */
void killQueue(Queue queue){
	if(!queue) return;

	//printf("Killing Queue\n--------------------\n");

	//Node* head = queue->head;
	//Node* tail = queue->tail;
	Node* temp;
	Node* curr;

	int i;
	for(i=0, curr=queue->tail; i < queue->itemCount && curr; i++, curr=curr->next, free(temp),queue->itemCount--){

		printf("%d. Order: %s\n\tCategory: %s\n\tPrice: $%.2f\n\n", 
			curr->order->customerID,
			curr->order->title ,
			curr->order->category, 
			curr->order->price);
		temp = curr;
		if (temp->order) {
			//free(temp->order->title);
			//free(temp->order->category);
			free(temp->order);
		}
    
	}

	free(queue);


}

/* Prints the Order Queue */
void printQueue(Queue queue){
	if(!queue) return;
	if(queue->itemCount==0) return;

	printf("\nPrinting Queue\n--------------------\n");

	Node* curr;
	int i;
	for(i=0, curr=queue->tail; i < queue->itemCount && curr; i++, curr=curr->next){
		if(!curr->order->title||!curr->order->category||!curr->order->price||!curr->order->customerID) continue;
		printf("Customer %d - Order: %s\n\tCategory: %s\n\tPrice: $%.2f\n\n", curr->order->customerID,curr->order->title ,curr->order->category, curr->order->price);
		
	}

}

/* Initializes the Orders */
Order createOrder(char* title, char* category, double price, int customerID){
//	printf("%s\t%s\t%f\t%d\n", title, category, price, customerID);
	
	Order newOrder = (Order)malloc(sizeof(struct Order));
	newOrder->title = (char*)malloc(sizeof(char)*strlen(title)+1);
	newOrder->category = (char*)malloc(sizeof(char)*strlen(category)+1);
	strcpy(newOrder->title, title);
	strcpy(newOrder->category, category);
	newOrder->price = price; 
	newOrder->customerID = customerID;
	return newOrder;
}

void destroyOrder(Order order){

	if(order->title) free(order->title);
	if(order->category) free(order->category);
	if(order) free(order);

	return;
}

#ifdef QUEUE_C
int main(int argc, char** argv){
	Order orderA = createOrder("OrderA", "HOUSING01", 123443.99, 1);
	Order orderB = createOrder("OrderB", "SPORTS01", 1.99, 2);
	Order orderC = createOrder("OrderC", "POLITICS01", 2.99, 3);
	Queue queue = (Queue)malloc(sizeof(struct Queue));

	enqueue(orderA, queue);
	enqueue(orderB, queue);
	enqueue(orderC, queue);
	printQueue(queue);
	printf("%d\n", queue->itemCount);
	Order next = nextOrder(queue);
	if(next)  printf("Next Order is:\n %d. Order: %s\n\tCategory: %s\n\tPrice: $%.2f\n\n",  next->customerID, next->title, next->category, next->price);

	next = nextOrder(queue);
	if(next) printf("Next Order is:\n %d. Order: %s\n\tCategory: %s\n\tPrice: $%.2f\n\n",  next->customerID, next->title, next->category, next->price);

	next = nextOrder(queue);
	if(next) printf("Next Order is:\n %d. Order: %s\n\tCategory: %s\n\tPrice: $%.2f\n\n",  next->customerID, next->title, next->category, next->price);

	//killQueue(queue);

	return 0; 
}
#endif
