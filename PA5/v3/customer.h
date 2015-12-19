#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <pthread.h>

struct Customer{
	char *name;
	int customerID;
	double creditLim;
	char *address;
	char *state;
	char *zip;
	Queue bought;
	Queue reject;
};


#endif
