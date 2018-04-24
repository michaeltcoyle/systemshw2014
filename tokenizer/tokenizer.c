/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct Token_{
	char *word;
	struct Token_ *next;
};

typedef struct Token_ Token;

struct TokenizerT_ {
	char *delims;
	char *stream;
	Token *front;

};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 * 
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate(char *separators, char *ts) {
	TokenizerT *x = malloc(sizeof(TokenizerT));
	x->delims = malloc(sizeof(separators));
	x->stream = malloc(sizeof(ts));
	x->front = malloc(sizeof(Token));
	strcpy(x->delims, separators);
	strcpy(x->stream, ts);		
  return x;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {
	//free(tk->delims);
	//free(tk->stream);
	Token *curr = tk->front;
	Token *delme = tk->front;
	if (curr != NULL)
	{
		while (curr->next != NULL)
		{
			curr = curr->next;
			free(delme);
			delme = curr;
		}
	}
	free(delme);	
	free(tk);
}

Token *CreateTokenStream(TokenizerT *tk){

	int len2 = strlen(tk->delims);
	int len1 = strlen(tk->stream);
	Token *currToken = tk->front;
	int wordIndex = 0;
	char k1;
	char k2;
	char ks2[len1];
	ks2[0] = '\0';
//i is each character in stream, j is each delimiter character
	for (int i = 0; i<len1; i++)
	{
		int flag = 0;
		k1 = tk->stream[i];
		//printf("%c - stream\n",k1);
		if (k1 == '\\')
		{
			if (tk->stream[i+1] == 'n')
			{
				ks2[wordIndex] = '\\';
				wordIndex++;
				ks2[wordIndex] = 'n';
				wordIndex++;
				i++;
				continue;
			}
			else if (tk->stream[i+1] == 't')
			{		
				ks2[wordIndex] = '\\';
				wordIndex++;
				ks2[wordIndex] = 't';
				wordIndex++;
				i++;
				continue;
			}
			else if (tk->stream[i+1] == 'v')
			{
				ks2[wordIndex] = '\\';
				wordIndex++;
				ks2[wordIndex] = 'v';
				wordIndex++;
				i++;
				continue;
			}
			else if (tk->stream[i+1] == 'b')
			{
				ks2[wordIndex] = '\\';
				wordIndex++;
				ks2[wordIndex] = 'b';
				wordIndex++;
				i++;
				continue;
			}
			else if (tk->stream[i+1] == 'r')
			{
				ks2[wordIndex] = '\\';
				wordIndex++;
				ks2[wordIndex] = 'r';
				wordIndex++;
				i++;
				continue;
			}
			else if (tk->stream[i+1] == 'f')
			{				
				ks2[wordIndex] = '\\';
				wordIndex++;
				ks2[wordIndex] = 'f';
				wordIndex++;
				i++;
				continue;
			}
			else if (tk->stream[i+1] == 'a')
			{
				ks2[wordIndex] = '\\';
				wordIndex++;
				ks2[wordIndex] = 'a';
				wordIndex++;
				i++;
				continue;
			}
			else if (tk->stream[i+1] == '\\')
			{				
				ks2[wordIndex] = '\\';
				wordIndex++;
				ks2[wordIndex] = '\\';
				wordIndex++;
				i++;
				continue;
			}
			else if (tk->stream[i+1] == '"')
			{
				ks2[wordIndex] = '\\';
				wordIndex++;
				ks2[wordIndex] = '"';
				wordIndex++;
				i++;
				continue;
			}
		}

		
		for (int j = 0; j<len2; j++)
		{
			k2 = tk->delims[j];
		
			//printf("%c - delim\n",k2);
			if (k1 == k2) //delimiter found in stream, save the token
			{
				char ks[100] = "";
				strncat(ks,ks2,wordIndex);	
	
				if (strcmp(ks2,"") != 0)
				{
					//printf("curr token word: %s\n",ks);
					char *newStr = malloc(sizeof(char)*(wordIndex+1));
					strcpy(newStr,ks);						
					currToken->word = newStr;
					currToken->next = malloc(sizeof(Token));
					currToken = currToken->next;
					wordIndex = 0;
					strcpy(ks2,"");
				}
				flag = 1;
			}
		}
		
		if (flag == 0)
		{
			ks2[wordIndex] = k1;
			ks2[wordIndex+1] = '\0';
			wordIndex++;
		}
		
	}
	/*if (k1 == '\\')
	{
		//wordIndex--;
	}*/
	
	char ks[100] = "";
	strncat(ks,ks2,wordIndex);
	if (strcmp(ks2,"") != 0)
	{
		char *newStr = malloc(sizeof(char)*(wordIndex+1));
		strcpy(newStr,ks);						
		currToken->word = newStr;
		currToken->next = malloc(sizeof(Token));
		currToken = currToken->next;
	}
	return tk->front;
}
/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk) {
	if (tk->front == 0)
	{
		printf("ERROR: There are no tokens to get.\n");
		return 0;
	}
	Token *newtk = tk->front;
	tk->front = newtk->next;
	return newtk->word;
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

char *ReplaceSlashChars(char *x, char *y){

	char newStream[100] = "";
	
	int j = 0;
	int len = strlen(x);

	for (int i = 0; i<len; i++)
	{
		if (x[i] == '\\'){
			
			if (x[i+1] == 'n')
			{
				newStream[j] = '[';
				newStream[j+1] = '0';
				newStream[j+2] = 'x';
				newStream[j+3] = '0';
				newStream[j+4] = 'a';
				newStream[j+5] = ']';
				j = j+6;
				i++;
			}
			else if (x[i+1] == 't')
			{
				newStream[j] = '[';
				newStream[j+1] = '0';
				newStream[j+2] = 'x';
				newStream[j+3] = '0';
				newStream[j+4] = '9';
				newStream[j+5] = ']';
				j = j+6;
				i++;
			}
			else if (x[i+1] == 'v')
			{
				newStream[j] = '[';
				newStream[j+1] = '0';
				newStream[j+2] = 'x';
				newStream[j+3] = '0';
				newStream[j+4] = 'b';
				newStream[j+5] = ']';
				j = j+6;
				i++;
			}
			else if (x[i+1] == 'b')
			{
				newStream[j] = '[';
				newStream[j+1] = '0';
				newStream[j+2] = 'x';
				newStream[j+3] = '0';
				newStream[j+4] = '8';
				newStream[j+5] = ']';
				j = j+6;
				i++;
			}
			else if (x[i+1] == 'r')
			{
				newStream[j] = '[';
				newStream[j+1] = '0';
				newStream[j+2] = 'x';
				newStream[j+3] = '0';
				newStream[j+4] = 'd';
				newStream[j+5] = ']';
				j = j+6;
				i++;
			}
			else if (x[i+1] == 'f')
			{
				newStream[j] = '[';
				newStream[j+1] = '0';
				newStream[j+2] = 'x';
				newStream[j+3] = '0';
				newStream[j+4] = 'c';
				newStream[j+5] = ']';
				j = j+6;
				i++;
			}
			else if (x[i+1] == 'a')
			{
				newStream[j] = '[';
				newStream[j+1] = '0';
				newStream[j+2] = 'x';
				newStream[j+3] = '0';
				newStream[j+4] = '7';
				newStream[j+5] = ']';
				j = j+6;
				i++;
			}
			else if (x[i+1] == '\\')
			{
				newStream[j] = '[';
				newStream[j+1] = '0';
				newStream[j+2] = 'x';
				newStream[j+3] = '5';
				newStream[j+4] = 'c';
				newStream[j+5] = ']';
				j = j+6;
				i++;
			}
			else if (x[i+1] == '"')
			{
				newStream[j] = '[';
				newStream[j+1] = '0';
				newStream[j+2] = 'x';
				newStream[j+3] = '2';
				newStream[j+4] = '2';
				newStream[j+5] = ']';
				j = j+6;
				i++;
			}

			else
			{
			 //nothing
			}
		}
		else{ //no escape character, copy current char to new string
			newStream[j] = x[i];
			j++;
		}
	}
	strcpy(y,newStream);
	return y;
}

int main(int argc, char **argv) 
{

//check if there are sufficient arguments
	if (argc != 3){
		printf("ERROR: there must be 2 arguments; a delimiter string and tokens string.\n");
	}
	TokenizerT *currTokenizer = TKCreate(argv[1],argv[2]);

	//currTokenizer->stream = "hello \\\\world \\lol \\\"test\\";
	//printf("interpreted input stream as: %s\n",currTokenizer->stream);
	CreateTokenStream(currTokenizer);
	
	char *currWord1 = TKGetNextToken(currTokenizer);
	int flag = 0;
	if (currWord1 == 0)
	{
		flag = 1;
	}
	else
	{
		char newStr1[100] = "";
		ReplaceSlashChars(currWord1,newStr1);
		printf("%s\n",newStr1);
	}
	while (flag != 1)
	{
		char *currWord2 = TKGetNextToken(currTokenizer);
		if (currWord2 == 0)
		{
			flag = 1;
		}
		else
		{
			char newStr2[100] = "";
			ReplaceSlashChars(currWord2,newStr2);
			printf("%s\n",newStr2);
		}
	}
	TKDestroy(currTokenizer);
	return 0;
}


