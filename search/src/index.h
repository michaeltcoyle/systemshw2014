

#include	<string.h>
#include 	<stdlib.h>
#include	<stdio.h>
#include 	"uthash.h"
#include	<sys/stat.h>
#include	<dirent.h>


typedef struct {
	int count;
	char* fn;
} filetoken;


typedef struct {
	char *name;
	filetoken *ft[5];
	//filetoken *ft;
	UT_hash_handle hh;

} tokentable; 

int is_file(const char* path);

int is_dir(const char* path);

void add_token(tokentable** hsh, char *name, char* path);

tokentable *find_token(tokentable **hsh, char *name, char *fn);

int name_sort(tokentable *x, tokentable *y);

void sort_by_name(tokentable **hsh);

void print_tokens(tokentable *x, FILE **wf);

void get_file_tokens(char* path, tokentable** hsh, char* abspath);

void traverse(DIR *root, tokentable **hsh, char* abspath);

