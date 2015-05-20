/*
*	Implementação de uma lista encadeada
*
*	Autor: Michael Douglas Pacheco
*	Data: 19/05/2015
*	Hora: 21:04
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <limits.h>
#include <stdbool.h>

#define LAST_POS INT_MAX - 1

typedef struct Cell {
    void *data;
    struct Cell *next;
} Cell;

typedef struct {
    Cell *first;
    Cell *last;
    int size;
    int dataTypeSizeInBytes;
} LinkedList;

bool            LinkedList_Init         (LinkedList *list, int dataTypeSizeInBytes);		
bool            LinkedList_IsEmpty      (const LinkedList *list);                           
int             LinkedList_GetSize      (const LinkedList *list);                           
void*           LinkedList_Get          (const LinkedList *list, int pos);                  
bool            LinkedList_GetCopyOf    (const LinkedList *list, int pos, void *backup);    
bool            LinkedList_Insert       (LinkedList *list, void *data, int pos);            
bool            LinkedList_Remove       (LinkedList *list, int pos, void *backup);          
void            LinkedList_Clear        (LinkedList *list);                                 
LinkedList*     LinkedList_Duplicate    (const LinkedList *list);                           

#endif // LINKEDLIST_H