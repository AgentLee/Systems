#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include "tokenizer.c"

void extractWords(char *line, char *file){
	TokenizerT *tok = TKCreate(" ", line);
	
	char *token = NULL;
	
	while((token = TKGetNextToken(tok)) != NULL){
		printf("%s\n", token);
	}
}

void processFile(char *file){
	FILE *fp = fopen(file, "r");
	
	char *l = (char*)malloc(BUFSIZ);
	char *line = fgets(l, BUFSIZ, fp);

	while(line != NULL){
		extractWords(line, file);
		line = fgets(l, BUFSIZ, fp);
	}		
}

void isFile(char *fileName){
	struct stat s;
	if(stat(fileName, &s) == 0){
		if(s.st_mode &S_IFREG){
			processFile(fileName);
		}
	}
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

void processDir(char *dirName){
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
		else
			isFile(directory);

		if(dir->d_type & DT_DIR){
			if(strcmp(dname, ".") != 0 && strcmp(dname, "..") != 0){
				char path[BUFSIZ];
				strcpy(path, dirName);
				strcat(path, "/");
				strcat(path, dname);
				processDir(path);
			}
		}
	}
}
/*
void processDir(char *dirName){
	//pass argv[1] as dirName
	DIR *d;
	struct dirent *dir;
	struct stat s;

	FILE *p;
	size_t nread;
	char word[BUFSIZ];

	char directory[sizeof(dirName) + 1];
	strcpy(directory, dirName);
	strcat(directory, "/");

	printf("%s\n", directory);
	d = opendir(directory);
	
	char *file;

	if(d){
		while((dir = readdir(d)) != NULL){
			if(strcmp(dir->d_name, ".") == 0)
				continue;
			if(strcmp(dir->d_name, "..") == 0)
				continue;
			else {
				//printf("dir->d_name:\t%s\n", dir->d_name);
										


	

				//if(stat(dir->d_name, &s) == 0){
					//if(isDir("directory1/folder") == 1){
						//printf("%s:\t\nis a directory\n", dir->d_name);
						/*
 						 * Maybe make a linked list that has all the subfolders in
 						 */
						//char directory2[sizeof(directory) + 1 + sizeof(dir->d_name)];
						//char directory2[100];
						//strcat(directory2,"directory1/folder");
						//strcat(directory2, "/");
						//printf("%s\n****", directory2); 
						//processDir(directory2);
					//}
					
				//}

				/*
 				 *
				 * Path name needs to have directory1/folder
 				 * SegFault because passing in file directory1
 				 * Need check for file
 				 *
 				 */

				//if(stat(dir->d_name, &s) == 0){
				//	if(s.st_mode & S_IFDIR){
				//		processDir(dir->d_name);
				//	}
				//}

/*					strcpy(directory, dirName);
					strcat(directory, "/");
					file = directory;
					strcat(file, dir->d_name);	
					processFile(file);
			}
		}
	}		
}
*/

int main(int argc, char **argv){
	struct stat s;
	
	if(stat(argv[1], &s) == 0){
	    	if(isDir(argv[1]) == 1)
			processDir(argv[1]);
	}

	return 0;	
/*
 * 	DIR *FD;
	struct dirent* in_file;
	FILE *common_file;
	FILE *entry_file;
	char buffer[BUFSIZ];

	common_file = fopen("/directory1/test.txt", "w");
	if(common_file == NULL){
		printf("file not found\n");
	}

	while((in_file = readdir(FD))){
		if(!strcmp(in_file->d_name, ".") || !strcmp(in_file->d_name, "..")){
			continue;
		}

		//entry_file = fopen(in_file->d_name, "rw");

		processFile(in_file->d_name);
	}
*/
}

