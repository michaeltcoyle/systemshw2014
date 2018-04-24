//mymalloc and myfree
#include	<stddef.h>
typedef long Align;

typedef union header
{

	struct
	{
		int start;
		int end;
		union header *ptr; //next block
		size_t size; //this block size

	} stuff;

	Align x;

} Myheader;

//void block_manage(Myheader *x);

void *mymalloc(size_t size, char* f, int l);

void myfree(void *, char* f, int l);
