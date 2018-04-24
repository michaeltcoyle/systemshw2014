#include "src/search.h"
#include "src/sorted-list.h"
#include "src/sorted-list.c"

int compareStrings(void *p1, void *p2)
{
	char *s1 = p1;
	char *s2 = p2;

	return strcasecmp(s1, s2);
}


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

	CompareFuncT cf = &compareStrings;
	DestructFuncT df = &destroyBasicTypeAlloc;


	if (argc != 2){
		printf("ERROR: Invalid arguments. Correct usage is: ./search <inverted index file>\n");
		return 0;
	}

	//ask for commands (sa/so/q);

	while (1){

		printf("Please enter a command: ""sa <terms>"" / ""so <terms>"" or ""q"" to quit.\n");

		char *input = malloc(sizeof(char)*1024);
		fgets(input, 1024, stdin);
		
		//printf("read input: %s\n", input);
		char* token = strtok(input, " \n");
		
		//printf("read the command: %s\n", token);
		

		SortedListPtr list = SLCreate(cf, df);

		if (strcasecmp(token, "sa") == 0){

			//populate query list
			token = strtok(NULL, " \n");
			while (token != NULL) {
	
				//printf("read the word: %s\n", token);
				char *word = (char *)malloc(sizeof(token));
				strcpy(word,token);
				SLInsert(list, word);
				token = strtok(NULL, " \n"); 

			}

			//create query list iterator
			SortedListIteratorPtr iter = SLCreateIterator(list);
			char *ch;
			ch = SLNextItem(iter);
			
			

			//malloc various things
			char* buff = malloc(sizeof(char)*1024);
			char* cmpbuff = malloc(sizeof(char)*1024);
			FILE *indexFile;
			SortedListPtr fileList = SLCreate(cf, df);
			
			int flag = 1;
			int flag2 = 0;
			
			while (ch != NULL){ //term
				
				indexFile = fopen(argv[1], "r");
				while (feof(indexFile) == 0){ //seek term
					
					strcpy(buff,"");
					fgets(buff, 1024, indexFile);
					strcpy(cmpbuff, "<list> ");
					strcat(cmpbuff, ch);
					strcat(cmpbuff, "\n");
					//printf("buff: ""%s""\n",buff);
					//printf("cmpbuff: ""%s""\n",cmpbuff);
					
					if (strcasecmp(cmpbuff, buff) == 0){
						//printf("match: %s\n", ch);
						SortedListPtr fileList2 = SLCreate(cf, df);
						char *line = malloc(sizeof(char)*1024);
						token = strtok(fgets(line, 1024, indexFile), " \n");
						
						while (token != NULL){ //file loop
							//printf("token, term: %s, %s\n", token,ch);
							if (flag){

								char *temp = malloc(sizeof(token));
								char *temp2 = malloc(sizeof(token));
								strcpy(temp, token);
								strcpy(temp2, token);
								SLInsert(fileList, temp);
								SLInsert(fileList2, temp2);
								//printf("added token to list: %s\n",temp);
								//printf("added token to list2: %s\n",temp2);
								token = strtok(NULL, " \n");
								token = strtok(NULL, " \n");
								
							}
							else{

								char *temp2 = malloc(sizeof(token));
								strcpy(temp2, token);
								//printf("added token to list2: %s\n",temp2);
								SLInsert(fileList2, temp2);
								token = strtok(NULL, " \n");
								token = strtok(NULL, " \n");
								

							}
						}

						

						flag = 0;
						compareSL(fileList, fileList2);
						flag2 = 1;
						break;

					}
				}

				fclose(indexFile);
				
				if (flag2 != 1){
					printf("No files matching search terms were found.\n");
					break;
				}

				ch = SLNextItem(iter);

			}

			//SLDestroyIterator(iter);
			SortedListIteratorPtr iterrr = SLCreateIterator(fileList);
			char *cha;
			cha = SLNextItem(iterrr);
			while (cha != NULL){
				printf("%s\n",cha);
				cha = SLNextItem(iterrr);
			}

			



		}
		else if (strcasecmp(token, "so") == 0){
			//populate query list
			token = strtok(NULL, " \n");
			while (token != NULL) {
	
				//printf("read the word: %s\n", token);
				char *word = (char *)malloc(sizeof(token));
				strcpy(word,token);
				SLInsert(list, word);
				token = strtok(NULL, " \n"); 

			}

			//create query list iterator
			SortedListIteratorPtr iter = SLCreateIterator(list);
			char *ch;
			ch = SLNextItem(iter);
			
			

			//malloc various things
			char* buff = malloc(sizeof(char)*1024);
			char* cmpbuff = malloc(sizeof(char)*1024);
			FILE *indexFile;
			SortedListPtr fileList = SLCreate(cf, df);
			SortedListPtr fileList2 = SLCreate(cf, df);
			int flag = 1;
			
			while (ch != NULL){ //term
				
				indexFile = fopen(argv[1], "r");
				while (feof(indexFile) == 0){ //seek term
					
					strcpy(buff,"");
					fgets(buff, 1024, indexFile);
					strcpy(cmpbuff, "<list> ");
					strcat(cmpbuff, ch);
					strcat(cmpbuff, "\n");
					//printf("buff: ""%s""\n",buff);
					//printf("cmpbuff: ""%s""\n",cmpbuff);
					
					if (strcasecmp(cmpbuff, buff) == 0){
						//printf("match: %s\n", ch);
						
						char *line = malloc(sizeof(char)*1024);
						token = strtok(fgets(line, 1024, indexFile), " \n");
						
						while (token != NULL){ //file loop
							//printf("token, term: %s, %s\n", token,ch);
							if (flag){

								char *temp = malloc(sizeof(token));
								char *temp2 = malloc(sizeof(token));
								strcpy(temp, token);
								strcpy(temp2, token);
								SLInsert(fileList, temp);
								SLInsert(fileList2, temp2);
								//printf("added token to list: %s\n",temp);
								//printf("added token to list2: %s\n",temp2);
								token = strtok(NULL, " \n");
								token = strtok(NULL, " \n");
								
							}
							else{

								char *temp2 = malloc(sizeof(token));
								strcpy(temp2, token);
								//printf("added token to list2: %s\n",temp2);
								SLInsert(fileList2, temp2);
								token = strtok(NULL, " \n");
								token = strtok(NULL, " \n");
								

							}
						}

						

						flag = 0;
						break;

					}
				}
				
				
				fclose(indexFile);
				ch = SLNextItem(iter);

			}

			//SLDestroyIterator(iter);
			SortedListIteratorPtr iterrr = SLCreateIterator(fileList2);
			char *cha;
			cha = SLNextItem(iterrr);
			while (cha != NULL){
				if (iterrr->curr != NULL && fileList2->cf(iterrr->curr->obj,iterrr->prev->obj) == 0){
					SLRemove(iterrr->list, iterrr->curr->obj);
					cha = SLNextItem(iterrr);
					continue;
				}
				else{
					printf("%s\n",cha);
					cha = SLNextItem(iterrr);
				}
			}

			



		

			//........

		}
		else if (strcasecmp(token, "q") == 0){

			printf("Program will now exit. Please give me a 100%%.\n");
			return 0;

		}
		else{

			printf("ERROR: Invalid usage, exiting.\n");
			return 0;

		}

	}


//if sa

	

	//char *stream = malloc(sizeof(char)*128);

	//fgets(stream, 128, indexFile);

	//if (strcasecmp())






	return 0;
}