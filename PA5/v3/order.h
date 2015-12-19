#ifndef ORDER_H
#define ORDER_h
#include <pthread.h>

struct Order{
	char *title;
	double price;
	int customerID;
	char *category;
	double remainingCredit;
};
typedef struct Order *Order;

#endif
