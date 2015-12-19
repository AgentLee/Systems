#ifndef BOOKORDER_H
#define BOOKORDER_H

struct Order {
        char* title;
        char* category;
        double price;
        int customerID;
	double remainingCredit;
};
typedef struct Order *Order;

extern Order createOrder(char *title, char *category, double price, int customerID);

#endif
