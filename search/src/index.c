

#include	"index.h"


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

void add_token(tokentable** hsh, char *name, char* path) {

	
  	tokentable *r;
	r = (tokentable*)malloc(sizeof(tokentable));
	memset(r, 0, sizeof(tokentable));


    filetoken *f;
    f = (filetoken*)malloc(sizeof(filetoken));

    memset(f, 0, sizeof(filetoken));
    char *temp = malloc(sizeof(name));
    strcpy(temp, name);
    r->name = temp;
    
    
    r->ft[0] = f;
    r->ft[0]->count = 1;
 
    r->ft[0]->fn = path;
    int i = 1;
    while (i < 5){
    	r->ft[i] = (filetoken*)malloc(sizeof(filetoken));
    	r->ft[i]->count = 0;
    	r->ft[i]->fn = (char*)malloc(sizeof(char)*256);
    	i++;
    }

    
   // HASH_ADD( hh, *hsh, name, sizeof(r->name), r );  
    HASH_ADD_STR(*hsh, name, r);
}

tokentable *find_token(tokentable **hsh, char *name, char *fn) {
    tokentable *s;

    HASH_FIND_STR(*hsh, name , s );  /* s: output pointer */
	return s;
}

int name_sort(tokentable *x, tokentable *y){
	return strcasecmp(x->name,y->name);
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
		
		while (i < 5 && y->ft[i] != NULL && y->ft[i]->count != 0){
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
			////printf("name: %s\ncount: %d\n file: %s\n",y->name,y->ft[i]->count, y->ft[i]->fn);
			fputs(strbuffer, *wf);
			i++;
		}
		
		
		fputs("\n</list>\n", *wf);
		
		y=y->hh.next;
		
	}
}

void get_file_tokens(char* path, tokentable** hsh, char* abspath){
	
	FILE *rf = (FILE*)malloc(sizeof(FILE));
	rf = fopen(abspath, "r");	
	char *garbage;
	garbage = calloc(sizeof(char)*256, 1);	
	memset(garbage, '\0', sizeof(char)*256);
	char *buffer;
	buffer = calloc(sizeof(char)*256, 1);	
	memset(buffer, '\0', sizeof(char)*256);
	//int flag = 0;
	while (1) {
	
		//printf("start: %s\n",abspath);

		if (rf == NULL){
			////printf("null file\n");
			return;
		}
		if (hsh == NULL){
			////printf("!!\n");
		}
 		//delim garbage


	

		fscanf(rf, "%255[^a-z^A-Z^0-9]", garbage);

 		//these are the tokens
	
		tokentable **temp = hsh;
		
		if (fscanf(rf, "%255[a-zA-Z0-9]", buffer) == 1){
			
			//printf("buffer: %s\n",buffer);
			tokentable *x = find_token(temp,buffer,abspath);

			if (x != NULL){ //token exists in hash table
				int i = 0;
				while (i < 5){
					if (x->ft[i] != NULL && strcasecmp(abspath, x->ft[i]->fn) == 0){
						//printf("first\n");
						x->ft[i]->count++;
						break;
					
					}
					else if (x->ft[i]->count == 0){
						//printf("second1\n");

						
						x->ft[i]->count = 1;
			
						strcpy(x->ft[i]->fn, abspath);
						//printf("second2\n");
						break;
					}
					else{ //new file in the array
						//printf("third\n");
						i++;
					}
				}
			}
			else if (x == NULL){
				//printf("else\n");
				add_token(temp,buffer,abspath);
			}
  		}
   		else{
   			break;
   		}
	}
	memset(garbage, '\0', sizeof(char)*256);
	memset(buffer, '\0', sizeof(char)*256);

}

void traverse(DIR *root, tokentable **hsh, char* abspath){
	
	struct dirent *curr = readdir(root);

	

	while (curr != NULL){
		//printf("first directory is %s\n",curr->d_name);
		
		char *temp1 = malloc(sizeof(abspath)+sizeof(curr->d_name));
		char *temp2 = malloc(sizeof(abspath)+sizeof(curr->d_name));
		char *temp3 = malloc(sizeof(abspath)+sizeof(curr->d_name));
		
		//temp1 = realloc(temp1, sizeof(abspath));
		//temp2 = realloc(temp2, sizeof(abspath));
		//temp3 = realloc(temp3, sizeof(abspath));
		strcpy(temp1,abspath);
		strcpy(temp2,abspath);
	    strcpy(temp3,abspath);
	
		//printf("temps: %s\n", temp1);
		//printf("temps: %s\n", temp2);
		//printf("temps: %s\n", temp3);
		char *check = malloc(sizeof(temp3)+sizeof(curr->d_name));
		strcpy(check,temp3);
		strcat(check,curr->d_name);
		//printf("check: %s\n",check);
		if (strcasecmp(curr->d_name, ".") == 0 || (strcasecmp(curr->d_name, "..") == 0)){
			curr = readdir(root);
			temp1 = NULL;
			temp2 = NULL;
			temp3 = NULL;
			check = NULL;
			free(temp1);
			free(temp2);
			free(temp3);
			free(check);
			continue;
		}
		////printf("name atm: %s\n",abspath);
		if ((is_dir(check) == 1) && strcasecmp(curr->d_name, ".") != 0 && strcasecmp(curr->d_name, "..") != 0){

			//printf("directory found: %s\n",curr->d_name);
			char *recstr = malloc(sizeof(abspath) + sizeof(curr->d_name) + sizeof("/"));
			strcpy(recstr,"");
			strcpy(recstr,abspath);
			strcat(recstr, curr->d_name);
			strcat(recstr, "/");
			//printf("recstr: %s\n",recstr);

			
			traverse(opendir(recstr), hsh, recstr);
		}
		else if (is_file(check) == 1 && strcasecmp(curr->d_name, ".") != 0 && strcasecmp(curr->d_name, "..") != 0){
			//printf("file found: %s\n", curr->d_name);
			
			//printf("getting tokens from: %s\n",check);
			get_file_tokens(curr->d_name, hsh, check);
			
			
		}
		else{
			//printf("neither\n");
			
		}
		temp1 = NULL;
		temp2 = NULL;
		temp3 = NULL;
		check = NULL;
		free(temp1);
		free(temp2);
		free(temp3);
		free(check);
		curr = readdir(root);
	}
	closedir(root);
	return;
}
