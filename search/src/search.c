#include "search.h"

void compareSL(SortedListPtr one, SortedListPtr two){



	SortedListIteratorPtr iter1;
	SortedListIteratorPtr iter2;
	//iterator initialization
	if (one->head != NULL){
		iter1 = SLCreateIterator(one);
	}
	else{
		return;
	}
	if (two->head != NULL){
		iter2 = SLCreateIterator(two);
	}
	else{
		return;
	}



	//the heads are different, so there is no word similarity
	/*
	if (one->cf(iter1->curr->obj,iter2->curr->obj) != 0){

		SLDestroyIterator(iter1);
		SLDestroyIterator(iter2);
		return list;

	}
	*/

	//heads are the same, add to the new list
	/*
	SLInsert(list, iter1->curr->obj);

*/

	while (1){

		//next word in list

		//

		//both iterators are null, no more words to compare. return list as it is.
		if (iter1->curr == NULL && iter2->curr == NULL){
			//printf("what end\n");

			SLDestroyIterator(iter1);
			SLDestroyIterator(iter2);
			break;

		}
		else if (iter1->curr == NULL){
			//printf("null advance two\n");
			SLRemove(two, iter2->curr->obj);
			SLNextItem(iter2);
		}
		else if (iter2->curr == NULL){
			//printf("null advance one\n");
			SLRemove(one, iter1->curr->obj);
			SLNextItem(iter1);
		}
		//they're the same, so there can possibly be more words. continue.
		else if (one->cf(iter1->curr->obj,iter2->curr->obj) == 0){
			//printf("both advance\n");
			//printf("k: %s\n",(char*)iter1->curr->obj);
			//printf("k: %s\n",(char*)iter2->curr->obj);
			SLNextItem(iter1);
			SLNextItem(iter2);

		}
		else if (one->cf(iter1->curr->obj,iter2->curr->obj) > 0){
			//printf("one advance, removed: %s\n", (char*)iter1->curr->obj);
			SLRemove(one, iter1->curr->obj);
			SLNextItem(iter1);
			

		}
		else if (one->cf(iter1->curr->obj,iter2->curr->obj) < 0){
			//printf("two advance\n");
			SLRemove(two, iter2->curr->obj);
			SLNextItem(iter2);
			

		}
		else{

			//printf("what is this?\n");
			SLDestroyIterator(iter1);
			SLDestroyIterator(iter2);

			
		}


	}

}