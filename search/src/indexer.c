#include "index.h"


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