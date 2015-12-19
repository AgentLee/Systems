#ifndef SEARCH_H
#define SEARCH_H
#define BUFFER 2048

/* The following structs are used to store the file into memory */
/* linked lists containing files*/
struct fileNode{
	char* file;
	int wordCount;
	struct fileNode* next;
	char *word;
	struct wordNode *wnode;
	int checked;
};


struct fileLL{
        struct fileNode* head;
        int size;
};

typedef struct fileNode* fileNode;
typedef struct fileLL* fileLL;


/* linked list containing words */
struct wordNode{
    	char* value;
    	struct wordNode* next;
    	fileLL files;
	char *file;
};


struct wordLL{
        struct wordNode* head;
        int size;
};


typedef struct wordNode* wordNode;
typedef struct wordLL * wordLL;


struct list {
	char *file;
	wordLL words;
	struct list *next;
};

struct listLL {
	struct list *head;
};


struct fnode{
	char *file;
	struct wnode *word;;
};

struct wnode {
	char *word;
};


int isFile(char *fileName);
void processFile(char *file);
wordNode createWordNode();
fileNode createFileNode();
char *createSearchTerms(char *terms);
int inFile(char *terms, char *op, wordLL wLL);
int fileExists(char *file, fileLL files);
int goodBadFile(char *terms, fileLL filezilla, char *op);
fileLL processOr(char *terms, wordLL wLL);
void processNotOr(char *terms, wordLL wLL);
fileLL andSearch(char *terms, wordLL wLL);
fileLL orSearch(char* terms, wordLL wLL);
fileLL notOrSearch(char* terms, wordLL wLL);
fileLL notAndSearch(char* terms, wordLL wLL);





#endif
