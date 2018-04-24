#include "sorted-list.h"

SortedListPtr SLCreate(CompareFuncT cf){ 

	//allocate space and initialize stuff
	
	SortedListPtr newList = malloc(sizeof(SortedListPtr));;
	
	newList->cf = cf;
	
	newList->head = NULL;
	
	return newList;
}


void SLDestroy(SortedListPtr list){
	//check for null list
	if (list == NULL){
		////printf("ERROR: Tried to destroy null list.\n");
		return;
	} 
	Node *curr = list->head;
	//check for empty list
	if (curr == NULL){
		free(list);
		return;
	}
	//go to the front of the list (prev=NULL)->->->
	//while (list->prev != NULL){
	//	list = list->prev;
	//}
	//->->->then  delete/null/free everything until the end of the list
	while (curr != NULL)
	{
		curr->prev = NULL;
		free(curr->prev);
		curr = curr->next;
	}
	curr = NULL;
	free(curr);
	free(list);

	return;
	//done
}

int SLInsert(SortedListPtr list, void *newObj){
	if (list == NULL){
		////printf("ERROR: List is null.\n");
		return 0;
	}
	Node *curr = list->head;
	//make sure we have a valid list argument
	if (newObj == NULL){
		return 0;
	}
	//empty list, first item
	if (curr == NULL){ 
		Node *newNode = malloc(sizeof(Node));
		newNode->obj = newObj;
		newNode->prev = NULL;
		newNode->next = NULL;
		list->head = newNode;
		
		return 1;
	}
	//else, find the proper place
	else{
		//create new curr item
		CompareFuncT cf = list->cf;
		Node *newItem = malloc(sizeof(Node));
		
		newItem->obj = newObj;

		//check if it should be first item (larger than the current front)
		if (list->cf(curr->obj,newItem->obj) <= 0){
			newItem->next = curr;
			newItem->prev = NULL;
			curr->prev = newItem;
		
			list->head = newItem;
			
			return 1;
		}
		//traverse list to find the proper place for new item
		else{
			while (list->cf(curr->obj,newItem->obj) > 0){


				//end of the line
				if (curr->next == NULL){
					
					curr->next = newItem;
					newItem->prev = curr;
					newItem->obj = newObj;
					newItem->next = NULL;
					return 1;
				}
				curr = curr->next;
			}
			
			//insert BEFORE current spot
			curr->prev->next = newItem;
			newItem->next = curr;
			newItem->prev = curr->prev;
			curr->prev = newItem;
			return 1;
		}
	////printf("ERROR: Unknown error-> Check ~line 89.\n");
	return 0;
	}
}
		
int SLRemove(SortedListPtr list, void *newObj){
	
	
	//make sure we have a valid list argument
	if (list == NULL){ 
		////printf("ERROR: Null list argument.\n");
		return 0;
	}
	if (list->head == NULL || list->head->obj == NULL)
	{
		////printf("ERROR: Empty list.\n");
		return 0;
	}
	
	//save the head of the list
	Node *hold = list->head;

	//check if we are removing the head
	if (list->cf(list->head->obj,newObj) == 0){
		if (list->head->next == NULL){
			list->head = NULL;
			free(hold);
			return 1;
		}
		list->head = list->head->next;
		list->head->prev = NULL;
		hold->obj = NULL;
		//free(hold);
		//////printf("new list head\n");
		return 1;
		
	}

	//since we are not removing the head, continue searching.
	while (hold != NULL)
	{
		//if found
		if (list->cf(hold->obj,newObj) == 0)
		{
			if (hold->prev == NULL && hold->next == NULL) //only element in list
			{		
				//////printf("test!!\n");
				hold->obj = NULL;
				//list = NULL;
				//free(hold);
			}
			else if (hold->next == NULL) //end of list
			{
			
				hold->prev->next = NULL;
				hold->obj = NULL;
				//free(hold);
			}	
			else if (hold->prev == NULL) //beginning of list
			{
				list->head = list->head->next;
				list->head->prev = NULL;
				hold->obj = NULL;
				//free(hold);
			}
			else //middle of list
			{
				hold->prev->next = hold->next;
				hold->next->prev = hold->prev;
				
				hold->obj = NULL;
				//free(hold);
			}
			return 1;
		}
		hold = hold->next;
	}
	////printf("ERROR: Nothing to remove!\n");
	return 0;
}

SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
	SortedListIteratorPtr iter = malloc(sizeof(SortedListIteratorPtr));
	if (list != NULL){
		iter->curr = list->head;
	}
	else{
		////printf("ERROR: Cannot create iterator on null list.\n");
		return NULL;
	}

	iter->prev = NULL;
	iter->list = list;
	return iter;
}

void SLDestroyIterator(SortedListIteratorPtr iter){
	//free 
	free(iter);
}

void *SLNextItem(SortedListIteratorPtr iter){
	//check for null iterator
	if (iter == NULL){
		//printf("ERROR: Tried to call SLNextItem with null iterator.\n");
		return NULL;
	}

	//check for end of list/empty list
	if (iter->curr == NULL){
		//printf("ERROR: Iterator has reached end of list.\n");
		return NULL;
		
	}

	//undefined object
	/*if (iter->curr->obj == NULL){
		return NULL;
	}*/
	//next item

	iter->prev = iter->curr;
	iter->curr = iter->curr->next;
	
	while (iter->prev == NULL || iter->prev->obj == NULL){
		////printf("here!\n");
	//check for end of list/empty list
		if (iter->curr == NULL){
			//printf("ERROR: Iterator has reached end of list.\n");
			return NULL;
		
		}
			iter->prev = iter->curr;
			iter->curr = iter->curr->next;
		
	}

	////printf("here: \n");

	//if (iter->prev->obj )
	return iter->prev->obj;
}
