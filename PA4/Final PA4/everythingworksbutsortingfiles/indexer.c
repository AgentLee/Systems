/*
 *	CS214 - Assignment Three - Indexer
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include "hashtable.h"
#include "indexer.h"
#include "tokenizer.h"



void extractWords(char *line, char *file, HashTable *table){
 	char *delims = " ,./$!@^&*():;_`-?<>-\n\r\\#|[]{}~+=%%\"\'";
	TokenizerT *tok = TKCreate(delims, line);

	char *token = NULL;

	int key = 0;

	while((token = TKGetNextToken(tok)) != NULL){
		
        /*Print before storing and before changing case*/
		/*printf("%s\n", token);*/

		InsertWord(table, token, file);
		key = GetKey(table, token);

		/*Print after storing in hash table and after changing case*/
		/*printf("%s\n", table->buckets[key]->root->value);*/
	}
	TKDestroy(tok);
	return;
}


void processFile(char *file, HashTable *table){
 	FILE *fp = fopen(file, "r");

    if(!fp){
        fprintf(stderr, "ERROR: file does not exist\n");
    }

  	/*Figure out how to get bigger buffer size*/
 	char *l = (char*)malloc(BUFFER);
 	char *line = fgets(l, BUFFER, fp);

    if(line==NULL){
        fprintf(stderr, "ERROR: file is empty\n");
    }

 	while(line != NULL){
 		extractWords(line, file, table);
 		line = fgets(l, BUFFER, fp);
 	}
 	free(l);
 	return;
 }


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

int isDir(char *dirName){
        struct stat s;
        if(stat(dirName, &s) == 0){
                if(s.st_mode &S_IFDIR){
                        return 1;
                }else {
                        return 0;
                }
        }

        return 0;
}


void processDir(char *dirName, HashTable *table){
    DIR *d;
    d = opendir(dirName);

    if(!d){
        printf("Error: not a directory\n");
    }

    while(1){
        struct dirent *dir;
        char *dname;
        char directory[BUFSIZ];

        dir = readdir(d);
        
        if(!dir){
            break;
        }

        dname = dir->d_name;
        
        strcpy(directory, dirName);
        strcat(directory, "/"); 
        strcat(directory, dname);

        if(strcmp(directory, ".") == 0)
            continue;
        if(strcmp(directory, "..") == 0)
            continue;
        else if(isFile(directory) == 1)
            processFile(directory, table);

        if(dir->d_type & DT_DIR){
            if(strcmp(dname, ".") != 0 && strcmp(dname, "..") != 0){
                char path[BUFSIZ];
                strcpy(path, dirName);
                strcat(path, "/");
                strcat(path, dname);
                processDir(path, table);
            }
        }
    }
}
 
 
 int main(int argc, char **argv){
        if(argc != 3){
            printf("Invalid number of arguments.\nShould be of form:\n\t\"%s <inverted-index file name> <directory or file name>\"\n", argv[0]);
            return -1;
        }

 		struct stat s;

 		HashTable *table = CreateTable();

        char* overwrite = (char*) malloc(BUFFER);

        if(isFile(argv[1])==1){
            printf("Inverted-index already exists. Would you like to overwrite it? (y/n)\n");
            fflush(stdin);
            scanf(" %[^\n]s", overwrite);
            if(strcmp(overwrite, "y")==0){
                printf("Overwriting file\n");
                free(overwrite);
            }
            else{
                if(strcmp(overwrite, "n")!=0){
                    printf("ERROR: incorrect input\nEnter 'y' or 'n'\n");
                }
                printf("not overwriting file\n");
                free(overwrite);
                return 0;
            }
        }

        FILE *fp;
        fp = fopen(argv[1], "rb+");
        
        if(fp == NULL){
            fp = fopen(argv[1], "w");
        }

        if(fp == NULL){
            printf("Error writing to file\n");
            return 0;
        }

		if(stat(argv[2], &s) == 0){
			if(isDir(argv[2]) == 1)
		      processDir(argv[2], table);
            else if(isFile(argv[2]) == 1)
              processFile(argv[2], table);
            else{
                printf("Error: not a file or directory\n");
                return -1;
            }

		} else{
            printf("Error: file/directory not found\n");
            return -1;
        }

        printHash(table, fp);
        fclose(fp);

		DestroyTable(table);
		return 0;
}
