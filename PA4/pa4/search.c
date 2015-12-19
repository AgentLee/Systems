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
    printf("returning from createWordNode()\n");
    return w;
}

/* Creating fileNode to store files inside each wordNode */
fileNode createFileNode(){
    fileNode f = (fileNode) malloc(sizeof(struct fileNode));
    f->file = NULL;
    return f;
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
                printf("curr = %s\n\n", curr);

                /* if the word is not null after space is removed */
                if(strtok(curr, " ")){
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
                    char* curr2 = strtok(curr, " ");
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
                        curr2 = strtok(NULL, " ");  
                        if(curr2 == NULL) break;
                        /* This will be the count!*/
                        currFile->wordCount = atoi(curr2);
                        curr2 = strtok(NULL, " ");
                    }                   
                    
                    
                }   

            }

            curr = fgets(l,BUFFER,fp);
        }

    printAllNodes(wLL);
    printf("wLL->head =  %s\n", wLL->head->value );
    free(l);
    return 1;
}

int processWord(char *word, char *searchLine){
	char *searchLineCopy = (char *)malloc(strlen(searchLine) * sizeof(char));
	strcpy(searchLineCopy, searchLine);
	printf("searchLineCopy: %s\n", searchLineCopy);
	char *currWord = strtok(searchLineCopy, " ");
	int i = 0;

	while(currWord != NULL){
		printf("*******currWord: %s\tword:%s********\n", currWord, word);

		if(strcmp(word, currWord) == 0){
			printf("word:::::%s\n", word);
			printf("******Match******\n");
			return 1;
		}else if(strcmp(word, currWord) != 0){
			printf("****FUCK*****\n");
		}

		currWord = strtok(NULL, " ");
	}

	return 0;
}

/* NOTE DO NOT CHANGE wLL SINCE IT IS A GLOBAL VARIABLE*/
fileLL andSearch(char* terms){
	fileLL files = NULL;
    fileNode ftmp;
    printf("Entered andSearch()\n");
        printf("terms received in orSearch(): %s\n", terms);

    struct wordNode *temp = wLL->head;              
    int i;  

    ftmp = createFileNode();
    printf("created node\n");
    ftmp->file = "";

    while(temp != NULL){
        i = processWord(temp->value, terms);
        printf("dksjflds\n");
        
        if(i == 1){
            printf("match here ****\n");
            ftmp->file = temp->files->head->file;
            printf("*********file name\n");
            if(files == NULL){
                files = (struct fileLL*)malloc(sizeof(struct fileLL));
                files->head = createFileNode();
                files->head = ftmp;
            }else {
                fileLL fileTMP = (struct fileLL*)malloc(sizeof(struct fileLL));
                fileTMP = files;
                
                while(fileTMP != NULL){
                    if(fileTMP->head->next == NULL){
                        fileTMP->head->next = createFileNode();
                        fileTMP->head->next->file = temp->files->head->file;
                    }else
                        fileTMP = fileTMP->head->next;
                }
            }
        }

        temp = temp->next;
    }

    printf("files head: %s\n", files->head->file);
        return files;
}

fileLL orSearch(char* terms){
	fileLL files = NULL;
	fileNode ftmp;
	printf("Entered orSearch()\n");
    	printf("terms received in orSearch(): %s\n", terms);

	struct wordNode *temp = wLL->head;				
	int i;	

	ftmp = createFileNode();
	printf("created node\n");
	ftmp->file = "";

	while(temp != NULL){
		i = processWord(temp->value, terms);
		printf("dksjflds\n");
		
		if(i == 1){
			printf("match here ****\n");
			ftmp->file = temp->files->head->file;
			printf("*********file name\n");
			if(files == NULL){
				files = (struct fileLL*)malloc(sizeof(struct fileLL));
				files->head = createFileNode();
				files->head = ftmp;
			}else {
				fileLL fileTMP = (struct fileLL*)malloc(sizeof(struct fileLL));
				fileTMP = files;
				
				while(fileTMP != NULL){
					if(fileTMP->head->next == NULL){
						fileTMP->head->next->file = temp->files->head->file;
					}else{
                        fileTMP->head->next = createFileNode();
						fileTMP = fileTMP->head->next;
                    }
				}
			}
		}

		temp = temp->next;
	}

	fileLL fileTMP = (struct fileLL*)malloc(sizeof(struct fileLL));
    fileTMP = files; 

    while(fileTMP != NULL){
        printf("file: %s\n", fileTMP->head->file);
        fileTMP = fileTMP->head->next;
    }

    return files;
}

/****extra credit**** 
 *     find all files which don't have a word
 *             */
fileLL notSearch(char* terms){
    fileLL files =NULL;


    printf("Entered notSearch()\n");
    printf("terms received in notSearch(): %s\n", terms);
    return files;

}



void promptUser(){
    int option = 0;
    char terms[BUFFER];

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
                notSearch(terms);
                break;

            /* q:  quit the program*/
            case 4:
                return;
            
            default:
                fprintf(stderr, "ERROR: invalid option\nMust choose options from 1-4\n");

                break;
        
        }

    
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
    if(readIndex(argv[1])==-1) return -1; 

    /*Prompt user on what they would like to do*/
    else promptUser();

    return 0;

}
