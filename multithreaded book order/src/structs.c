#include "structs.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

order *order_create(float price, char *title, char *category, int customerid){
	order *ord = (order *)malloc(sizeof(order));

	if (ord){
		ord->price = price;
		ord->title = (char *)malloc(strlen(title)+1);
		strcpy(ord->title,title);
		ord->category = (char *)malloc(strlen(category)+1);
		strcpy(ord->category,category);
		ord->customerid = customerid;
	}

	return ord;
}

void order_destroy(order *ord){

	if (ord){

		free(ord->category);
		free(ord->title);
		free(ord);

	}
}

receipt *receipt_create(float price, char *title, float balance){

	receipt *rpt = (receipt *)malloc(sizeof(receipt));

	if (rpt){

		rpt->price = price;
		rpt->balance = balance;
		rpt->title = (char *)malloc(strlen(title)+1);
		strcpy(rpt->title,title);

	}

	return rpt;
}

void receipt_destroy(void *data){

	if (data){
		receipt *rpt = (receipt *)data;
		free(rpt->title);
		free(rpt);
	}
}

customer *customer_create(char *name, int customerid, float credit, char *addr, char *state, char *zip){

	customer *custard = (customer *)malloc(sizeof(customer));
	if (custard){

		custard->name = (char *)malloc(sizeof(name)+1);
		strcpy(custard->name,name);
		custard->state = (char *)malloc(sizeof(state)+1);
		strcpy(custard->state,state);
		custard->zip = (char *)malloc(sizeof(zip)+1);
		strcpy(custard->zip,zip);
		custard->customerid = customerid;
		custard->credit = credit;
		custard->success = queue_create();
		custard->fail = queue_create();
		

	}

	return custard;
}

void customer_destroy(customer *custard){

	if (custard){

		free(custard->name);
		queue_destroy(custard->fail, &receipt_destroy);
		queue_destroy(custard->success, &receipt_destroy);
		free(custard);
	}
}

database *database_create(){
	return (database *)malloc(sizeof(database));
}

void database_destroy(database *database){
	if (database){
		int i = 0;
		while(i < 512){
			customer_destroy(database->customer[i]);
			i++;
		}
		free(database);
	}
}

void database_customer_add(database *database, customer *custard){

	database->customer[custard->customerid] = custard;

}

customer *database_customer_get(database *database, int customerid){

	if (customerid > -1 && customerid < 512){
		return database->customer[customerid];
	}
	else
	{
		fprintf(stderr, "Customer id %d out of array bounds.\n", customerid);
		return NULL;
	}
}