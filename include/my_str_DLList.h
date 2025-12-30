#ifndef __MY_STR_DLLIST_H__
#define __MY_STR_DLLIST_H__

typedef struct StringDLLNode{
    struct StringDLLNode* prev;
    char* str1;
    char* str2;
    struct StringDLLNode* next;
}StringDLLNode;

typedef struct StringDLList{
    StringDLLNode* sentinel;
    unsigned int currentSize;
}StringDLList;

char* my_strdup(const char* str);
StringDLList* create_StringDLList(void);
void add_head_StrDLLNode(StringDLList* list,const char* str1,const char* str2);
void add_tail_StrDLLNode(StringDLList* list,const char* str1,const char* str2);
void remove_head_StrDLLNode(StringDLList* list);
void remove_tail_StrDLLNode(StringDLList* list);
void remove_StrDLLNode(StringDLList* list,StringDLLNode* node);
void free_StringDLList(StringDLList* list);
void print_StringDLList(StringDLList* list);

#endif
