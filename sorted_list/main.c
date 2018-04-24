/*
 * sorted-list.c
 */

#include	<string.h>
#include	"sorted-list.h"

int compareInts(void *p1, void *p2)
{
	int i1 = *(int*)p1;
	int i2 = *(int*)p2;

	return i1 - i2;
}

int compareDoubles(void *p1, void *p2)
{
	double d1 = *(double*)p1;
	double d2 = *(double*)p2;

	return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2)
{
	char *s1 = p1;
	char *s2 = p2;

	return strcmp(s1, s2);
}
/*

void printListInt(SortedListPtr x){
	if (x == NULL){
		printf("ERROR: Empty list.\n");
	}
	else if (x->head == NULL){
		printf("ERROR: Empty list.\n");
	}
	else{
		Node *y = x->head;
		printf("Printing Integer List:\n");
		while (y != NULL){
			printf("%d\n", *((int*)y->obj));
			y = y->next;
		}
	}

void printListChar(SortedListPtr x){
	if (x == NULL){
		printf("ERROR: Empty list.\n");
	}
	else if (x->head == NULL){
		printf("ERROR: Empty list.\n");
	}
	else{
		Node *y = x->head;
		printf("Printing Integer List:\n");
		while (y != NULL){
			printf("%c\n", *((char*)y->obj));
			y = y->next;
		}
	}

}*/
//Destructor functions
void destroyBasicTypeAlloc(void *p){
	//For pointers to basic data types (int*,char*,double*,...)
	//Use for allocated memory (malloc,calloc,etc.)
	free(p);
}

void destroyBasicTypeNoAlloc(void *p) {
	//For pointers to basic data types (int*,char*,double*,...)
	//Use for memory that has not been allocated (e.g., "int x = 5;SLInsert(mylist,&x);SLRemove(mylist,&x);")
	return;
}

int main(){
	SortedListPtr list = SLCreate(&compareStrings);
	char *w1, *w2, *w3, *w4;
	w1 = (char *)malloc(sizeof(char)*1);
	w2 = (char *)malloc(sizeof(char)*1);
	w3 = (char *)malloc(sizeof(char)*1);
	w4 = (char *)malloc(sizeof(char)*1);
	w1[0] = 'f';
	w2[0] = 'd';
	w3[0] = 'b';
	w4[0] = 'z';


	printf("Test 1: [""f"",""d"",""b""] is the list, and an iterator is created.  Then ""z"" is inserted (in the beginning).  SLNextItem calls with the iterator will return ""f"",""d"",""b"",NULL in that order.\n");


	SLInsert(list, w1);
	SLInsert(list, w2);
	SLInsert(list, w3);
	SortedListIteratorPtr iter = SLCreateIterator(list);
	SLInsert(list, w4);


	char *ch;

	ch = SLNextItem(iter);
	printf("%s\n",ch);
	ch = SLNextItem(iter);
	printf("%s\n",ch);
	ch = SLNextItem(iter);
	printf("%s\n",ch);
	ch = SLNextItem(iter);
	if (ch == NULL){
		printf("NULL\n");
	}
	else{
		printf("%s\n",ch);
	}
	

	SLDestroyIterator(iter);
	
	printf("Test 1.2: Confirm that the actual list is indeed z,f,d,b (by destroying and recreating the iterator).\n");

	iter = SLCreateIterator(list);

	ch = SLNextItem(iter);
	printf("%s\n",ch);
	ch = SLNextItem(iter);
	printf("%s\n",ch);
	ch = SLNextItem(iter);
	printf("%s\n",ch);
	ch = SLNextItem(iter);
	if (ch == NULL){
		printf("NULL\n");
	}
	else{
		printf("%s\n",ch);
	}

	SLRemove(list, w4);
	
	printf("Test 2: [""f"",""d"",""b""] is the list, and an iterator is created.  SLNextItem is called 3 times and returns ""f"",""d"",""b"".  ""a"" is then inserted.  The next call to SLNextItem returns NULL.\n");

	SLDestroyIterator(iter);

	iter = SLCreateIterator(list);

	ch = SLNextItem(iter);
	printf("%s\n",ch);
	ch = SLNextItem(iter);
	printf("%s\n",ch);
	ch = SLNextItem(iter);
	printf("%s\n",ch);

	w4[0] = 'a';

	SLInsert(list, w4);

	
	ch = SLNextItem(iter);
	if (ch == NULL){
		printf("NULL\n");
	}
	else{
		printf("%s\n",ch);
	}

	printf("Test 2.2: Confirm that the actual list is indeed f,d,b,a (by destroying and recreating the iterator).\n");

	SLDestroyIterator(iter);

	iter = SLCreateIterator(list);

	ch = SLNextItem(iter);
	printf("%s\n",ch);
	ch = SLNextItem(iter);
	printf("%s\n",ch);
	ch = SLNextItem(iter);
	printf("%s\n",ch);
	ch = SLNextItem(iter);
	if (ch == NULL){
		printf("NULL\n");
	}
	else{
		printf("%s\n",ch);
	}


	printf("Test 3: [""f"",""d"",""b""] is the list, and an iterator is created.  SLNextItem is called once, so the iterator points to ""d"". ""d"" is then removed with SLRemove.  Future calls to SLNextItem will return ""b"" and NULL in that order.\n");

	SLRemove(list, w4);
	SLDestroyIterator(iter);
	iter = SLCreateIterator(list);
	ch = SLNextItem(iter); //i'm calling SLNextItem twice because I think that it should point to F on the first call, and then D on the second call. (otherwise this case is inconsistent with the others)
	//ch = SLNextItem(iter);
	if (ch == NULL){
		printf("NULL\n");
	}
	else{
		printf("last was: %s\n",ch);
	}
	SLRemove(list, w2);

	ch = SLNextItem(iter);
	if (ch == NULL){
		printf("NULL\n");
	}
	else{
		printf("%s\n",ch);
	}
	ch = SLNextItem(iter);
	if (ch == NULL){
		printf("NULL\n");
	}
	else{
		printf("%s\n",ch);
	}


	return 0;

	

}
