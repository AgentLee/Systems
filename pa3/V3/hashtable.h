#ifndef HASHTABLE_H
#define HASHTABLE_H
typedef struct HashTable HashTable;
typedef struct LL LL;
typedef struct Node Node;
typedef struct fileNode fileNode;


LL *CreateLL();
HashTable *CreateTable();
void InsertTable(HashTable *hash, char *word, char *file);
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
struct fileNode{
	int count;
	char* file;
};
struct Node {
	char *value;
	int data;
	fileNode* fNode;
	//int count;
	Node *next;
	char* file;
};
struct fileLL{

};
#endif