all : sortedlist

sortedlist : main.c sorted-list.o
	gcc -o libsl.a -c sorted-list.c
	gcc -Wall -g -o sl main.c libsl.a

clean :
	$(RM) libsl.a
	$(RM) sl
