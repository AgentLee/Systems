#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include "bookorder.h"
#include "queue.h"
#include "customers.h"

/* GLOBAL VARIABLES */
Customers *customers;
Queue orderQueue;
Queue *categoryQueue;
int numCategories;

// USE THIS AS A BOOLEAN - 0 for NO/1 for YES
int isFinished = 0;

/* GLOBAL THREADS */
pthread_mutex_t lock;
pthread_cond_t hasSpace;
pthread_mutex_t bkrPrints;
// PRODUCER THREAD
pthread_t producerThread;
// CONSUMER THREAD
pthread_t *consumerThreads;

int maxqueue = 3;

int isFile(char *fileName){
        struct stat s;
        if(stat(fileName, &s) == 0){ 
                if(s.st_mode &S_IFREG){
                        return 1;
                }else {
                        return 0;
                }   
        }   
        return 0;       
}


/* Prints customer information and filess the CustomerArray: 
 * name, customerID, address, zip, state, remainingCredit  */
void infoFiller(char *customerInfo){
        Customers newCustomer = malloc(sizeof(*newCustomer));
        
        char *name = strtok(customerInfo, "\"");
        int id = atoi(strtok(NULL, "| "));
        double credit = atof(strtok(NULL, "| "));
        char *address = strtok(NULL, "\"");
        char *state = strtok(NULL, "\"|");
        char *zip = strtok(NULL, "\"|");
        /*
        printf("--------------- CUSTOMER ---------------\n");
        printf("Name: %s\n", name);    
        printf("Customer ID: %d\n", id);    
        printf("Credit: $%.2f\n", credit);    
        printf("Address: %s\n", address);    
        printf("State: %s\n", state);    
        printf("Zip: %s\n", zip);    
        printf("----------------------------------------\n\n");
        */
        newCustomer->name = (char *)malloc(sizeof(char) * strlen(name) + 1); 
        newCustomer->address = (char *)malloc(sizeof(char) * strlen(address) + 1); 
        newCustomer->zip = (char *)malloc(sizeof(char) * strlen(zip) + 1); 
        newCustomer->st = (char *)malloc(sizeof(char) * strlen(state) + 1); 
        strcpy(newCustomer->name, name);
        strcpy(newCustomer->address, address);
        strcpy(newCustomer->zip, zip);
        strcpy(newCustomer->st, state);
        newCustomer->customerID = id; 
        newCustomer->remainingCredit = credit;
        customers[id] = newCustomer;
        return;
}

/* reads the database file line by line */
void readDatabase(FILE* database){
        if(database==NULL){
                fprintf(stderr, "ERROR: could not read database\n");
        }   
        //100 customers?
        customers = malloc(sizeof(Customers) * 100);
    
        //holds information from each line of file
        char person[100];
        int i;
        //intializes the customers
        for(i = 0; i < 100; i++)
                customers[i] = NULL;
    
        
        /*  fills customers with information, person holds all the info from each line
         *  pass person into infoFiller to add to customers */
        while(fgets(person, 100, database) != NULL)
                infoFiller(person);
        return;
}

/* reads the order file line by line */
void readOrders(FILE *orders){
        if(orders == NULL)
                fprintf(stderr, "ERROR: could not read order\n");
           
        char* title;
        char* category;
        double price;
        int custID;
        Order newOrder; //= malloc(sizeof(*newOrder));;
        
        char order[200];
        while(fgets(order, 200, orders)) {  
                
                //printf("%s\n", order);
                title = strtok(order, "|");
                price = atof(strtok(NULL,"|\""));
                custID = atoi(strtok(NULL,"|\""));
                category = strtok(NULL, "|\"\r\n ");
                //printf("Category: %s\n", category);

    	        newOrder = createOrder(title, category, price, custID);
                
                printf("---------------- ORDER -----------------\n");
                printf("Title: %s\n", newOrder->title);
                printf("Category: %s\n", newOrder->category);
                printf("Price: $%.2f\n", newOrder->price);
                printf("Title: %d\n", newOrder->customerID);
                printf("----------------------------------------\n\n");
               
                enqueue(newOrder, orderQueue);
       }

        //printQueue(orderQueue);


      return;
}

Order orderParse(char *order){
	char *title;
	char *category;
	double price;
	int custID;

	title = strtok(order, "|");
	price = atof(strtok(NULL, "|\""));
	custID = atoi(strtok(NULL, "|\""));
	category = strtok(NULL, "|\"\r\n ");

	Order newOrder = malloc(sizeof(*newOrder));
	newOrder->title = (char *)malloc(sizeof(char) * strlen(title));
	newOrder->category = (char *)malloc(sizeof(char) * strlen(category));
	newOrder->customerID = custID;
	newOrder->price = price;

	//printf("%s\n", newOrder->title);


	return newOrder;
}


//void enqueue(Order order, Queue queue){
//	printf("$$$$$$\t%s\n", queue->category);

/*        Node* newNode = createNode(order);

	printf("enqueuing %s\n", order->category);

        if(!queue->head){
    
                queue->head = newNode;
                queue->tail = newNode;
                queue->itemCount=0;
    
        } else if(queue->head == queue->tail){
    
                queue->head->next = newNode;
                newNode->prev = queue->head;
                queue->head = newNode;

        } else { *//* queue->head != NULL*/
/*    
                newNode->prev = queue->head;
                queue->head->next = newNode;
                queue->head = newNode;
    
        }   
    
        queue->itemCount++;
*/
//}

void printstuff(Queue queue){
	printf("^^^^^^\t%s\n", queue->category);
}


/* need to write threading functions for initializing, producer and consumer */

void *producer(void *ordqueue){
	Queue horders = (Queue)ordqueue;
	Node *ptr = horders->tail;
	int i;
	int wasWaiting = 0;

	Order newOrder = ptr->order;


	printf("****\t%s\n", categoryQueue[0]->category);
	printstuff(categoryQueue[0]);
	//enqueue(newOrder, categoryQueue[0]);	
	printf("*****\t%s\n", newOrder->category);

	while(ptr != NULL){
		
		for(i = 0; i < numCategories; i++){
				
			pthread_mutex_lock(&lock);

			while(categoryQueue[i]->itemCount == maxqueue){

				pthread_cond_signal(&categoryQueue[i]->hasData);
				pthread_mutex_lock(&bkrPrints);
				
				if(wasWaiting == 0)
					printf("Producer waits when queue[%d] is full\n", i);

				pthread_mutex_unlock(&bkrPrints);
				pthread_cond_wait(&hasSpace, &lock);
				
				wasWaiting = 1;
	
			}

			if(wasWaiting == 1){

				printf("Producer resumes when queues have avaialable space\n");
				wasWaiting = 0;

			}

			if(strcmp(categoryQueue[i]->category, ptr->order->category) == 0)
				//enqueue(ptr->order, categoryQueue[i]);
				printf("cat: %s\n", categoryQueue[i]->category);

			pthread_cond_signal(&categoryQueue[i]->hasData);
			pthread_mutex_unlock(&lock);

		}		
	
		ptr = ptr->next;

	}

	pthread_mutex_lock(&lock);
	isFinished = 1;
	for(i = 0; i < numCategories; i++){
		pthread_cond_signal(&categoryQueue[i]->hasData);
	}
	pthread_mutex_unlock(&lock);


/*
    // horders is a temp for the queue that is passed in
    Queue horders = (Queue) ordQueue;
    // ptr gets you to the data throughout horders
    Node *ptr = horders->tail;
    // I don't always use a counter but when I do, I choose i.
    int i;
    //0 = NO/1 = YES
    int waiting = 0;

    printf("producer\n");

	while(ptr != NULL){
		printf("==================================\n");
		printf("Title: %s\n", ptr->order->title);
		printf("Category: %s\n", ptr->order->category);
		printf("Price: %f\n", ptr->order->price);
		printf("CustomerID: %d\n", ptr->order->customerID);
		printf("Customer Name: %s\n", customers[ptr->order->customerID]->name);


		for(i = 0; i < numCategories; i++){
			pthread_mutex_lock(&lock);

			while(categoryQueue[i]->itemCount == maxqueue){
				pthread_cond_signal(&categoryQueue[i]->hasData);
				pthread_mutex_lock(&bkrPrints);
				if(waiting == 0){
					printf("%s %d\t", categoryQueue[i]->category, categoryQueue[i]->itemCount);
					printf("Producer has a full queue\n");
				}
				pthread_mutex_unlock(&bkrPrints);
				waiting = 1;
			}				



			if(strcmp(categoryQueue[i]->category, ptr->order->category) == 0){
				enqueue(ptr->order, categoryQueue[i]);
				//printf("match\n");			
				//printf("category queue itemCount: %d\n", categoryQueue[i]->itemCount);
		
			}
			
			pthread_cond_signal(&categoryQueue[i]->hasData);
			pthread_mutex_unlock(&lock);
		}
		ptr = ptr->next;
	}	


	pthread_mutex_lock(&lock);
	isFinished = 1;
	
	for(i = 0; i < numCategories; i++){
		pthread_cond_signal(&categoryQueue[i]->hasData);
	}
	
	pthread_mutex_unlock(&lock);
*/
    return NULL;
}

void *consumer(void *catQueue){




/*    Order order = (struct Order *)malloc(sizeof(struct Order));
    Customers customer = (struct Customers *)malloc(sizeof(struct Customers));
    Queue queue = (Queue)catQueue;
    Node *ptr = queue->tail;
    //0 = NO/1 = YES
    int waiting = 0;


	printf("consumer\n");
	

	while(queue->itemCount != 0 || isFinished == 0){
		order = NULL;

		pthread_mutex_lock(&lock);

		while(queue->itemCount == 0){
			if(isFinished == 1){
				pthread_mutex_unlock(&lock);
				
				return NULL;
			}

			pthread_cond_signal(&hasSpace);

			pthread_mutex_lock(&bkrPrints);
			
			if(waiting == 0)
				printf("Consumer is waiting\n");

			pthread_mutex_unlock(&bkrPrints);

			pthread_cond_wait(&queue->hasData, &lock);

			waiting = 1;
		}
		
		if(waiting == 1){
			pthread_mutex_lock(&bkrPrints);
				
			printf("Consumer can do stuff again\n");
			waiting = 0;

			pthread_mutex_unlock(&bkrPrints);
		}

		order = nextOrder(queue);
		customer = customers[order->customerID];

		printf("id: %d\n", order->customerID);
		printf("idddd: %d\n", customer->customerID);
			
		printf("limit: %f\n", customer->remainingCredit);
		pthread_mutex_unlock(&lock);


		pthread_cond_signal(&hasSpace);
	}
*/
    return NULL;
}

void printReport(){

    return;
}



int main (int argc, char** argv){
    
        if(argc != 4){ 
                fprintf(stderr, "ERROR: Incorrect Input\n");
                printf("Should be of the form:\n");
                printf("\t\"%s <name of database input file> <name of book order input file>\n\t <name of file containing category names>\"\n", argv[0]);
                return -1; 
        }   
        FILE* database;
        FILE* orders;
        FILE* categories;


        int i;
        char category[20];
        numCategories = 0;
        categoryQueue = (Queue *)malloc(sizeof(Queue) * 200);
        orderQueue = (Queue)malloc(sizeof(struct Queue));
        consumerThreads = (pthread_t *)malloc(sizeof(pthread_t) * 200);

        if(isFile(argv[1])==0){
                fprintf(stderr, "ERROR: %s is not a file\n", argv[1]);
                return -1; 
        }   
        if(isFile(argv[2])==0){
                fprintf(stderr, "ERROR: %s is not a file\n", argv[2]);
                return -1; 
        }   
        if(isFile(argv[3])==0){
                fprintf(stderr, "ERROR: %s is not a file\n", argv[3]);
                return -1; 
        }   
        database = fopen(argv[1], "r");
        orders = fopen(argv[2], "r");
        categories = fopen(argv[3], "r");
        if( (database == NULL) || (categories == NULL) || (orders == NULL) ){
                fprintf(stderr, "ERROR: Could not open file\n");
        }   
        readDatabase(database);
        if(customers == NULL){
                printf("Error with database\n");
                return -1; 
        }   
        
        /*
         * Go through the different categories in the file and
         * add them to the categoryQueue.
         * Set the number of items in the queue to 0.
         * Set the queueID to i.
         * Also need to initialize the thread with the condition variable.
         */
        for(i = 0; fgets(category, 20, categories) != NULL; i++){
            categoryQueue[i] = malloc(sizeof(*categoryQueue[i]));
            categoryQueue[i]->head = NULL;
            categoryQueue[i]->itemCount = 0;
            categoryQueue[i]->queueID = i;
            categoryQueue[i]->category = (char *)malloc(sizeof(char) * strlen(category));
            strtok(category, " \n\t\r");
            strcpy(categoryQueue[i]->category, category);
            pthread_cond_init(&categoryQueue[i]->hasData, 0);
	    printf("cat\t%s\n", categoryQueue[i]->category);
            numCategories++;
        }

        /*
         * Initialize the different threads
         * that are used throughout the program
         */
        pthread_cond_init(&hasSpace, 0);
        pthread_mutex_init(&bkrPrints, 0);
        pthread_mutex_init(&lock, 0);

        //queueCat(categories);
       
        readOrders(orders);

        /*
         * Start up the meat of the program
         * start the producerThread if it returns 0
         * 
         * start the consumerThreads if they return 0
         */
        if(pthread_create(&producerThread, NULL, producer, (void *)orderQueue) != 0)
            printf("Error creating producer thread\n");

        for(i = 0; i < numCategories; i++){
            if(pthread_create(&consumerThreads[i], NULL, consumer, categoryQueue[i]) != 0)
                printf("Error creating consumer thread %d\n", i);
        }

        /* 
         * Call join to wait for all threads to finish what they're doing
         */
        pthread_join(producerThread, NULL);

        for(i = 0; i < numCategories; i++)
            pthread_join(consumerThreads[i], NULL);

        fclose(database);
        fclose(orders);
        fclose(categories);

        /* exit  all threads */
        pthread_exit(0);

        return 0;
}
