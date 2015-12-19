#include "search.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* Global Variables */
wordLL wLL=NULL;
fileLL fLL=NULL;

char *changeToLower(char *terms){
    int i;
    for(i = 0; i < strlen(terms); i++){
        terms[i] = tolower(terms[i]);
    }

    return terms;
}

/* reads through the index and stores into memory */
wordLL readIndex(char* index){

		if(index == NULL){
			fprintf(stderr, "ERROR: invalid index name\n");
			return NULL;
		}

        FILE *fp = fopen(index, "r");
        if(!fp){
            fprintf(stderr, "ERROR: cannot open file\n");
            return NULL;
        }

        /* The first word in the list */
        char* l = (char*)malloc(BUFFER*sizeof(char)+1);
        char* curr = fgets(l, BUFFER, fp);
        

        if(curr==NULL){
            fprintf(stderr, "ERROR: file is empty\n");
            return NULL;
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
               // printf("curr = %s\n\n", curr);

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

    printAllNodes(wLL);
    free(l);
    return wLL;
}


/* Prints the words and the files associated with them */
void printAllNodes(wordLL w){

	wordNode currWord = NULL;
	fileNode currFile = NULL;
	/* printing the words */
	for(currWord = w->head; currWord; currWord = currWord->next){
		printf("\nCurrent word is: %s\nFiles are:\n", currWord->value);
		for(currFile = currWord->files->head; currFile; currFile = currFile->next){
			if(currFile->wordCount!=0) printf("\t%s (count: %d)\n", currFile->file, currFile->wordCount);
		}
	}
}


/* Prints the output fileLL */
void printOutput(fileLL output){
	if(output == NULL){
		printf("No words found.\n");
		return;
	}

	fileNode currFile = NULL;

	printf("Word found in files:\n");
	for(currFile = output->head; currFile; currFile = currFile->next){
			if(currFile->wordCount!=0) printf("\t%s \n", currFile->file);
		}


	return;
}


void printMenu(){
	printf("\n------------------\nFor And Search Enter: sa <sequence of terms>\nFor Or Search Enter: so <sequence of terms>\nFor Not Or Search Enter: sno <sequence of terms>\nFor Not And Search Enter: sna <sequence of terms>\n");
	printf("To quit enter: q\n------------------\n\n");
	return;

}


void promptUser(){
  
	char* input = (char*)malloc(BUFFER);
			
		printMenu();

		fflush(stdin);
            scanf(" %[^\n]s", input);
            //printf("input = %s\n", input);

            if(input == NULL){

            }

            char* option = (char*)malloc(BUFFER);
            option=strtok(input, " ");
            //printf("option chosen is: %s\n", option);

            if(strcmp(option, "q") == 0){
            	printf("Quiting program. Goodbye!\n");
            	exit(0);
            }
            
            char* terms = (char*)malloc(BUFFER);
            terms = strtok(NULL, "");
            changeToLower(terms);

            if(strcmp(option, "sa") == 0){
            	andSearch(terms, wLL);
            }
            else if(strcmp(option, "so")==0){
            	orSearch(terms, wLL);
            }
            else if(strcmp(option, "sno")==0){
            	notOrSearch(terms, wLL);
            }
            else if(strcmp(option, "sna")==0){
            	printf("Calling notAndSearch(terms)\n");
            } else{
            	fprintf(stderr, "ERROR: Incorrect Format\n");
            }

}


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
    if(readIndex(argv[1])==NULL) return -1; 

    /*Prompt user on what they would like to do*/
	while(1){
		promptUser();
	}    


	//else askUser();
    return 0;

}

