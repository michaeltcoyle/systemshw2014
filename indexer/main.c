/*
 * sorted-list.c
 */

#include	<string.h>
#include 	<stdlib.h>
#include	<stdio.h>
#include 	"uthash.h"
#include	<sys/stat.h>
#include	<dirent.h>

int is_file(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

int is_dir(const char* path) {
   struct stat statbuf;
   stat(path, &statbuf);
   return S_ISDIR(statbuf.st_mode);
}

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


void add_token(tokentable** hsh, char *name, char* path) {

	
  	tokentable *r;
	r = (tokentable*)malloc(sizeof(tokentable));
	memset(r, 0, sizeof(tokentable));


    filetoken *f;
    f = (filetoken*)malloc(sizeof(filetoken));

    memset(f, 0, sizeof(filetoken));
    r->name = name;
    
    r->ft[0] = f;
    r->ft[0]->count = 1;
 
    r->ft[0]->fn = path;
    
   // HASH_ADD( hh, *hsh, name, sizeof(r->name), r );  
    HASH_ADD_STR(*hsh, name, r);
}

tokentable *find_token(tokentable **hsh, char *name, char *fn) {
    tokentable *s;

    HASH_FIND_STR(*hsh, name , s );  /* s: output pointer */
	return s;
}

int name_sort(tokentable *x, tokentable *y){
	return strcmp(x->name,y->name);
}

void sort_by_name(tokentable **hsh){
	HASH_SORT(*hsh, name_sort);
}

//print hash table function
void print_tokens(tokentable *x, FILE **wf){
	tokentable *y = x;
	
	while (y != NULL && y->ft != NULL){
		char strbuffer[512];
		char buffer[33];
		int i = 0;
		strcpy(strbuffer,"");
		strcat(strbuffer,"<list> ");
		strcat(strbuffer, y->name);
		
		while (i < 5 && y->ft[i] != NULL){
			if (i == 0){
				fputs(strbuffer, *wf);
				strcpy(strbuffer,"");
				strcat(strbuffer,"\n");
				fputs(strbuffer, *wf);
			}
			strcpy(strbuffer,"");
			strcat(strbuffer, y->ft[i]->fn);
			strcat(strbuffer, " ");
			sprintf(buffer, "%d", y->ft[i]->count);
			strcat(strbuffer,buffer);
			strcat(strbuffer, " ");
			//printf("name: %s\ncount: %d\n file: %s\n",y->name,y->ft[i]->count, y->ft[i]->fn);
			fputs(strbuffer, *wf);
			i++;
		}
		
		if (y->ft[i] == NULL){
			fputs("\n</list>\n", *wf);
		}
		y=y->hh.next;
		
	}
}

void get_file_tokens(char* path, tokentable** hsh, char* abspath){
	FILE *rf = fopen(abspath, "r");
	//int flag = 0;
	while (1) {
		//flag = 0;
		if (rf == NULL){
			//printf("null file\n");
			return;
		}
		if (hsh == NULL){
			//printf("!!\n");
		}
 		//delim garbage
 		char *garbage;
 		
		garbage = (char *)malloc(sizeof(char)*256);
		fscanf(rf, "%255[^a-z^A-Z^0-9]", garbage);
		
 		//these are the tokens
		char *buffer;
		buffer = (char *)malloc(sizeof(char)*256);
		tokentable **temp = hsh;
		
		if (fscanf(rf, "%255[a-zA-Z0-9]", buffer) == 1){
			tokentable *x = find_token(temp,buffer,abspath);
			if (x != NULL){
				int i = 0;
				while (i < 5){
					if (x->ft[i] != NULL && strcmp(abspath, x->ft[i]->fn) == 0){
						x->ft[i]->count++;
						//flag = 1;
						break;
					}
					else if (x->ft[i] == NULL){
						x->ft[i] = malloc(sizeof(filetoken));
						x->ft[i]->count = 1;
						x->ft[i]->fn = malloc(sizeof(abspath));
						strcpy(x->ft[i]->fn, abspath);
						break;
					}
					else{
						i++;
					}
				}
			}
			else if (x == NULL){
				add_token(temp,buffer,abspath);
			}
  		}
   		else{
   			break;
   		}
	}
	//fclose(rf);
	//return;
}

void traverse(DIR *root, tokentable **hsh, char* abspath){
	
	struct dirent *curr = readdir(root);


	
	
	/*printf("first directory is %s\n",curr->d_name);
	
	curr = readdir(root);
	printf("first directory is %s\n",curr->d_naaaa;
	return;*/
	while (curr != NULL){
		char *temp1 = malloc(sizeof(abspath));
		strcpy(temp1,abspath);
		char *temp2 = malloc(sizeof(abspath));
		strcpy(temp2,abspath);
		char *temp3 = malloc(sizeof(abspath));
		strcpy(temp3,abspath);
		if (strcmp(curr->d_name, ".") == 0 || (strcmp(curr->d_name, "..") == 0)){
			curr = readdir(root);
			temp1 = NULL;
			temp2 = NULL;
			temp3 = NULL;
			continue;
		}
		//printf("name atm: %s\n",abspath);
		if ((is_dir(strcat(temp1,curr->d_name)) == 1) && strcmp(curr->d_name, ".") != 0 && strcmp(curr->d_name, "..") != 0){

			//printf("directory found: %s\n",curr->d_name);
			char *recstr = malloc(sizeof(abspath) + sizeof(curr->d_name) + sizeof("/"));
			strcpy(recstr,abspath);
			strcat(recstr, curr->d_name);
			strcat(recstr, "/");

			
			traverse(opendir(recstr), hsh, recstr);

		}
		else if (is_file(strcat(temp2,curr->d_name)) == 1 && strcmp(curr->d_name, ".") != 0 && strcmp(curr->d_name, "..") != 0){
			//printf("file found: %s\n", curr->d_name);
			
			abspath = strcat(abspath, curr->d_name);
			//printf("reading file as: %s\n",abspath);
			get_file_tokens(curr->d_name, hsh, abspath);
			abspath = NULL;
			abspath = malloc(sizeof(temp3));
			strcpy(abspath,temp3);
			
			//curr = readdir(root);
		}
		else{
			//printf("neither\n");
			
		}
		temp1 = NULL;
		temp2 = NULL;
		temp3 = NULL;
		curr = readdir(root);
	}
	closedir(root);
	return;
}

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

int main(int argc, char **argv){
	FILE *rf; //read file
	FILE *wf; //write file
	char *rfpath = (char *)argv[2]; //path of readfile
	char *wfpath = (char *)argv[1]; //path of writefile
	char ow; //overwrite flag

	//check for correct amount of arguments
	if (argc != 3){
			printf("ERROR: Invalid arguments. Exiting.\n");
			return 0;
	}

	//open read file/directory
	rf = fopen(argv[2], "r");
	//check for failure
	if (rf == NULL){
		printf("ERROR: File not found (2nd argument). Exiting.\n");
		return 0;
	}

	//open write file/directory
	wf = fopen(argv[1], "r");
	//check if file/directory exists
	if (wf != NULL){
		while (ow != 'y' && ow != 'n'){
			printf("WARNING: Write file/directory exists. Overwrite? (y/n)\n");
			ow = getc(stdin);
			if (ow == 'n'){
				printf("WARNING: Program will now terminate, file will NOT be overwritten.\n");
				return 0;
			}
		}
	}
	else{
		printf("ERROR: File not found (1st argument). File has been created.\n");
	}
	//reopen file for writing. create file if it does not exist
	wf = fopen(argv[1],"w");

	//create token stream from fscan

	tokentable *hsh = NULL;
	tokentable **temp = &hsh;
	char *abspath = malloc(sizeof(char)*256);
	strcpy(abspath,rfpath);
	//printf("abs path: %s\n", abspath);
	if (is_dir(wfpath) == 1){
		printf("ERROR: Output file is a directory.\n");
		return 0;
	}

	if (is_dir(rfpath) == 1){
		strcat(abspath, "/");
		traverse(opendir(rfpath), temp, abspath);
	}
	else
	{
		//printf("else!\n");
		//traverse(opendir("."), temp, abspath);
		get_file_tokens(rfpath, temp, abspath);
	}

	//sort the list
	sort_by_name(temp);

	//save tokens to write file with correct formatting
	print_tokens(hsh,&wf);

	printf("Successfully wrote token information to write file: %s\n", wfpath);
	return 0;
}