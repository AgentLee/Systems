#include "search.h"
#include "andSearch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void processAnd(char *terms){
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
	
		if(goodBadFile(terms, reducedFiles, "and") == 1){
			//printf("*************************************************************\n");
			//printf("GOOD FILE: %s\n", reducedFiles->head->file);
			printf("%s\n", reducedFiles->head->file);
		}

		reducedFiles = emptyfileLL;

		listmover = listmover->next;
	}
}