#ifndef HASHTABLE_H
#define HASHTABLE_H
typedef struct HashTable HashTable;
typedef struct LL LL;
typedef struct fileLL fileLL;
typedef struct Node Node;
typedef struct fileNode fileNode;


LL *CreateLL();
HashTable *CreateTable();
int InsertWord(HashTable *hash, char *word, char *file);
int InsertFile( Node* word, char* file);
int GetKey(HashTable *hash, char *word);
char * changeCase(char * w);
void DestroyTable(HashTable* hash);

void printHash(HashTable *hash, FILE *fp);

struct HashTable {
	LL **buckets;
	int capacity;
	int size;
};
struct LL {
	Node *root;
	int size;
};

/* Nodes for words*/
struct Node {
	char *value;
	Node *next;
	fileLL* files;

};

struct fileLL {
	fileNode* root; 
	int size;
};

struct fileNode {
	char* file;
	fileNode* next;
	int count;
};





#endif