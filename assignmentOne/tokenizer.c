/*
 * Jonathan Lee
 * CS214 Section 1
 * 9/17/14
 * tokenizer.c
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
	char *delims;
	char *toks;
	int ptr;
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
	TokenizerT *tok = malloc(sizeof(TokenizerT));
	tok->delims = separators;
	tok->toks = ts;
	tok->ptr = 0;

	return tok;
}


/*
 *  * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 *   * allocated memory that is part of the object being destroyed.
 *    *
 *     * You need to fill in this function as part of your implementation.
 *      */

void TKDestroy(TokenizerT *tk) {
	free(tk);
}

void append(char *str, char c){
	int length = strlen(str);

	str[length] = c;
	str[length+1] = '\0';
}

int isDelim(char c, char *delims){
	int i;

	for(i = 0; i < strlen(delims); i++){
		if(c == delims[i]){
			return 1;
		}
	}

	return 0;
}

/*
 *  * TKGetNextToken returns the next token from the token stream as a
 *   * character string.  Space for the returned token should be dynamically
 *    * allocated.  The caller is responsible for freeing the space once it is
 *     * no longer needed.
 *      *
 *       * If the function succeeds, it returns a C string (delimited by '\0')
 *        * containing the token.  Else it returns 0.
 *         *
 *          * You need to fill in this function as part of your implementation.
 *           */

char *TKGetNextToken(TokenizerT *tk) {
	int i;
	int delimCheck;

	for(i = 0; i < strlen(tk->toks); i++){
		delimCheck = isDelim(tk->toks[i], tk->delims);

		if(delimCheck == 1){
			if(i > 0 && i < (strlen(tk->toks) - 1))
				printf("\n");
		}else {
			printf("%c", tk->toks[i]);
		}
	}
	printf("\n");

	return NULL;
}

/*
 *  * Reduces the amounts of slashes
 *   */
void reduceSlash(char *delims){
	int i = 0;
	int j = 0;
	char newDelims[500];
	char c;

	for(i = 0; i < strlen(delims); i++){
		c = delims[i];

		if(i == 0){ 
			append(newDelims, c);
		}else {
			if(delims[i] == newDelims[j]){
				continue;
			}else {
				append(newDelims, delims[i]);
				j++;
			}
		}
	}
}

int isEscape(char c){
	int i;

	if(c == '\"'){
		i = 2;
	}
	if(c == 'n' || c == 't' || c == 'v' || c == 'b' || c == 'r' || c == 'f' || c == 'a'){
		i = 1;
	}else {
		i = 0;
	}

	return i;
}

char *escapeChars(char c){
	char *hexCode;

	if(c == 'n'){
		hexCode = "[0x0a]";
		return hexCode;
	}else if(c == 't'){
		hexCode = "[0x09]";
		return hexCode;
	}else if(c == 'v'){
		hexCode = "[0x0b]";
		return hexCode;
	}else if(c == 'b'){
		hexCode = "[0x08]";
		return hexCode;
	}else if(c == 'r'){
		hexCode = "[0x0d]";
		return hexCode;
	}else if(c == 'f'){
		hexCode = "[0x0c]";
		return hexCode;
	}else if(c == 'a'){
		hexCode = "[0x07]";
		return hexCode;
	}else if(c == '\\'){
		hexCode = "[0x5c]";
		return hexCode;
	}else if(c == '\"'){
		hexCode = "[0x22]";
		return hexCode;
	}

	return NULL;
}


/*
 *  * main will have two string arguments (in argv[1] and argv[2]).
 *   * The first string contains the separator characters.
 *    * The second string contains the tokens.
 *     * Print out the tokens in the second string in left-to-right order.
 *      * Each token should be printed on a separate line.
 *       */

int main(int argc, char **argv) {
	if(argc != 3){
		printf("Error: Invalid Number of Arguments\n");
		return -1;
	}else {
		int i;
		int j = -1;
		int delimCheck;
		int prevDelimCheck;
		int escapeCheck = 0;
		char *tokenz;
		char *escapes;
		char c;

		/* 
 * 		 * If the inputs make it thus far, they will be sent to TokenizerT
 * 		 		 * to make tok 
 * 		 		 		 */
		TokenizerT *tok = TKCreate(argv[1], argv[2]);
		TokenizerT *tok2 = TKCreate(argv[1], argv[2]);

		tokenz = malloc(sizeof(char) * 1000);

		/* Reduce the number of delimeters */
		reduceSlash(tok->delims);
		reduceSlash(tok->toks);

		reduceSlash(tok2->delims);
		reduceSlash(tok2->toks);

		for(i = 0; i < strlen(tok->toks); i++){
			c = tok->toks[i];
			delimCheck = isDelim(c, tok->delims);

			if(escapeCheck == 1){
				if(isEscape(c) == 1){
					escapes = escapeChars(c);
					strcat(tokenz, escapes);
				}else {
					append(tokenz, c);
				}

				escapeCheck = 0;
			}else if(delimCheck == 1){
				prevDelimCheck = isDelim(tokenz[j], tok->delims);
				if(prevDelimCheck == 1){
					continue;
				}else {
					j = strlen(tokenz);
					append(tokenz, c);
				}
			}else if(c == '\\'){
				if(tok->toks[i-1] == '\\'){
					printf("%c %c\n", tok->toks[i-1], tok->toks[i]);
					strcat(tokenz, "[0x22]");
				}else {
					escapeCheck = 1;
				}
			}else if(c == '\"'){
				strcat(tokenz, "[0x22]");

			}else {
				j = strlen(tokenz);
				append(tokenz, c);
			}
		}

		tok2->toks = tokenz;

		TKGetNextToken(tok2);

		printf("%s\n", tok->toks);
		printf("%s\n", tok->delims);

		TKDestroy(tok);
		TKDestroy(tok2);
		free(tokenz);
	}

	return 0;
}

