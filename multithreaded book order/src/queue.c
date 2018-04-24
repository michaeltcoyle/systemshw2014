#include <stdlib.h>
#include "queue.h"

#include <pthread.h>

queue *queue_create(){
	queue *solo = (queue *)malloc(sizeof(queue));
	if (solo){
		solo->last = NULL;
		if (pthread_mutex_init(&solo->mutex, NULL) != 0){
			free(solo);
			solo = NULL;
		}
		else if (pthread_cond_init(&solo->contains_data, NULL) != 0){
			pthread_mutex_destroy(&solo->mutex);
			free(solo);
			solo = NULL;
		}

	}
	return solo;
}

void queue_enqueue(queue *tqueue, void *data){
	node_t *node;
	if (!tqueue){
		return;
	}

	node = node_create(data, NULL);

	if (tqueue->last == NULL){
		tqueue->last = node;
		tqueue->last->next = tqueue->last;
	}
	else{
		node->next = tqueue->last->next;
		tqueue->last->next = node;
		tqueue->last = node;
	}
}

void *queue_dequeue(queue *tqueue){

	void *data;

	if (tqueue == NULL || tqueue->last == NULL){
		return NULL;
	}

	if (!tqueue->last->next || tqueue->last == tqueue->last->next){
		data = tqueue->last->data;
		node_destroy(tqueue->last);
	}

	else{
		node_t *killme = tqueue->last->next;
		data = tqueue->last->next->data;
		tqueue->last->next = tqueue->last->next->next;
		node_destroy(killme);
	}
	return data;
}

void queue_destroy(queue *tqueue, void (*destroy_func)(void *)){
	node_t *node;
	node_t *next;
	if (tqueue){
		pthread_mutex_destroy(&tqueue->mutex);
		pthread_cond_destroy(&tqueue->contains_data);

		if (tqueue->last == tqueue->last->next){
			node_destroy(tqueue->last);

		}
		else
		{
			node = tqueue->last;
			while(node){
				next = node->next;
				if (destroy_func){
					destroy_func(node->data);
				}
				node_destroy(node);
				node = next;
			}
		}
		free(tqueue);
	}
}

int queue_isempty(queue *tqueue){
	if (tqueue == NULL || tqueue->last == NULL){
		return 1;
	}
	else{
			return 0;
	}
}

const void *queue_peek(queue *tqueue){
	if (tqueue && tqueue->last && tqueue->last->next) {
		return tqueue->last->next->data;
	}
	else{
		return NULL;
	}
}