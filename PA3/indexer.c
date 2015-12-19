/*
 *	CS214 - Assignment Three - Indexer
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include "hashtable.h"
#include "hashtable.c"
#include "indexer.h"
#include "tokenizer.c"

void extractWords(char *line, char *file, HashTable *table){
 	char *delims = " ,./$!@^&*():;_`-?<>-\n\r\\#|";
	TokenizerT *tok = TKCreate(delims, line);

	char *token = NULL;

	int key = 0;

	while((token = TKGetNextToken(tok)) != NULL){
		/*Print before storing and before changing case*/
		//printf("token:\t%s\n", token);
		//printf("file:\t%s\n\n", file);
		InsertTable(table, token, file);
		key = GetKey(table, token);

		/*Print after storing in hash table and after changing case
		printf("%s\n", table->buckets[key]->root->value);
		*/
	}

}

void processFile(char *file, HashTable *table){
 	FILE *fp = fopen(file, "r");

  	/*Figure out how to get bigger buffer size*/
 	char *l = (char*)malloc(BUFFER);
 	char *line = fgets(l, BUFFER, fp);

 	while(line != NULL){
 		extractWords(line, file, table);
 		line = fgets(l, BUFFER, fp);
 	}
 }

int isFile(char *fileName){
	struct stat s;
	if(stat(fileName, &s) == 0){
		if(s.st_mode &S_IFREG){
			return 1;
		}else
			return 0;
	}

	return 0;
}

int isDir(char *dirName){
	struct stat s;
	if(stat(dirName, &s) == 0){
		if(s.st_mode &S_IFDIR)
			return 1;
		else		
			return 0;
	}	

	return 0;
}

void processDir(char *dirName, HashTable *table){
	DIR *d;
	d = opendir(dirName);

	if(!d){
		printf("Error\n");
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

/*
void printHash(HashTable *hash){
    printf("printing hash table\n----------------\n");
    if(hash==NULL) return;
    int k;
    for(k = 0; k <= 36; k++){
        if(hash->buckets[k]!=NULL){
            Node *curr = hash->buckets[k]->root;
            while(curr!=NULL){
                printf("%s | %s | %d\t", curr->value, curr->fileName, curr->count);
                curr= curr->next;
            }
            printf("\n");
        }
    }
}
*/

int main(int argc, char **argv){
 	if(argc != 2){
 		printf("Invalid Number of arguments\n");
 		return -1;
 	}

 	HashTable *table = CreateTable();

	struct stat s;

	if(stat(argv[1], &s) == 0){
	    	if(isDir(argv[1]) == 1)
				processDir(argv[1], table);
			else if(isFile(argv[1]) == 1)
				processFile(argv[1], table);
	}

	printHash(table);

	/*
	if( stat(argv[1],&s) == 0 ){
	    if(isDir(argv[1]) == 1){
	    	processDir(argv[1], table);
		//processDir(argv[1]);
		}else if( s.st_mode & S_IFREG ){
	 			processFile(argv[1], table);
	 			printf("\n\n\n");
	 			printHash(table);
		        //it's a file
		}else{
		        //something else
		    	printf("error: not a file or directory\n");	
	    }
	}else{
	    //error
	    printf("error: file/directory not found\n");
	}
	*/
	DestroyTable(table);
 	return 0;
 }
