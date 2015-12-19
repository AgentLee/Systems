/*
 * CS214 Assignment 4
 *
 * Jonathan Lee (jl1424)
 * Niharika Nagar (nn164)
 */

#include "search.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

wordLL wLL=NULL;
fileLL fLL=NULL;

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

void printAllNodes(wordLL w){
    wordNode currWord = NULL;
    fileNode currFile = NULL;
    /* printing the words */
    for(currWord = w->head; currWord; currWord = currWord->next){
        printf("Current word is: %s\n\tFiles are:\n", currWord->value);
        for(currFile = currWord->files->head; currFile; currFile = currFile->next){
            printf("\t%s (count: %d)\n", currFile->file, currFile->wordCount);
        }
    }   
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

/* reads through the index and stores into memory */
int readIndex(char* index){
        FILE *fp = fopen(index, "r");
        if(!fp){
            fprintf(stderr, "ERROR: cannot open file\n");
            return -1;
        }

        /* The first word in the list */
        char* l = (char*)malloc(BUFFER*sizeof(char)+1);
        char* curr = fgets(l, BUFFER, fp);
        

        if(curr==NULL){
            fprintf(stderr, "ERROR: file is empty\n");
            return -1;
        }

        wordNode currWord=NULL;
        fileNode currFile=NULL;

        while(curr!=NULL){

            /* If at the end of the list */
            if(strcmp(curr, "</list>")==0){
                curr = curr + 7;
                if(curr == NULL){
                    curr = fgets(l, BUFFER, fp);
                    continue;
                }
            }
            /* Anything after <list> is a wordNode 
 *                 - only want to read the word into memory 
 *                                 not "<list>*/
            if(strstr(curr, "<list>")){
                curr = curr + 7;
                //printf("curr = %s\n\n", curr);

                /* if the word is not null after space is removed */
                if(strtok(curr, " \n")){
                    wordNode word = createWordNode();
                    word->value = (char*)malloc(sizeof(char)*(strlen(curr)+1));
                    strcpy(word->value, curr);

                    /* if word linked list has not been created */
                    if(wLL==NULL){
                        wLL = (wordLL)malloc(sizeof(struct wordLL));
                        wLL->head = word;
                        currWord = wLL->head;
                    }
                    /* if linked list has been created already */
                    else{

                        currWord->next= word;
                        currWord = currWord->next;
                    }

                    /* Next line after "<list> [word]" will be files AND counts!*/
                    curr = fgets(l, BUFFER, fp);

                    /* reading each token in line separately */
                    char* curr2 = strtok(curr, " \n");
                    int i = 0;
                    while(curr2!=NULL){
                        fileNode f = createFileNode();
                        f->file = (char*)malloc(sizeof(char)*(strlen(curr2)+1));
                        strcpy(f->file, curr2);

                        /* if the file linked list is not created, create it and link it to the current word */
                        if(currWord->files == NULL){
                            fLL = (fileLL) malloc(sizeof(struct fileLL));
                            fLL->head = f; 
                            currFile = fLL->head;
                            currWord->files = fLL;

                        }
                        /* otherwise the file linked list is already created for that word and must add to the next file */
                        else{
                            currFile->next = f;
                            currFile = currFile->next;
                        }

                        i++;
                        curr2 = strtok(NULL, " \n");  
                        if(curr2 == NULL) break;
                        /* This will be the count!*/
                        currFile->wordCount = atoi(curr2);
                        curr2 = strtok(NULL, " \n");
                    }                   
                    
                    
                }   

            }

            curr = fgets(l,BUFFER,fp);
        }

    //printAllNodes(wLL);
    free(l);
    return 1;
}

char *changeCase(char *terms){
	int i;
	for(i = 0; i < strlen(terms); i++){
		terms[i] = tolower(terms[i]);
	}

	return terms;
}

/*
 * This function is useful for AND
 * If a word doesn't exist at all in any of the files then AND fails
 */
int inFile(char *terms, char *op){
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
	
	if(numTokens == matchTokens && op == "and")
		return 1;
	else if(matchTokens > 0 && op == "or")
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
int goodBadFile(char *terms, fileLL filezilla){
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
			//if(op == "and"){
				if(comp == 0){
					//go to next searchTerm item
					numCorrect++;
					break;
				}	
				//else go to next word in file
			//}
			filefox = filefox->next;
		}
		
		comp = 0;
		filefox = filezilla->head;
		searchTerm = strtok(NULL, " ");
	}

	filefox = filezilla->head;

	if(numCorrect == numItems)// && op == "and")
		return 1;		
	else if(numCorrect != numItems)// && op == "and")
		return 0;
}

fileLL processOr(char *terms){
	fileLL files = NULL;

	if(inFile(terms, "or") == 0)
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
						files = (fileLL)malloc(sizeof(struct fileLL));
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

	struct fileNode *tmp = files->head;
	while(tmp != NULL){
		//printf("*********************************\n");
		//printf("GOOD FILE: %s\n", tmp->file);
		if(tmp->file == " ")
			printf("NULL FILE\n");
		else
		printf("WORD: %s\tFILE: %s\n", tmp->word, tmp->file);

		tmp = tmp->next;
	}

	return files;
}

void processNot(char *terms, struct fileNode *filefox){
	char *searchTerms = createSearchTerms(terms);
	char *token = strtok(searchTerms, " ");

	struct wordNode *wtmp = wLL->head;
	int fileComp = 0;

	while(token != NULL){
		while(wtmp != NULL){
				struct fileNode *ftmp = wtmp->files->head;
				while(ftmp != NULL){
					fileComp = strcmp(filefox->file, ftmp->file);

					if(fileComp != 0){
						//file is good because you're passing in a file 
						//that has all the terms
						printf("NOT %s\n", ftmp->file);
					}

					ftmp = ftmp->next;
				}

			wtmp = wtmp->next;
		}
		token = strtok(NULL, " ");
	}
}

/*******************************************/
			/* AND WAS HERE */
/*******************************************/
void processAnd(char *terms, char *op){
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
						list = (fileLL)malloc(sizeof(struct fileLL));

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
								fileNames = (fileLL)malloc(sizeof(struct fileLL));
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
 	 * 		match -> add to reducedFiles
 	 *
 	 * reducedFiles gets resetted when listmoverptr finishes iterating and
 	 * the list is passed into goodBadFile to be judged
	 */ 	
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
	
		/* add another condition for xor at in this function
		 * if it's a good file and xor -> go to another function and 
		 * reprocess wLL and choose those files not 
		 */	
		if(goodBadFile(terms, reducedFiles) == 1){
			//printf("*************************************************************\n");
			//printf("GOOD FILE: %s\n", reducedFiles->head->file);
			printf("hi\n");
			if(op == "not"){
				printf("heere\n");
				processNot(terms, reducedFiles->head);
			}else
				printf("%s\n", reducedFiles->head->file);
		}

		reducedFiles = emptyfileLL;

		listmover = listmover->next;
	}
}



/*
 * This currently does not(a or b)
 */
void processNotOr(char *terms){
	char *searchTerms = createSearchTerms(terms);
	char *token = strtok(searchTerms, " ");
		
	struct wordNode *wtmp = wLL->head;

	int wordComp = 0;
	int fileComp = 0;

	/* 
 	 * FILL THIS UP WITH THE TOKENS/FILES
 	 * CHECK ALL OTHER WORDS/FILES AGAINST THIS
 	 */
	fileLL notTerms = NULL;

	/* Get all the files associated with those words */ 
	while(token != NULL){
		while(wtmp != NULL){
			wordComp = strncmp(token, wtmp->value, strlen(token));
		
			struct fileNode *ftmp = wtmp->files->head;
			while(ftmp != NULL){
				/* wordComp == 0 --> notTerm 
 			 	 * 	add to notTerms list */
				if(wordComp == 0){
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

	/* ALL THE CORRECT FILES ARE HERE (the files with the not terms) */
	struct fileNode *tmp = notTerms->head;
	while(tmp != NULL){
		printf("%s\n", tmp->file);
		tmp = tmp->next;
	}

	wtmp = wLL->head;

	searchTerms = createSearchTerms(terms);
	token = strtok(searchTerms, " ");

	fileLL noNots = NULL;






/*	while(token != NULL){
		while(wtmp != NULL){
			struct fileNode *ftmp = wtmp->files->head;
			while(ftmp != NULL){
				wordComp = strncmp(token, wtmp->value, strlen(token));

				if(wordComp == 0){
					//skip
				}else if(wordComp != 0){
					//check the file
					tmp = notTerms->head;
					while(tmp != NULL){
						fileComp = strcmp(tmp->file, ftmp->file);

						if(fileComp == 0){
							//skip
						}else {
							//add to list
							if(noNots == NULL){
								noNots = (fileLL)malloc(sizeof(struct fileLL));
								noNots->head = createFileNode();
								noNots->head->file = ftmp->file;
								noNots->head->word = wtmp->value;
							}else {
								struct fileNode *tmp2 = createFileNode();
								tmp2->file = ftmp->file;
								tmp2->word = wtmp->value;
								tmp2->next = noNots->head;
								noNots->head = tmp2;
							}
						}
						tmp = tmp->next;
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
	printf("LOOK HERE\n");

/*
	struct fileNode *finalHead = noNots->head;
	while(finalHead != NULL){
		printf("%s\n", finalHead->file);
		finalHead = finalHead->next;
	}
*/

















	fileLL semiFinalList = NULL;
	struct fileNode *nottmp = notTerms->head;

	/*
 	 * Here find all the files that match notTerms
 	 * Append those files that don't match to another fileLL
 	 */ 
	while(nottmp != NULL){
		while(wtmp != NULL){
			struct fileNode *ftmp = wtmp->files->head;
			while(ftmp != NULL){
				wordComp = strncmp(nottmp->word, wtmp->value, strlen(nottmp->word));
				fileComp = strcmp(nottmp->file, ftmp->file);

				/* 
 				 * If the files are NOT the same
 				 * add them to finalList
 				 */ 
				if(fileComp != 0){
					if(semiFinalList == NULL){
						semiFinalList = (fileLL)malloc(sizeof(fileLL));
						semiFinalList->head = createFileNode();	
						semiFinalList->head->file = ftmp->file;
						semiFinalList->head->word = wtmp->value;
					}else {
						/* 
 						 * Need to traverse to see if the file has already been added 
 						 * Sets a flag if it exists
						 */

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

	free(searchTerms);

}

/* NOTE DO NOT CHANGE wLL SINCE IT IS A GLOBAL VARIABLE*/
fileLL andSearch(char *terms){
	fileLL files = NULL;
    	printf("Entered andSearch()\n");
        printf("terms received in andSearch(): %s\n", terms);

	/*
 	 * If one term in the search query doesn't exist then the whole case fails
 	 * Thus we use inFile to see if wLL has the word or not
 	 */ 
	if(inFile(terms, "and") != 1)
		return NULL;

	processAnd(terms, "and");

	return files;
}

fileLL orSearch(char* terms){
	fileLL files = NULL;

	printf("Entered orSearch()\n");
    printf("terms received in orSearch(): %s\n", terms);

	processOr(terms);

    return files;
}

/****extra credit**** 
 *     find all files which don't have a word
 *             */
fileLL notSearch(char* terms){
	fileLL files =NULL;

	processAnd(terms, "not");

	printf("Entered notSearch()\n");
	printf("terms received in notSearch(): %s\n", terms);

    	return files;
}

void promptUser(){
    int option = 0;
    char terms[BUFFER];

    //terms = (char *)malloc(BUFFER);

    while(option!=4){

        printf("\n\nOptions (ENTER A NUMBER)\n------------\n1. 'and' Search - search for a file containing multiple words\n2. 'or' Search - search for a file containing at least one of a list of words\n3. 'not' Search - search for a file not containing a word\n4. 'q' - Quit the program\n\n\n");
        scanf("%d", &option);
        


        switch(option){

            /* 'and' of terms */
            case 1:
                /* prompt user to enter terms*/
                printf("\nEnter the terms you would like to search:\n");
                
                /* why must this be called twice??? */
                fgets(terms, BUFFER, stdin);
                fgets(terms, BUFFER, stdin);
                changeCase(terms);
                andSearch(terms);
                break;
            
            /* 'or' terms */
            case 2:
                /* prompt user to enter terms*/
                printf("\nEnter the terms you would like to search:\n");
                
                /* why must this be called twice???*/
                fgets(terms, sizeof(terms), stdin);
                fgets(terms, sizeof(terms), stdin);
	
                if(terms == NULL){
                    fprintf(stderr, "ERROR: no terms entered.\n");
                    break;
                }
                changeCase(terms);
                orSearch(terms);
                break;
            
            case 3: 
                /* prompt user to enter terms*/
                printf("\nEnter the terms you would like to search:\n");
                
                /* why must this be called twice???*/
                fgets(terms, sizeof(terms), stdin);
                fgets(terms, sizeof(terms), stdin);

                if(terms == NULL){
                    fprintf(stderr, "ERROR: no terms entered.\n");
                    break;
                }
                changeCase(terms);
                notSearch(terms);
                break;
            /* q:  quit the program*/
            case 4:
			   // free(terms);

                return;
            
            default:
                fprintf(stderr, "ERROR: invalid option\nMust choose options from 1-4\n");

                break;
        
        }
    }

    //free(terms);
}

/*
void askUser() {
char* input;
	char* command;
	int op;
	TokenizerT* tk;
	input = malloc(BUFSIZ);
	memset(input,0,BUFSIZ);

	for(;;) {
        printf("Enter command: ");
        fflush(stdin);
		scanf(" %[^\n]s", input);
		if(strcmp(input,"q")==0) {
			printf("User exiting\n");
			exit(1);
		}
        
		tk = TKCreate(input);
		if(tk==0) {
			printf("Error in tokenizing input.\n");
			return 1;
		}
        
		command = TKGetNextToken(tk);
		if(strcmp(command,"sa")==0) {
			op = 1;
		}else if(strcmp(command,"so")==0) {
			op = 2;
		}else{
			printf("Error: Invalid command.\n");
            return 1;
		}
        
		printf("Searching for %s, op = %d\n",input,op);
        optionChoice(op);
	}
	return 0;
}
*/

int main(int argc, char** argv){
    
    /* Incorrect number of arguments */
    if(argc != 2){
        if(argc == 1){
            fprintf(stderr, "ERROR: Too few arguments.\n");
        }
        if(argc > 2){
            fprintf(stderr, "ERROR: Too many arguments.\n");
        }

        printf("Should be of the form:\n\t\"%s <inverted index file name>\"\n", argv[0]);
        return -1;
    }

    /* Check if second argument is file*/
    if(isFile(argv[1])==0){
        fprintf(stderr, "ERROR: Not a file\n");
        return -1;
    }

    /**/
    	if(readIndex(argv[1])==-1) return -1; 

    /*Prompt user on what they would like to do*/
    	else promptUser();
	//else askUser();
    return 0;

}