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


extern int isFile(char *fileName);
extern void processFile(char *file);





#endif
