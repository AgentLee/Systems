#include "bookorder.h"
#include "queue.h"


Node* createNode(Order* order){
	Node* newNode = (Node*) malloc(sizeof(Node));
	newNode->order = order;
	newNode->next = NULL;
	newNode->prev = NULL;
	return newNode;
}


/* add Order to queue */
void addToQueue(Order* order, Queue* queue){
	Node* newNode = createNode(order);

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
	queue->itemCount++;

}


/* returns the next order in the queue and removes it from the queue */
Order* nextOrder(Queue* queue){
	if(!queue) return NULL;
	if(queue->itemCount == 0) return NULL;

	Node* oldOrder = queue->tail;
	Order* next = queue->tail->order;

	if(queue->tail != queue->head) queue->tail = queue->tail->next;

	queue->tail->prev = NULL;

	queue->itemCount--;

	if(queue->itemCount==0){
		queue->head = NULL;
		queue->tail = NULL;
	}

	free(oldOrder);
	return next;

}


/* free all elements of Queue*/
void destroyQueue(Queue* queue)
{	
	if (queue->head == NULL){
		free(queue);
		return;
	} 
	
	Node* curr; 
	Node* temp;

	curr = queue->head;
	while (curr != NULL)
	{
		temp = curr;
		curr = curr->prev;
		if (temp->order != NULL)
		{
			free(temp->order->title);
			free(temp->order->category);
			free(temp->order);
		}
		free(temp);
	}

	free(queue);
}
