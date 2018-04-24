#include "node.h"
#include <stdlib.h>

node_t *node_create(void *data, node_t *next){
	node_t *shit = (node_t*)malloc(sizeof(node_t));
	shit->data = data;
	shit->next = next;
	shit->refs = 0;
	return shit;
}

void node_destroy(node_t *node){
	free(node);
}