#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf, stderr */
#include <string.h>
#include <ctype.h>
#include "hashtable.h"

Node *CreateNode(char *word, char* newFile) {
	Node *new_Node = malloc(sizeof(Node));
	new_Node->value = word;
	new_Node->data = 0;
	new_Node->next = NULL;
    new_Node->fNode = CreateFileNode(newFile);
	return new_Node;
}

Node *CreateFileNode(char* file) {
	Node *new_Node = malloc(sizeof(Node));
	new_Node->next = NULL;
	new_Node->file = file;
    new_Node->count=1;
	return new_Node;
}

LL *CreateLL() {
	LL *new_LL = malloc(sizeof(LL));
	new_LL->size = 0;
	return new_LL;
}

char * changeCase(char * w) {
	char * p = strdup(w);
	char * n = p;
    for (; *p ; ++p)
    	*p = tolower(*p);
    return n;
}


HashTable *CreateTable() {
	HashTable *new_table = malloc(sizeof(HashTable));
	new_table->buckets = (LL**) malloc(sizeof(LL*) * 36);
	new_table->capacity = 36;
	new_table->size = 36;
	return new_table;
}

void InsertTable(HashTable *hash, char *word, char *file) {
    char f[strlen(file)];
    strcpy(f, file);
    printf("FILE:\t%s\n", f);

	if(word==NULL)
        return;
    word = changeCase(word);
	/*printf("inserting word: %s\n", word); */
	int key = GetKey(hash, word);

	Node *newNode;
	Node* curr; 
	Node* prev;
	Node* temp;

    if (hash == NULL) {
        return; /* error in trying to get hash table */
    }

    if(file==NULL){
        return;
    }

    if (key == -1) {
        return; /* error has shown up with the key */
    }
    if (hash->buckets[key] == NULL) { /* we havent init an LL yet */
        hash->buckets[key] = CreateLL();        /*Creates new list at bucket*/
	
        hash->buckets[key]->root = CreateNode(word, file);
        hash->buckets[key]->size++;
   
        return;
    } 

/*
 * overwrote linked list
 */




	/* char *f = (char*)malloc(sizeof(char)*(strlen(file) + 1));
    strcpy(f, file);
    char *w = (char*)malloc(sizeof(char)*(strlen(word) + 1));
    strcpy(w, word);*/

    newNode = CreateNode(word, file);

    curr = hash->buckets[key]->root;
    int comp;

    while(curr != NULL){
        comp = strcmp(newNode->value, curr->value);

        /*Adding to the head*/ 
        if(strcmp(newNode->value, hash->buckets[key]->root->value) < 0){
            newNode->next = hash->buckets[key]->root;
            hash->buckets[key]->root = newNode;
            hash->buckets[key]->size++;

            return;
        }

        if(comp == 0 ){   

            /* In the same file */
            if(strcmp(curr->file, newNode->file)==0){
                curr->count++;
                free(newNode);
                return;
            }
            
            else {
               
                /* NEED TO TEST!!! check if the file has already been added if it is not the first file*/
                while(strcmp(curr->next->value, newNode->value)==0){
                   
                    if(strcmp(curr->file, newNode->file)==0){
                        curr->count++;
                        free(newNode);
                        return;
                    }

                    curr=curr->next;

                 }

                /* NEED TO TEST!!! For words in different files  */
                 
				prev->next = newNode;
				newNode->next = curr;
                hash->buckets[key]->size++;
                return;
            }

        } 

        else {
            prev = curr;
            curr = curr->next;

            if(comp < 0){
               /* printf("******<****** newNode:\t%s\tprev:\t%s\tcurr:\t%s\n", newNode->value, prev->value, curr->value);*/

                prev->next = newNode;
                newNode->next = curr;
                hash->buckets[key]->size++;
                return;

            } else if(comp > 0){
                if(curr == NULL){
                    /*printf("******>****** newNode:\t%s\tprev:\t%s\n", newNode->value, prev->value);*/

                    prev->next = newNode;
                    hash->buckets[key]->size++;

                    return;
                } else if(curr != NULL){
                   /* printf("******!>******newNode:\t%s\tprev:\t%s\tcurr:\t%s\n", newNode->value, prev->value, curr->value);*/

                    comp = strcmp(newNode->value, curr->value);

                    if(comp < 0){
                        prev->next = newNode;
                        newNode->next = curr;
                        hash->buckets[key]->size++;
                        return;
                    }
                }

            }

        }
    }


	/*FOR UNSORTED LINKED LIST*/

	/*newNode->next = hash->buckets[key]->root;
	hash->buckets[key]->root = newNode; */

}

int GetKey(HashTable *hash, char *word) {
	if (word != NULL && word[0] != '\0') {
		if(isdigit(word[0])){
			return (word[0] - 47) + 26;
		}else {
			return (word[0] - 96) % 27;
		}
	} else {
		return -1;
	}
}


void DestroyLLFiles(LL* list){
    if(list==NULL){
        return;
    }
    Node* temp;
    Node* curr = list->root;
    while(curr != NULL){
        temp = curr;
        curr = curr->next;
        /*free(temp->file);*/
        free(temp);
    }
    free(list);
    return;
}

void DestroyLL(LL* list){
    if(list==NULL){
        return;
    }
    Node* temp;
    Node* curr = list->root;
    while(curr != NULL){
        temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(list);
    return;
}

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
     printf("printing hash table\n----------------\n");
     if(hash==NULL) return;
     int k;
     for(k = 0; k <= 36; k++){
        if(hash->buckets[k]!=NULL){
            Node *curr = hash->buckets[k]->root;
            while(curr!=NULL){
                fprintf(fp, "<list> %s\n", curr->value);
                fprintf(fp, "%s %d\n", curr->file, curr->count);
                printf("(%s, %d) in file %s | ", curr->value, curr->count, curr->file);
                fprintf(fp, "</list>\n");
                curr= curr->next;
            }
            printf("\n\n");
        }
    }

    return;
}



/*
int main(int argc, char **argv) {

	HashTable *hash = CreateTable();
	InsertTable(hash, "hello");
	InsertTable(hash, "world");
	return 0;

}
*/
