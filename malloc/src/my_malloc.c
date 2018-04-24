//my_malloc and my_free function definitions

#include "my_malloc.h"
#include <unistd.h>
#include <stdio.h>

#define NALLOC 128


static char myblock[5000] = { [ 0 ... 4999] = '0' };

static Myheader base;
static Myheader *allocp;

static Myheader *morecore(unsigned int nu)

{
	char *cp;
	Myheader *up;
	int rnu;

	rnu = NALLOC * ((nu + NALLOC - 1) / NALLOC);
	cp = sbrk (rnu * sizeof (Myheader));
	if ((long)cp == 0)
		return 0;
	up = (Myheader *) cp;
	up->stuff.size = rnu;
	myfree((char *)(up + 1), (char *)__FILE__,(int)__LINE__);
	return (allocp);
}



void *mymalloc(size_t nbytes, char* f, int l)
{

	printf("Called malloc at %s:%d\n",f,l);
	Myheader *p, *q;	/* K&R called q, prevp */
	unsigned nunits;
	//printf("nbytes: %d\n",nbytes);
	nunits = (nbytes + sizeof (Myheader) - 1) / sizeof (Myheader) + 1;
	//printf("nunits: %d\n",nunits);
	if ((q = allocp) == 0) {	/* no free list yet */
		base.stuff.ptr = allocp = q = &base;
		base.stuff.start = 0;
		base.stuff.end = 4999;
		base.stuff.size = 0;
	}
	for (p = q->stuff.ptr;; q = p, p = p->stuff.ptr) {
		if (p->stuff.size >= nunits) {	/* big enough */
			if (p->stuff.size == nunits){	/* exactly */
				q->stuff.ptr = p->stuff.ptr;
				q->stuff.start = p->stuff.start;
				p->stuff.start += nunits;
				q->stuff.end = q->stuff.start + nunits;
				
				int i = q->stuff.start;
				//printf("here and i = %d\n",i);
				for (; i <= q->stuff.end; i++)
				{
					//printf("%d",i);
					myblock[i] = '1';
				}
				//printf("%d",i);
			}
			else {		/* allocate tail end */
				q->stuff.ptr = p->stuff.ptr;
				q->stuff.start = p->stuff.start;
				p->stuff.start += nunits;
				q->stuff.end = q->stuff.start + nunits;
				int i = q->stuff.start;
				//printf("here and i = %d\n",i);
				for (; i <= q->stuff.end; i++)
				{
					//printf("%d",i);
					myblock[i] = '1';
				}
				//printf("%d",i);
			}
			allocp = q;


			return ((char *)(p + 1));
		}
		if (p == allocp)
			//printf("morecore thing\n");
			if ((p = morecore(nunits)) == 0)
			{
				//printf("RETURNING 0\n");
				return 0;
			}
			
	}

}


void myfree(void *ap, char* f, int l)
{
	printf("Called free at %s:%d\n",f,l);
	Myheader *p, *q;
	if(ap == NULL)
	{
		printf("ERROR: Was this free in file %s at line %d malloc'd?\n",f,l);
		return;
	}
	//if (myblock)
	p = (Myheader *) ap - 1;
	for (q = allocp; !(p > q && p < q->stuff.ptr); q = q->stuff.ptr)
	{
		if (q >= q->stuff.ptr && (p > q || p < q->stuff.ptr))
			break;
	}

	if (p + p->stuff.size == q->stuff.ptr) {
		p->stuff.size += q->stuff.ptr->stuff.size;
		p->stuff.ptr = q->stuff.ptr->stuff.ptr;
	} else
		p->stuff.ptr = q->stuff.ptr;
	if (q + q->stuff.size == p) {
		q->stuff.size += p->stuff.size;
		q->stuff.ptr = p->stuff.ptr;
	} else
		q->stuff.ptr = p;
	allocp = q;

}

int allocsize(void *ap)
{
	Myheader *p;

	p = (Myheader *) ap - 1;
	return (p->stuff.size * sizeof (Myheader));
}