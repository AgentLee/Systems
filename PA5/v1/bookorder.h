#ifndef BOOKORDER_H
#define BOOKORDER_H
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Order *Order;
typedef struct Customers *Customers;

struct Order {
	char* title;
	char* category;
	double price;
	int customerID;
};

struct Customers {
	char* name;
	int customerID;
	char* address;
	char* zip;
	double remainingCredit;
	char* st; //need this just cause

	//Queue *successful;
	//Queue *unsuccessful;
};

#endif 
