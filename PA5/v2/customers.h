#ifndef CUSTOMERS_H
#define CUSTOMERS_H

struct Customers {
        char* name;
        int customerID;
        char* address;
        char* zip;
        double remainingCredit;
        char* st; //need this just cause
        Queue *successful;
        Queue *unsuccessful;
};

typedef struct Customers *Customers;
#endif
