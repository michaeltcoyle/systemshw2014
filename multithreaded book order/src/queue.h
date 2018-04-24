#include "node.h"
#include <pthread.h>

//queue structure

typedef struct queue{
	node_t *last;
	pthread_mutex_t mutex;
	pthread_cond_t contains_data;
} queue;

//create queue

queue *queue_create(void);

//enqueue

void queue_enqueue(queue *, void *);

//dequeue

void *queue_dequeue(queue *);

//destroy queue

void queue_destroy(queue *, void (*)(void *));

//check if queue is empty

int queue_isempty(queue *);

//queue peek (no dequeue)

const void *queue_peek(queue *);