#ifndef SEARCH_H
#define SEARCH_H
#define BUFFER 2048

/* The following structs are used to store the file into memory */
/* linked lists containing files*/
struct fileNode{
    char* file;
    int wordCount;
    struct fileNode* next;

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

};


struct wordLL{
        struct wordNode* head;
        int size;
};


typedef struct wordNode* wordNode;
typedef struct wordLL * wordLL;

extern int isFile(char *fileName);
extern void processFile(char *file);



#endif
