#include <unistd.h>
#include "my_malloc.c"


#define malloc( x ) mymalloc( x,(char*) __FILE__,(int) __LINE__ )
#define free( x ) myfree( x,(char*) __FILE__,(int) __LINE__ )

int main(int argc, char **argv)
{


	//case 1: freeing pointers that were never allocated.
	int h;
	free(h);

	return 0;








}


	/*
	int test = 10;
	//char *s;
	char *t = (char*)malloc(5);
	char *s = (char*)malloc(256);
	Myheader *derp;
	Myheader *herp = malloc(sizeof(Myheader));

	if (derp)
	{
		printf("derp\n");
		//derp->stuff.size = 1;
		//printf("derp size: %d\n", derp->stuff.size);
	}
	if (herp)
	{
		printf("herp\n");
		herp->stuff.size = 1;
		printf("herp size: %d\n", herp->stuff.size);
	}

	t[0] = 'a';
	t[1] = 'b';
	t[2] = 'c';
	t[3] = 'd';
	t[4] = 'e';
	t[5] = 'f';
	t[6] = 'g';
	t[7] = 'h';
	t[8] = 'i';
	t[9] = 'j';

	printf("%c %c\n", t[1], t[9]);

	printf("sizeof t: %d\n",sizeof(t));
	printf("sizeof s: %d\n",sizeof(s));
	if (t == NULL)
	{
		printf("t null.\n");
	}
	if (s == NULL)
	{
		printf("s null.\n");
	}
	//printf("%s\n",s);

	char *x;
	char *y = (char*)malloc(sizeof(char)*256);
	char *z;
	y = "Y asdf";
	x = "X asdfghjkajsdasd";
	printf("test!\n");
	while (test != 0){
		//strcat(y,"1");
		//strcat(x,(char*)test);
		test = test-1;
	}
	z = x;
	printf("z = x : %s\n",z);
	printf("y = %s\n",y);
	printf("x = %s\n",x);
	z = y;
	printf("z = y : %s\n",z);

	int *h = malloc(32);
	//free(h);
*/

	
	/*int o;
	printf("\n");
	for (o = 0; o < 5000; o++)
	{
		printf("%c",myblock[o]);
	}
	printf("\n");*/




