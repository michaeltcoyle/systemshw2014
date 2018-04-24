#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"


database *cons_db;
queue *soloq;

int producer_flag = 0;

void print_usage() {
    printf("./bookorder <db> <orders> <cats> \n"
           "\t<db> = the name of the database input file\n"
           "\t<orders> = the name of the book order input file\n"
           "\t<cats> = the name of the category input file\n");
}

//initialize the database
database *database_init(FILE **db_file){

	//initial things/variables
	database *datbass = database_create();

	int hold;
	char *line = NULL;
	char *buff;
	char *name;
	char *addr;
	char *state;
	char *zip;

	int id;
	float credit;
	size_t bytes = 0;
	customer *custard = NULL;

	//read database file line by line
	while((hold = getline(&line, &bytes, *db_file) != -1)){

		//get name
		if ((buff = strtok(line, "|")) != NULL){
			printf("db token: %s\n",buff);
			name = (char *)malloc(strlen(buff)+1);
			strcpy(name,buff);

		}
		//get id
		if ((buff = strtok(NULL, "|")) != NULL){
			printf("db token: %s\n",buff);
			id = atoi(buff);
		}
		//get balance/credit
		if ((buff = strtok(NULL, "|")) != NULL){
			printf("db token: %s\n",buff);
			credit = atof(buff);
		}
		//get address
		if ((buff = strtok(NULL, "|")) != NULL){
			printf("db token: %s\n",buff);
			addr = (char *)malloc(strlen(buff)+1);
			strcpy(addr,buff);
		}
		//get state
		if ((buff = strtok(NULL, "|")) != NULL){
			printf("db token: %s\n",buff);
			state = (char *)malloc(strlen(buff)+1);
			strcpy(state,buff);
		}
		//get zip
		if ((buff = strtok(NULL, "|\n")) != NULL){
			zip = (char *)malloc(strlen(buff)+1);
			strcpy(zip,buff);
			//zip[strlen(zip)-1] = '\0'; 
			printf("db token: %s\n",zip);
		}
		printf("---\n");

		//create the customer and add to the database
		custard = customer_create(name, id, credit, addr, state, zip);
		database_customer_add(datbass, custard);

	}

	//clean up and return
	//free(line);
	fclose(*db_file);
	return datbass;

}

void producer(void *orders){

	//cast to file from void
	FILE **orders_file = (FILE **)orders;

	int hold;
	char *line = NULL;
	char *title;
	char *buff;
	float price;
	int cid;
	char *category;
	size_t bytes = 0;
	order *curr_order;

	while((hold = getline(&line, &bytes, *orders_file) != -1)){

		//get title
		if ((buff = strtok(line, "|")) != NULL){
		printf("order token: %s\n",buff);
		title = (char *)malloc(strlen(buff)+1);
		strcpy(title,buff);
		}

		//get price
		if ((buff = strtok(NULL, "|")) != NULL){
		printf("order token: %s\n",buff);
		price = atof(buff);
		}

		//get id
		if ((buff = strtok(NULL, "|")) != NULL){
		printf("order token: %s\n",buff);
		cid = atoi(buff);
		}

		//get category
		if ((buff = strtok(NULL, "|\n")) != NULL){
		category = (char *)malloc(strlen(buff)+1);
		strcpy(category,buff);
		//category[strlen(buff)-1] = '\0'; 
		printf("order token: %s\n",category);
		}
		printf("---\n");

		//create order
		curr_order = order_create(price, title, category, cid);

		//lock mutex and queue order
		pthread_mutex_lock(&soloq->mutex);	
		queue_enqueue(soloq, (void *)curr_order);
		pthread_mutex_unlock(&soloq->mutex);
		pthread_cond_signal(&soloq->contains_data);
	}

	fclose(*orders_file);
	pthread_cond_broadcast(&soloq->contains_data);
	producer_flag = 1;
	//free(line);
	return;

}

void consumer(void *cat){


	char *category = (char *)malloc(strlen((char *)cat+1));
	strcpy(category,(char*)cat);
	category[strlen(category)-1] = '\0';


	printf("thread cat: %s\n", category);
	order *next_order;
	order *curr_order;
	receipt *rcpt;
	customer *custard;
	float diff = 0;

	while (queue_isempty(soloq) == 0 || producer_flag == 0){


		//lock the mutex
		pthread_mutex_lock(&soloq->mutex);

		//check queue for orders
		if (queue_isempty(soloq) == 1 && producer_flag == 0){
			pthread_cond_wait(&soloq->contains_data,&soloq->mutex);
		}
		if (queue_isempty(soloq) == 1 && producer_flag == 1){
			pthread_mutex_unlock(&soloq->mutex);

			return;
		}
		else if (queue_isempty(soloq) == 1)
		{
			pthread_mutex_unlock(&soloq->mutex);
			sched_yield();
			continue;
		}


		//check next order category, if not equal to current category then unlock mutex
		next_order = (order *)queue_peek(soloq);
		if (strcmp(category,next_order->category) == 0){

			curr_order = (order *)queue_dequeue(soloq);
			custard = database_customer_get(cons_db, curr_order->customerid);

			if (!custard){
				fprintf(stderr,"Error: Invalid customer ID.\n");
			}
			else
			{

				//calculate the customers remaining money if they were to purchase
				diff = (custard->credit - curr_order->price);
				rcpt = receipt_create(curr_order->price,curr_order->title,diff);

				//check if the customer has enough money to purchase at all
				if (diff < 0){
					printf("Transaction failed. Insufficient funds.\n");
					printf("Customer: %s (%d) - Item: %s - Price: $%.2f - Balance: $%.2f\n\n", custard->name, custard->customerid, curr_order->title, curr_order->price, custard->credit);
					queue_enqueue(custard->fail, rcpt);
				}
				else
				{
					custard->credit = diff;
					printf("Transaction successful. Balance updated.\n");
					printf("Customer: %s (%d) - Item: %s - Price: $%.2f - Balance: $%.2f\n\n", custard->name, custard->customerid, curr_order->title, curr_order->price, custard->credit);
					queue_enqueue(custard->success, rcpt);
				}

			}

		}	
		else{

			pthread_mutex_unlock(&soloq->mutex);
			sched_yield();

		}

		order_destroy(curr_order);
		pthread_mutex_unlock(&soloq->mutex);
	
	}
	//free(category);
	return;
}

int main(int argc, char **argv){

	//some variables
	int index = 0;
	char *curr_category;

	//check for valid argument count, grats

	if (argc != 4){
		fprintf(stderr, "Error: Invalid argument count.\n");
		print_usage();
		exit(EXIT_FAILURE);
	}

	FILE *categories = fopen(argv[3], "rw");
	if (categories == NULL){
		fprintf(stderr, "Error: Invalid category file.\n");
		print_usage();
		exit(EXIT_FAILURE);
	} 

	FILE *database = fopen(argv[1], "rw");

	if (database == NULL){
		fprintf(stderr, "Error: Invalid database file.\n");
		print_usage();
		exit(EXIT_FAILURE);
	}

	FILE *orders = fopen(argv[2], "rw");

	if (orders == NULL){
		fprintf(stderr, "Error: Invalid orders file.\n");
		print_usage();
		exit(EXIT_FAILURE);
	}



	//parse categories file

	char **category_list = (char **)malloc(sizeof(char *)*1024);

	index = 0;
	curr_category = (char *)malloc(sizeof(char *)*32);

	while ((curr_category = fgets(curr_category, 32, categories)) != NULL && strcmp(curr_category,"") != 0){
		category_list[index] = (char *)malloc(strlen(curr_category)+1);

		strcpy(category_list[index],curr_category);
		category_list[index][strlen(category_list[index])] = '\0';
		//printf("%d: %s\n",index,curr_category);
		index++;
	}

	int category_amt = index;
	//temp2[strlen(temp2)] = '\0';
	index = 0;

	//printf("done.\n");


	//setup queue/db/thread array

	pthread_t thread_ids[category_amt+1];
	soloq = queue_create();
	cons_db = database_init(&database);

	//producer thread
	pthread_create(&thread_ids[0], NULL, (void *)producer, (void *)&orders);

	//consumer threads
	while (index < category_amt){
		printf("making thread for category: %s\n", category_list[index]);
		pthread_create(&thread_ids[index+1], NULL, (void *)consumer, (void *)category_list[index]);
		index++;

	}
	index = 0;
	void *temp;
	while (index < category_amt+1){
		pthread_join(thread_ids[index], &temp);
		index++;
	}
	index = 0;
	int revenue = 0.0f;
	customer *custard;
	receipt *rcpt;
	while (index < 512){
		custard = cons_db->customer[index];

		if (custard == NULL){
			continue;
		}


		printf("=== Customer Info ===\n");
        printf("--- Balance ---\n");
        printf("Customer name: %s\n", custard->name);
        printf("Customer ID number: %d\n", custard->customerid);
        printf("Remaining credit: %.2f\n", custard->credit);
        printf("\n--- Successful orders ---\n");
        if (queue_isempty(custard->success)) {
                printf("\tNone.\n");
		}
		else {
            while ((rcpt = (receipt *)queue_dequeue(custard->success)) != NULL) {
                printf("%s|%.2f|%.2f\n", rcpt->title, rcpt->price,
                                           rcpt->balance);
                revenue += rcpt->price;
                receipt_destroy(rcpt);
            }
        }
        printf("\n--- Failed orders ---\n");
        if (queue_isempty(custard->fail)) {
            printf("None.\n");
        }
        else {
            while ((rcpt = (receipt *)queue_dequeue(custard->fail)) != NULL) {
                printf("%s|%.2f\n", rcpt->title, rcpt->price);
                receipt_destroy(rcpt);
            }
        }
        printf("=== End Customer Info ===\n\n");
        index++;
    }
    printf("Total Revenue: $%.2f\n", revenue);

   	database_destroy(cons_db);
   	queue_destroy(soloq, NULL);
   	return EXIT_SUCCESS;
}