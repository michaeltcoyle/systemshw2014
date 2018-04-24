//node structure
typedef struct node {
	void *data;
	struct node *next;
	int refs;
} node_t;

//create node

node_t *node_create(void *, node_t *);

//destroy node

void node_destroy(node_t *);
