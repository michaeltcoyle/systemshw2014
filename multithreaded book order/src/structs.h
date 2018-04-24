#include "queue.h"
//order structure
typedef struct order {
		float price;
		char *title;
		char *category;
		int customerid;
} order;

//create new order

order *order_create(float, char *, char *, int);

//destroy order

void order_destroy(order *);

//receipt structure

typedef struct receipt {
	float price;
	char *title;
	float balance;
} receipt;

//create new receipt

receipt *receipt_create(float, char *, float);

//destroy receipt

void receipt_destroy(void *);

//customer structure

typedef struct customer {
	char *name;
	int customerid;
	float credit;
	queue *success;
	queue *fail;
	char *address;
	char *state;
	char *zip;
} customer;

//create new customer

customer *customer_create(char *, int, float, char *, char *, char *);

//destroy customer

void customer_destroy(customer *);

//database structure

typedef struct database {
	customer *customer[512];
} database;

//create database

database *database_create();

//destroy database

void database_destroy(database *);

//add customer to database

void database_customer_add(database *, customer *);

//get customer info from database

customer *database_customer_get(database *, int);