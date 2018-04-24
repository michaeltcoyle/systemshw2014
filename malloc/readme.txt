Michael Coyle

malloc error checking, assignment 6

malloc() and free() both are from the K&R implementation.

I don't know if I was overthinking this or not, but I couldn't seem to figure out how to modify the code to use the static char array.
What I tried to do was leave K&R's code in-tact and just use the static char array for error checking, however I couldn't get it to work that way.
I suspect that it would have been better to change the code to rely on the char array and actually use it... either way I should have given myself more time.

Makefile is working: make all, make malloc, make clean


Current errors that seem to be caught:
1. 	"Freeing pointers that were never allocated"