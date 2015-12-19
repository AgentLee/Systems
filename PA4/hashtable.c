#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf, stderr */
#include <string.h>
#include <ctype.h>
#include "hashtable.h"

Node *CreateNode(char *word, char* newFile, int isHead) {
	Node *new_Node = malloc(sizeof(Node));
    //if(isHead == 1){
	   new_Node->value = (char*)malloc(sizeof(char)*strlen(word));
       new_Node->file = (char*)malloc(sizeof(char)*strlen(newFile));
	//}else if(isHead == 0){
      //  new_Node->value = word;
       // new_Node->file = newFile;

        //printf("%s\t%s\n", new_Node->value, new_Node->file);
    //}
    new_Node->data = 0;
	new_Node->next = NULL;
	new_Node->count=1;
	return new_Node;
}

Node *CreateFileNode(char* file) {
	Node *new_Node = malloc(sizeof(Node));
	new_Node->next = NULL;
	new_Node->file = file;
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

        hash->buckets[key]->root = CreateNode(word, file, 1);
        strcpy(hash->buckets[key]->root->value, word);
        strcpy(hash->buckets[key]->root->file, file);
        //printf("hash\t%s\n", hash->buckets[key]->root->value);
        //printf("hash file\t%s\n", hash->buckets[key]->root->file);
        //printf("********\n");
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

    //printf("hash1%s\t%s\n", hash->buckets[key]->root->value, hash->buckets[key]->root->file);

    newNode = CreateNode(word, file, 0);
    //printf("hash2%s\t%s\n", hash->buckets[key]->root->value, hash->buckets[key]->root->file);

    strcpy(newNode->value, word);
    strcpy(newNode->file, file);

    //printf("-----hash\t%s\n", hash->buckets[key]->root->value);
    //printf("-----hash file\t%s\n", hash->buckets[key]->root->file);
    //printf("********\n");

    //printf("newNode\t%s\n", newNode->value);
    //printf("newNode file\t%s\n", newNode->file);
    //printf("********\n");

    curr = hash->buckets[key]->root;
    int comp;

     while(curr != NULL){
        comp = strcmp(newNode->value, curr->value);

        if(strcmp(newNode->value, hash->buckets[key]->root->value) < 0){
            //add to head
            newNode->next = hash->buckets[key]->root;
            hash->buckets[key]->root = newNode;

            return;
        }

        if(comp == 0){
            printf("newNode->file\t%s\tnewNode->value\t%s\tcurr->file\t%s\tcurr->value\t%s\n", newNode->file, newNode->value, curr->file, curr->value);
            if(strcmp(newNode->file, curr->file) == 0){
                curr->count++;
                free(newNode);
            }else {
                if(strcmp(newNode->file, curr->file) > 0){
                    curr->next = newNode;
                    return;
                }else {

                    while(strcmp(curr->next->value, newNode->value)==0){
                   
                        if(strcmp(curr->file, newNode->file)==0){
                            curr->count++;
                            free(newNode);
                            return;
                        }

                        Node *temp = curr->next;
                        curr->next = newNode;
                        newNode->next = temp;
                        return;
                    }
                }
            }

            return;
        }else {
            prev = curr;
            curr = curr->next;

            if(comp < 0){
                printf("******<****** newNode:\t%s\tprev:\t%s\tcurr:\t%s\n", newNode->value, prev->value, curr->value);

                prev->next = newNode;
                newNode->next = curr;
                return;
            }else if(comp > 0){
                if(curr == NULL){
                    printf("******>****** newNode:\t%s\tprev:\t%s\n", newNode->value, prev->value);

                    prev->next = newNode;

                    return;
                }else if(curr != NULL){
                    printf("******!>******newNode:\t%s\tprev:\t%s\tcurr:\t%s\n", newNode->value, prev->value, curr->value);

                    comp = strcmp(newNode->value, curr->value);

                    if(comp < 0){
                        prev->next = newNode;
                        newNode->next = curr;

                        return;
                    }else if(comp > 0){
                        continue;
                    }else if(comp == 0){
                        //count++
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
