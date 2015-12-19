#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf, stderr */
#include <string.h>
#include <ctype.h>
#include "hashtable.h"

/* Nodes for words*/
Node *CreateNode(char *word) {
    if (word == NULL) {
        fprintf(stderr, "CreateNode(): NULL word was received\n");
        exit (10);
    }
	Node *new_Node = malloc(sizeof(Node));
    new_Node->value = (char*)malloc(sizeof(char)*(strlen(word)+1));
    strcpy(new_Node->value, word);
    new_Node->files = NULL;
	new_Node->next = NULL;
	return new_Node;
}

/* Nodes for files */
fileNode *CreateFileNode(char* file) {
    if (file == NULL) {
        fprintf(stderr, "CreateFileNode(): NULL file was received\n");
        exit (10);
    }
	fileNode *new_Node = malloc(sizeof(fileNode));
    new_Node->file = (char*)malloc(sizeof(char)*(strlen(file)+1));
    strcpy (new_Node->file, file);
    new_Node->count = 1;
	new_Node->next = NULL;
	return new_Node;
}

 
/* File linked list to store fileNodes */
fileLL *CreateFileLL() {
    fileLL *new_LL = (fileLL*)malloc(sizeof(fileLL));
    new_LL->size = 0;
    return new_LL;
}


/* Linked list to store Nodes */
LL *CreateLL() {
	LL *new_LL = malloc(sizeof(LL));
	new_LL->size = 0;
	return new_LL;
}

void swap(void* a, void * b) {
 //   printf("swap(): %x, %x\n", a, b);

    void* temp;
    temp  = a;
    a = b;
    b = temp;
   // printf("swap():%x, %x\n", a, b);
}

/* Converts words from uppercase to lowercase */
char * changeCase(char * w) {
	char * p = strdup(w);
	char * n = p;
    for (; *p ; ++p)
    	*p = tolower(*p);
    return n;
}

/* Initializes hash table */
HashTable *CreateTable() {
	HashTable *new_table = malloc(sizeof(HashTable));
	new_table->buckets = (LL**) malloc(sizeof(LL*) * 36);
	new_table->capacity = 36;
	new_table->size = 36;

    int i; /* initializing all buckets in table */
    for(i=0; i<new_table->capacity; i++){
        new_table->buckets[i] = NULL;
    }

	return new_table;

}


/* Inserts Word into Hash Table 
    returns 1 upon success, returns 0 upon failure */
int InsertWord(HashTable *hash, char* word, char* file){

    /*error checking*/
    if(word==NULL){
        fprintf(stderr, "ERROR: no word given\n");
        return 0; /* error no word */
    }
    if (hash == NULL) {
        fprintf(stderr, "ERROR: could not access hash table\n" );
        return 0; /* error in trying to get hash table */
    }
    if(file==NULL){
        fprintf(stderr, "ERROR: no file given\n");
        return 0; /* error improper file*/
    }
    word = changeCase(word);
    /*printf("inserting word: %s\n", word); */
    int key = GetKey(hash, word);

    /* debugging 
    printf("word %s has key %d\n", word, key);
    */

    if (key == -1) {
        fprintf(stderr, "ERROR: key not found\n");
        return 0; /* error has shown up with the key */
    } /* end initial error checking*/

    Node *newWord=NULL;
    Node* curr = NULL; 
    Node* prev = NULL;

    /* The hash table bucket is initially empty - no head*/
    if (hash->buckets[key] == NULL) { 
        

        hash->buckets[key] = CreateLL();    /*Creates new list at bucket*/
        hash->buckets[key]->root = CreateNode(word);    /* Creates the word node */

        /* debugging 
        printf("root word in bucket %d is %s\n", key, hash->buckets[key]->root->value);
        */

        hash->buckets[key]->size++; 
        
        InsertFile(hash->buckets[key]->root, file);  /* Insert initial file for word */

        return 1;
    } 
    /* If hash table created already - has head */
    else{

        newWord = CreateNode(word); /* Creating word node to add into linked list*/
        curr = hash->buckets[key]->root; /* set init curr value to head of linked list */

        int comp; /* comparing words */

        while(curr){
            comp = strcmp(curr->value, newWord->value);

            /* word already exists, add file */
            if(comp == 0){
                InsertFile(curr, file);
                return 1; /* insert successful - duplicate word */
            }
            /* need to insert before current */
            else if(comp > 0){
                /* this is the head */
                if(prev == NULL){
                    newWord->next = hash->buckets[key]->root;
                    hash->buckets[key]->root = newWord;
                }
                else{
                    newWord->next = curr;
                    prev->next = newWord;
                }

                hash->buckets[key]->size++; 
                InsertFile(newWord, file);
                return 1; /* insert successful */

            }
            /* go to next item */
            prev = curr;
            curr = curr->next;

        }

    }
        /* Add word to the end of the list */
        prev->next = newWord;
        InsertFile(newWord, file);
        hash->buckets[key]->size++; 

        return 1;
}

/* Inserts File into File Linked List in a Word Node
    returns 1 upon success, returns 0 upon failure 
        NOTE: does not store more than 5 in the list*/
int InsertFile(Node* word, char* file){
    fileNode *newFile = NULL;
    fileNode* curr = NULL; 
    fileNode* prev = NULL;

    if(word == NULL){
        fprintf(stderr, "ERROR: no word given\n");
        return 0;
    }

    if(file == NULL){
        fprintf(stderr, "ERROR: no file name given\n");
        return 0;
    }

   
    /* file linked list is empty */
    if(word->files == NULL){
        word->files = CreateFileLL();
        word->files->root = CreateFileNode(file);
 
          /* debugging 
        printf("root file in linked list for word %s is %s\n", word->value, word->files->root->file);
            */
        word->files->size++;
        return 1;
    }



    else{ 
        newFile = CreateFileNode(file); /* Creating file node to add into linked list*/

        prev = curr = word->files->root; /* set init curr value to head of linked list */

        while(curr){
  
            /* file already exists, count++, re-sort */
            if(strcmp(curr->file, file) == 0){
                curr->count++;  


                /* HAVE TO SORT FILES HERE IDK HOW TO DO THISS!!!!!! :( */


                return 1; 
            }
            /* need to insert before current */
            if(curr->count < newFile->count){
                /* this is the head */
                if(prev == NULL){
                    prev = newFile;
                    newFile->next = word->files->root;
                    word->files->root = prev;
                }
                else{
                    newFile->next = curr;
                    prev->next = newFile;
                }

                word->files->size++; 
                return 1; /* insert successful */

            }
            /* go to next item */
            prev = curr;
            curr = curr->next;

        }

    }
        /* Add word to the end of the list */
        prev->next = newFile;
        word->files->size++; 

        return 1;

}

/* Generates the hashing key: 
    if the first character is a digit then the word is stored in buckets 0 - 9 
    if it is a letter then it is stored in buckets 10 - 35 */
int GetKey(HashTable *hash, char *word) {
	if (word != NULL && word[0] != '\0') {
		if(isdigit(word[0])){
			return (word[0] - 48);
		}else {
			return ((word[0] - 97) % 27) + 10;
		}
	} else {
		return -1;
	}
}

/* destroys fileLL */
void DestroyFiles(fileLL* list){
    if(list==NULL){
        return;
    }
    fileNode* temp = NULL;
    fileNode* curr = list->root;
    while(curr != NULL){
        temp = curr;
        curr = curr->next;
        free(temp->file);
        free(temp);
    }
    free(list);
    return;
}

/* Destroys word linked list */
void DestroyLL(LL* list){
    if(list==NULL){
        return;
    }
    Node* temp;
    Node* curr = list->root;
    while(curr != NULL){
        if(curr->files!=NULL){
            DestroyFiles(curr->files);
        }
        temp = curr;
        curr = curr->next;
        free(temp->value);
        free(temp);
    }
    free(list);
    return;
}

/* Destory hash tables */
void DestroyTable(HashTable* hash){
    if(hash == NULL)
        return;

    int key;
    for(key = 0; key <= 36; key++){

        if(hash->buckets[key] != NULL)
            DestroyLL(hash->buckets[key]);
        
    }

    free(hash->buckets);
    free(hash);
    return;
}


void printHash(HashTable *hash, FILE *fp){
    if(hash==NULL){
        fprintf(stderr, "ERROR: Nothing stored in hash table\n");
        return;
    }

    if(fp == NULL){
        fprintf(stderr, "ERROR: Cannot write to file\n");
    }



    printf("printing hash table\n--------------------\n");

    int k;
    for(k=0; k<36; k++){
        if(hash->buckets[k]!=NULL){
            Node* currWord = hash->buckets[k]->root;
            fprintf(fp, "<list> %s\n", currWord->value);
            char* prevWord = currWord->value;
            while(currWord){
                 fileNode* currFile  = NULL;
                if(strcmp(prevWord, currWord->value) != 0){
                    fprintf(fp, "</list>\n");
                    fprintf(fp, "<list> %s\n", currWord->value);
                }
                if (currWord != NULL) {
                    if (currWord->files != NULL ){
                        if (currWord->files->root != NULL)
                           currFile = currWord->files->root;
                    
                        else {
                            fprintf(stderr, "currWord->files->root is NULL\n");
                            exit(20);
                        }
                    }                   
                    else {
                        fprintf(stderr, "currWord->files == NULL\n");
                        exit(21);
                        }
                } else {
                    fprintf(stderr, "currWord->files == NULL\n");
                    exit(22);
                }

                int lineCount; /* tracks the amount of words in the line (only 5 per line)*/
                for(lineCount=0; currFile; currFile = currFile->next){
                    if(lineCount == 5){
                        fprintf(fp, "\n");
                        lineCount = 0;

                    }

                    fprintf(fp, "%s %d ", currFile->file, currFile->count);
                    lineCount++;
                   
                }

                printf("(%s)\n ", currWord->value );
                for(lineCount=0, currFile=currWord->files->root; currFile; currFile = currFile->next){
                    if(lineCount == 5){
                        printf("\n");
                        lineCount = 0;

                    }

                    printf("%s %d ", currFile->file, currFile->count);
                    lineCount++;
                   
                }

                prevWord=currWord->value;
                currWord=currWord->next;
            }
            fprintf(fp, "\n");
            fprintf(fp, "</list>\n");
            printf("\n");

        }
    }

}


/*
int main(int argc, char **argv) {

	HashTable *hash = CreateTable();
	InsertTable(hash, "hello");
	InsertTable(hash, "world");
	return 0;

}
*/
