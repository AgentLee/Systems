/*
 * CS214 Assignment 4
 *
 * Jonathan Lee (jl1424)
 * Niharika Nagar (nn164)
 *
 */

#include "search.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


int isFile(char *fileName){
        struct stat s;
        if(stat(fileName, &s) == 0){
                if(s.st_mode &S_IFREG){
                        return 1;
                }else {
                        return 0;
                }
        }

        return 0;
}

void freeAllNodes(wordLL w){

}

/* Creating wordNode to store words */
wordNode createWordNode(){
    wordNode w = (wordNode)malloc(sizeof(struct wordNode));
    w->value = NULL;  
    return w;
}

/* Creating fileNode to store files inside each wordNode */
fileNode createFileNode(){
    fileNode f = (fileNode) malloc(sizeof(struct fileNode));
    f->file = NULL;
    return f;
}

/* 
 * Instead of doing these 3 lines over and over again here's a nifty function
 * It takes in the search terms and creates a separate iteration 
 * This way it won't mess up the terms for future use 
 */
char *createSearchTerms(char *terms){
	char *searchTerms = (char *)malloc(sizeof(char) * strlen(terms) +1);
	strcpy(searchTerms, terms);
	strcat(searchTerms, "\0");

	return searchTerms;	
}

/*
 * This function is useful for AND
 * If a word doesn't exist at all in any of the files then AND fails
 */
int inFile(char *terms, char *op, wordLL wLL){
	char *searchTerms = createSearchTerms(terms);
	
	struct wordNode *wtmp = wLL->head;

	int comp = 0;
	int numTokens = 0;
	int matchTokens = 0;

	char *token = strtok(searchTerms, " ");
	while(token != NULL){
		while(wtmp != NULL){
			comp = strncmp(token, wtmp->value, strlen(token));
		
			if(comp == 0)
				matchTokens++;				
			//else
			//	printf("NO MATCH\n");				

			wtmp = wtmp->next;
		}
		
		comp = 0;
		wtmp = wLL->head;

		numTokens++;
		token = strtok(NULL, " ");
	}
	
	if(numTokens == matchTokens && (strcmp(op, "and")==0))
		return 1;
	else if(matchTokens > 0 && (strcmp(op, "or")==0))
		return 1;
	free(searchTerms);
	
	printf("Your search query does not match our records\n");
	return 0;
}

/* Traverses the list to see if a given file is there or not */
int fileExists(char *file, fileLL files){
	struct fileNode *ptr = files->head;
	int fileComp = 0;
	int exists = 0;

	while(ptr != NULL){
		fileComp = strcmp(ptr->file, file);

		if(fileComp == 0){
			exists = 1;
		}
		ptr = ptr->next;
	}

	return exists;
}

/* 
 * Determines if the file is good or not
 * Use with AND 
 */
int goodBadFile(char *terms, fileLL filezilla, char *op){
	char *searchTerms = createSearchTerms(terms);
	char *searchTerm = strtok(searchTerms, " ");

	int numItems = 0;

	struct fileNode *filefox = filezilla->head;

	int comp = 0;
	int numCorrect = 0;

	while(searchTerm != NULL){
		numItems++;
			
		while(filefox != NULL){
			comp = strncmp(searchTerm, filefox->word, strlen(searchTerm));			
	
			/* 
 			 * IF THE WORD MATCHES THE SEARCHTERM THEN YOU INCREASE THE NUMCORRECT COUNT
 			 * OTHERWISE YOU JUST GO ONTO THE NEXT ONE
 			 */
			if((strcmp(op, "and")==0)){
				if(comp == 0){
					//go to next searchTerm item
					numCorrect++;
					break;
				}	
				//else go to next word in file
			}

			filefox = filefox->next;
		}
		
		comp = 0;
		filefox = filezilla->head;
		searchTerm = strtok(NULL, " ");
	}

	filefox = filezilla->head;

	if(numCorrect == numItems && (strcmp(op, "and")==0))
		return 1;		
	else if(numCorrect != numItems && (strcmp(op, "and")==0))
		return 0;
	else 
		return 0;

}

void processNot(char *terms, wordLL wLL, struct fileNode *filezilla){
	char *searchTerms = createSearchTerms(terms);
	char *token = strtok(searchTerms, " ");

	struct wordNode *wtmp = wLL->head;
	struct fileNode *filefox = filezilla;

	fileLL filelist = NULL;

	int fileComp = 0;

	while(wtmp != NULL){
		printf("\t\t%s\n", wtmp->file);
		wtmp = wtmp->next;
	}

	while(filefox != NULL){
		printf("FILE FROM AND: %s\n", filefox->file);

		while(wtmp != NULL){
			fileNode ftmp = wtmp->files->head;
			while(ftmp != NULL){
				printf("FILE FROM WLL: %s\n", wtmp->file);

				ftmp = ftmp->next;
			}

			wtmp = wtmp->next;
		}

		wtmp = wLL->head;
		filefox = filefox->next;
	}




	while(filefox != NULL){
		printf("filefox->file: %s\n", filefox->file);
		while(wtmp != NULL){
			struct fileNode *ftmp = wtmp->files->head;
			while(ftmp != NULL){
				printf("ftmp->file: %s\n", ftmp->file);
				fileComp = strcmp(filefox->file, ftmp->file);

				if(fileComp != 0){
					printf("%s\n", ftmp->file);
				}

				ftmp = ftmp->next;
			}
			wtmp = wtmp->next;
		}

		wtmp = wLL->head;
		filefox = filefox->next;
	}


/*

	while(token != NULL){
		while(wtmp != NULL){
			struct fileNode *ftmp = wtmp->files->head;
			while(ftmp != NULL){
				fileComp = strcmp(filefox, ftmp->file);

				if(fileComp != 0){
					if(filelist == NULL){
						filelist = (fileLL)malloc(sizeof(struct fileLL));
						filelist->head = createFileNode();
						filelist->head->file = ftmp->file;
						filelist->head->word = wtmp->value;
					}else {
						struct fileNode *tmp = createFileNode();
						tmp->file = ftmp->file;
						tmp->word = wtmp->value;
						tmp->next = filelist->head;
						filelist->head = tmp;
					}

				}		

				ftmp = ftmp->next;
			}

			wtmp = wtmp->next;
		}

		wtmp = wLL->head;
		token = strtok(NULL, " ");
	}
	*/
}

void procNot(char *terms, wordLL wLL){
	char *searchTerms = createSearchTerms(terms);
	char *token = strtok(searchTerms, " ");

	struct wordNode *wtmp = createWordNode();
	wtmp = wLL->head;

	fileLL notFiles = NULL;

	int comp = 0;

	/* Gets all the files affiliated with the token you don't want */
	while(token != NULL){
		while(wtmp != NULL){
			comp = strncmp(token, wtmp->value, strlen(token));
			if(comp == 0){
				struct fileNode *ftmp = wtmp->files->head;
				while(ftmp != NULL){
					if(notFiles == NULL){
						notFiles = (fileLL)malloc(sizeof(struct fileLL));
						notFiles->head = createFileNode();
						notFiles->head->file = ftmp->file;
						notFiles->head->word = wtmp->value;
					}else {
						struct fileNode *tmp = createFileNode();
						tmp->file = ftmp->file;
						tmp->word = wtmp->value;
						tmp->next = notFiles->head;
						notFiles->head = tmp;
					}
					ftmp = ftmp->next;
				}
			}
			wtmp = wtmp->next;
		}
		wtmp = wLL->head;
		token = strtok(NULL, " ");
	}

	searchTerms = createSearchTerms(terms);
	token = strtok(searchTerms, " ");

	struct fileNode *ftmp = notFiles->head;
	int isFile = 0;

	wtmp = wLL->head;

	while(ftmp != NULL){
		while(wtmp != NULL){
			struct fileNode *fff = wtmp->files->head;
			while(fff != NULL){
				comp = strcmp(ftmp->file, fff->file);
				if(comp == 0){
					fff = fff->next;
				}else {
					ftmp = ftmp->next;
				}
			}
			wtmp = wtmp->next;
		}

		ftmp = ftmp->next;
	}

	wtmp = wLL->head;

	fileLL notList = NULL;
	while(wtmp != NULL){
		while(ftmp != NULL){
			struct fileNode *tmp = wtmp->files->head;
			while(tmp != NULL){
				printf("\t%s\n", tmp->file);
				printf("\t%s\n", ftmp->file);
				comp = strncmp(tmp->file, ftmp->file, strlen(tmp->file));

				if(comp != 0){
						if(notList == NULL){
							notList = (fileLL)malloc(sizeof(struct fileLL));
							notList->head = createFileNode();
							notList->head->file = tmp->file;
						}else {
							if(fileExists(tmp->file, notList) == 0){
								struct fileNode *tmp2 = createFileNode();
								tmp2->file = tmp->file;
								tmp2->next = notList->head;
								notList->head = tmp2;
							}
						}
				}

				tmp = tmp->next;
			}

			ftmp = ftmp->next;
		}

		ftmp = notFiles->head;
		wtmp = wtmp->next;
	}

	ftmp = notList->head;
	while(ftmp != NULL){
		printf("\t\t\t%s\n", ftmp->file);

		ftmp = ftmp->next;
	}
}

fileLL processOr(char *terms, wordLL wLL){
	fileLL files = NULL;

	if(inFile(terms, "or", wLL) == 0)
		return NULL;

	/* Make a copy of the terms so you don't mess it up */
	char *searchTerms = createSearchTerms(terms);

	/* Tokenize the searchTerms */
	char *token = strtok(searchTerms, " ");

	struct wordNode *wtmp = wLL->head;

	int wordComp = 0;

	while(token != NULL){
		while(wtmp != NULL){
			wordComp = strncmp(token, wtmp->value, strlen(token));

			if(wordComp == 0){
				struct fileNode *ftmp = wtmp->files->head;
				while(ftmp != NULL){
					if(files == NULL){
						files = (fileLL)malloc(sizeof(fileLL));
						files->head = createFileNode();
						files->head->file = ftmp->file;
						files->head->word = wtmp->value;
					}else {
						/* Check if the file already exists in files */
						if(fileExists(ftmp->file, files) == 0){
							struct fileNode *tmp = createFileNode();
							tmp->file = ftmp->file;
							tmp->word = wtmp->value;
							tmp->next = files->head;
							files->head = tmp;
						}
					}

					ftmp = ftmp->next;
				}
			}

			wtmp = wtmp->next;
		}

		wtmp = wLL->head;
		token = strtok(NULL, " ");
	}

	fileNode tmp = files->head;
	printf("\n");
	while(tmp != NULL){
		if(strcmp(tmp->file, "</list>")!=0){
			//printf("*********************************\n");
			//printf("GOOD FILE: %s\n", tmp->file);
			printf("%s\n", tmp->file);
		}
		tmp = tmp->next;
	}

	return files;
}

void processAnd(char *terms, wordLL wLL, char *op){
	/* USED THIS TO RESET reducedFiles */
	static const fileLL emptyfileLL = NULL;
		
	/* MAKE A COPY OF THE TERMS */
	char *searchTerms = createSearchTerms(terms);

	/* TOKENIZE SEARCHTERMS */
	char *tokens = strtok(searchTerms, " ");

	/* MAKE A COPY OF wLL->head FOR ITERATION */
	struct wordNode *wtmp = wLL->head;

	/* THIS HOLDS THE DISTINCT FILES THAT CORRESPOND TO THE SEARCHTERMS */
	fileLL list = NULL;

	/* 
 	 * COMP WILL BE USED WITH STRNCMP OR STRCMP
	 */ 
	int comp = 0;

	/*
 	 * Get all the files that correspond to the token
 	 * Add to list if found
 	 */ 	 
	while(tokens != NULL){
		while(wtmp != NULL){
			comp = strncmp(tokens, wtmp->value, strlen(tokens));

			/* IF THE WORD MATCHES THE TOKEN CHECK THE FILES */
			if(comp == 0){
				struct fileNode *ftmp = wtmp->files->head;
				while(ftmp != NULL){
					/* 
 					 * IF THE LIST IS EMPTY, CREATE A NEW ONE
 					 * OTHERWISE ADD TO THE FRONT 
 					 */	 	
					if(list == NULL){
						/* IF THE LIST IS EMTPY INIT THE LIST */
						list = (fileLL)malloc(sizeof(fileLL));

						/* PROCEED TO ADD TO THE HEAD OF THE LIST */
						list->head = createFileNode();
						list->head->file = ftmp->file;
						
						list->head->wnode = createWordNode();
						list->head->wnode->value = wtmp->value;
					}else {
						/* 
 						 * IF THE LIST IS ALREADY INITIALIZED 					
 						 * FIND ALL DISTINCT FILE NAMES  
 						 */

						/* IF FILE DOES NOT EXIST IN THE LIST, ADD TO LIST */
						if(fileExists(ftmp->file, list) == 0){
							struct fileNode *tmp = createFileNode();
							tmp->file = ftmp->file;
							tmp->wnode = createWordNode();
							tmp->wnode->value = wtmp->value;
							tmp->next = list->head;
							list->head = tmp;
						}
					}

					ftmp = ftmp->next;
				}
			}

			wtmp = wtmp->next;
		}

		wtmp = wLL->head;
		tokens = strtok(NULL, " ");
	}

	/* RESET WTMP TO THE HEAD OF THE WORD LIST */
	wtmp = wLL->head;
	struct fileNode *listHead = list->head;
	int fileComp = 0;
	int wordComp = 0;

	char *search2 = createSearchTerms(terms);
	char *tokens2 = strtok(search2, " ");

	/* fileNames regroups the words by file (maybe...I forgot what I coded here actually) */
	fileLL fileNames = NULL;

	/* REARRANGE TO BE SORTED BY FILENAME RATHER THAN THE WORD */
	while(tokens2 != NULL){
		while(listHead != NULL){
			while(wtmp != NULL){
				wordComp = strncmp(tokens2, wtmp->value, strlen(tokens2));
			
				if(wordComp == 0){
					struct fileNode *ftmp = wtmp->files->head;
					while(ftmp != NULL){
						fileComp = strncmp(listHead->file, ftmp->file, strlen(listHead->file));					
						if(fileComp == 0){	
							/* IF THE WORD MATCHES THE TOKEN, ADD THE FILE TO A NEW LIST WITH THE WORD */
							if(fileNames == NULL){
								fileNames = (fileLL)malloc(sizeof(fileLL));
								fileNames->head = createFileNode();
								fileNames->head->file = ftmp->file;
								fileNames->head->word = wtmp->value;
							}else {
								struct fileNode *tmp = createFileNode();
								tmp->file = ftmp->file;
								tmp->word = wtmp->value;
								tmp->next = fileNames->head;
								fileNames->head = tmp;
							}
						}
	
						ftmp = ftmp->next;
					}
				}
				wtmp = wtmp->next;
			}
		
			wtmp = wLL->head;
			listHead = listHead->next;
		}
		tokens2 = strtok(NULL, " ");
		listHead = list->head;
	}
	
	/* HERE WE ACTUALLY CHECK TO SEE IF THE FILE IS GOOD OR NOT */

	/* 
 	 * HOLDS THE WORDS OF ONE FILE
 	 * GETS RESET AFTER A WORD HAS BEEN VISITED
 	 */  
	fileLL reducedFiles = NULL;
	
	/* BASICALLY SENDS ALL THE WORDS OF A SPECIFIC FILE TO GOODBADFILE()
 	 * 
 	 * listmover points to the head of fileNames and moves down the list
 	 * listmoverptr points to whatever listmover points to and moves down the list
 	 * 
 	 * Then you compare the file of listmover and listmoverptr 
 	 * 	match -> add to reducedFiles
 	 *
 	 * reducedFiles gets resetted when listmoverptr finishes iterating and
 	 * the list is passed into goodBadFile to be judged
	 */ 	

	fileLL holdforNot = NULL;
	 
	struct fileNode *listmover = fileNames->head;
	while(listmover != NULL){
		struct fileNode *listmoverptr = listmover;
		while(listmoverptr != NULL){
			if((strncmp(listmoverptr->file, listmover->file, strlen(listmoverptr->file)) == 0)){
				if(reducedFiles == NULL){
					reducedFiles = (fileLL)malloc(sizeof(fileLL));
					reducedFiles->head = createFileNode();
					reducedFiles->head->file = listmoverptr->file;
					reducedFiles->head->word = listmoverptr->word;
				}else {
					struct fileNode *rtmp = createFileNode();
					rtmp->file = listmoverptr->file;
					rtmp->word = listmoverptr->word;
					rtmp->next = reducedFiles->head;
					reducedFiles->head = rtmp;
				}
			}
			listmoverptr = listmoverptr->next;
		}
		
		printf("\n");
		if(goodBadFile(terms, reducedFiles, "and") == 1){
			//printf("*************************************************************\n");
			//printf("GOOD FILE: %s\n", reducedFiles->head->file);

			if(strcmp(op, "not") == 0){
				if(holdforNot == NULL){
					holdforNot = (fileLL)malloc(sizeof(struct fileLL));
					holdforNot->head = createFileNode();
					holdforNot->head->file = reducedFiles->head->file;
				}else {
					struct fileNode *fff = createFileNode();
					fff->file = reducedFiles->head->file;
					fff->next = holdforNot->head;
					holdforNot->head = fff;
				}
			}else if(strcmp(op, "and") == 0)
				printf("%s\n", reducedFiles->head->file);
		}

		reducedFiles = emptyfileLL;

		listmover = listmover->next;
	}

	//processNot(terms, wLL, holdforNot->head);
}

/*
 * This currently does not(a or b)
 */
void processNotOr(char *terms, wordLL wLL){
	char *searchTerms = createSearchTerms(terms);
	char *token = strtok(searchTerms, " ");
		
	struct wordNode *wtmp = wLL->head;
/*
	int wordComp = 0;
	int fileComp = 0;
*/
	/* 
 	 * FILL THIS UP WITH THE TOKENS/FILES
 	 * CHECK ALL OTHER WORDS/FILES AGAINST THIS
 	 */
//	fileLL notTerms = NULL;

	/*
 	 * Get all the files associated with those words 
 	 */ 
/*	while(token != NULL){
		while(wtmp != NULL){
			wordComp = strncmp(token, wtmp->value, strlen(token));
		
			struct fileNode *ftmp = wtmp->files->head;
			while(ftmp != NULL){
*/				/* 
 				 * wordComp == 0 --> notTerm 
 			 	 * 	add to notTerms list	 
			 	 */
/*				if(wordComp == 0){
					if(notTerms == NULL){
						notTerms = (fileLL)malloc(sizeof(fileLL));
						notTerms->head = createFileNode();
						notTerms->head->file = ftmp->file;	
						notTerms->head->word = wtmp->value;
					}else {
						struct fileNode *ftmp2 = createFileNode();
						ftmp2->file = ftmp->file;
						ftmp2->word = wtmp->value;
						ftmp2->next = notTerms->head;
						notTerms->head = ftmp2;
					}
				}

				ftmp = ftmp->next;
			}

			wtmp = wtmp->next;
		}	
	
		wordComp = 0;
		wtmp = wLL->head;
		token = strtok(NULL, " ");
	}

	wtmp = wLL->head;

	searchTerms = createSearchTerms(terms);
	token = strtok(searchTerms, " ");

	fileLL semiFinalList = NULL;
	struct fileNode *nottmp = notTerms->head;
*/
	/*
 	 * Here find all the files that match notTerms
 	 * Append those files that don't match to another fileLL
 	 */ 
/*	while(nottmp != NULL){
		while(wtmp != NULL){
			struct fileNode *ftmp = wtmp->files->head;
			while(ftmp != NULL){
				wordComp = strncmp(nottmp->word, wtmp->value, strlen(nottmp->word));
				fileComp = strcmp(nottmp->file, ftmp->file);
*/
				/* 
 				 * If the files are NOT the same
 				 * add them to finalList
 				 */ 
/*				if(fileComp != 0){
					if(semiFinalList == NULL){
						semiFinalList = (fileLL)malloc(sizeof(fileLL));
						semiFinalList->head = createFileNode();	
						semiFinalList->head->file = ftmp->file;
						semiFinalList->head->word = wtmp->value;
					}else {
*/						/* 
 						 * Need to traverse to see if the file has already been added 
 						 * Sets a flag if it exists
						 */
/*
						if(fileExists(ftmp->file, semiFinalList) == 0){
							struct fileNode *tmp = createFileNode();
							tmp->file = ftmp->file;
							tmp->word = wtmp->value;
							tmp->next = semiFinalList->head;
							semiFinalList->head = tmp;
						}
					}
				}
			
				ftmp = ftmp->next;
			}	
	
			wtmp = wtmp->next;
		}

		wtmp = wLL->head;
		nottmp = nottmp->next;
	}

	fileLL finalList = NULL;

	struct fileNode *ftmp = semiFinalList->head;
	printf("\n\n\n*****\tfinalList\t*****\n");
	while(ftmp != NULL){
		//printf("WORD: %s\tFILE: %s\n", ftmp->word, ftmp->file);
		printf("file: %s\n", ftmp->file);
		ftmp = ftmp->next;
	}	
*/
	free(searchTerms);

}

/* NOTE DO NOT CHANGE wLL SINCE IT IS A GLOBAL VARIABLE*/
fileLL andSearch(char *terms, wordLL wLL){
	fileLL files = NULL;
    	//printf("Entered andSearch()\n");
        //printf("terms received in andSearch(): %s\n", terms);

	/*
 	 * If one term in the search query doesn't exist then the whole case fails
 	 * Thus we use inFile to see if wLL has the word or not
 	 */ 
	if(inFile(terms, "and", wLL) != 1)
		return NULL;

	processAnd(terms, wLL, "and");

	return files;
}

fileLL orSearch(char* terms, wordLL wLL){
	fileLL files = NULL;

	//printf("Entered orSearch()\n");
    //printf("terms received in orSearch(): %s\n", terms);

	processOr(terms, wLL);

    return files;
}




/****extra credit**** 
 *     find all files which don't have a word
 *             */
fileLL notOrSearch(char* terms, wordLL wLL){
	fileLL files =NULL;

	procNot(terms, wLL);
	//processNotOr(terms, wLL);

	//printf("Entered notOrSearch()\n");
	//printf("terms received in notSearch(): %s\n", terms);

    	return files;
}

fileLL notAndSearch(char* terms, wordLL wLL){
	fileLL files =NULL;

	//processNotAnd(terms);

	//printf("Entered notSearch()\n");
	//printf("terms received in notAndSearch(): %s\n", terms);

    	return files;
}
