#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include "queue.h"
#include "bookorder.h"

Customers *customers;
Queue *orders;
Queue *categoryQueue;

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

void infoFiller(char *customerInfo){
	Customers newCustomer = malloc(sizeof(*newCustomer));
	/* name
 	 * customerID
 	 * address
 	 * zip
 	 * state
 	 * reaminingCredit
 	 */	
	char *name = strtok(customerInfo, "\"");
	int id = atoi(strtok(NULL, "| "));
	double credit = atof(strtok(NULL, "| "));
	char *address = strtok(NULL, "\"");
	char *state = strtok(NULL, "\"|");
	char *zip = strtok(NULL, "\"|");

	printf("====== N E W  P E R S O N =====\n");
	printf("Name: %s\n", name);	
	printf("Customer ID: %d\n", id);		
	printf("Credit: %f\n", credit);		
	printf("Address: %s\n", address);		
	printf("State: %s\n", state);		
	printf("Zip: %s\n", zip);		
	printf("===============================\n\n");

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
}

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
	
	/*
 	 *  fills customers with information
 	 *  person holds all the info from each line
 	 *  pass person into infoFiller to add to customers
	 */
	while(fgets(person, 100, database) != NULL)
		infoFiller(person);

	/* prints the contents of customers 
	for(i = 0; i < 100; i++){
		if(customers[i] != NULL){
			printf("%s\n", customers[i]->name);
			printf("%s\n", customers[i]->address);
			printf("%s\n", customers[i]->st);
			printf("%s\n", customers[i]->zip);
			printf("%d\n", customers[i]->customerID);
			printf("%f\n", customers[i]->remainingCredit);
			printf("================\n");
		}
	}
	*/
	
	return;
}

void readOrders(/*char* order*/FILE *orders){
	if(orders == NULL)
		fprintf(stderr, "ERROR: could not read order\n");

	/*
 	 * title
 	 * price
 	 * customerID
 	 */

	char orderQueue[200];

	while(fgets(orderQueue, 200, orders))	
		printf("%s\n", orderQueue);

	return;
}

void queueCat(FILE *categories){
	char cats[100];
	
	while(fgets(cats, 100, categories) != NULL){
		printf("%s\n", cats);
	}
}

int main (int argc, char** argv){
	
	if(argc != 4){
		fprintf(stderr, "ERROR: Incorrect Input\n");
		printf("Should be of the form:\n");
		printf("\t\"%s <name of database input file> <name of book order input file>\n\t <name of file containing category names>\"\n", argv[0]);
		return -1;
	}

	FILE* database;
	FILE* books;
	FILE* categories;

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
	books = fopen(argv[2], "r");
	categories = fopen(argv[3], "r");

	if( (database == NULL) || (categories == NULL) || (books == NULL) ){
		fprintf(stderr, "ERROR: Could not open file\n");
	}

	readDatabase(database);
	if(customers == NULL){
		printf("Error with database\n");
		return -1;
	}

	queueCat(categories);

	//readOrders(books);

	return 0;

}
