#ifndef HASHTABLE_H
#define HASHTABLE_H
typedef struct HashTable HashTable;
typedef struct LL LL;
typedef struct Node Node;

LL *CreateLL();
HashTable *CreateTable();
void InsertTable(HashTable *hash, char *word, char *file);
int GetKey(HashTable *hash, char *word);
char * changeCase(char * w);

struct HashTable {
	LL **buckets;
	int capacity;
	int size;
};
struct LL {
	Node *root;
	int size;
};
struct Node {
	char *value;
	char *fileName;
	int count;
	Node *next;
};
#endif