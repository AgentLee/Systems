#ifndef INDEXER_H
#define INDEXER_H
#define BUFFER 4096
/* Takes out words from a line in a file and stores it into the hashtable */
void extractWords(char* line, char* file, HashTable *table);


  /* Process the files and directories differently 
   * Note: should build path (unless you can think of an alternate way) */
void processFile(char* file, HashTable *table);
void processDir(char* dir, HashTable* table);

int isFile(char *fileName);

int isDir(char *dirName);





#endif