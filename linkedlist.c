#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static Cell* createCell (LinkedList *list, void *data) {
    Cell *temp = (Cell*) malloc(sizeof (Cell));
    if (!temp) return false;
    temp->data = malloc(list->dataTypeSizeInBytes);
    if (!temp->data) {
        free(temp);
        return temp;
    }
    memcpy(temp->data, data, list->dataTypeSizeInBytes);
    return temp;
}

bool LinkedList_Initt(LinkedList *list, int dataTypeSizeInBytes) {
    if (!list)
        return false;
    list->first = list->last = NULL;
    list->size = 0;
    list->dataTypeSizeInBytes = dataTypeSizeInBytes;
    return true;
}

bool LinkedList_IsEmpty (const LinkedList *list) {
    if (!list)
        return true;
    return (list->size == 0);
}

int LinkedList_GetSize (const LinkedList *list) {
    if (!list)
        return -1;
    return list->size;
}

void* LinkedList_Get (const LinkedList *list, int pos) {
    if (!list || pos < 0 || list->size <= 0 || pos >= list->size)
        return NULL;

    if (pos == 0)
        return list->first->data;

    if (pos == list->size - 1)
        return list->last->data;

    Cell *f = NULL;
    for (register int i = 0; i < pos; i++) {
        if (i == 0)
            f = list->first;
        else
            f = f->next;
    }

    return f->next->data;
}

bool LinkedList_GetCopyOf (const LinkedList *list, int pos, void *backup) {
    if (!list || pos < 0 || list->size <= 0 || pos >= list->size) // Lista vazia ou posição nula
        return false;

    if (pos == 0) {
        memcpy(backup, list->first->data, list->dataTypeSizeInBytes);
        return true;
    }

    if (pos == list->size - 1) {
        memcpy(backup, list->first->data, list->dataTypeSizeInBytes);
        return true;
    }

    Cell *f;
    for (register int i = 0; i < pos; i++) {
        if (i == 0)
            f = list->first;
        else
            f = f->next;
    }

    memcpy(backup, f->next->data, list->dataTypeSizeInBytes);
    return true;
}

bool LinkedList_PushFront (LinkedList *list, void *data) {
    Cell *temp;
    if (!(temp = createCell(list, data)))
        return false;

    if (list->size == 0) {
        list->first = list->last = temp;
        temp->next = NULL;
        list->size++;
        return true;
    }

    temp->next = list->first;
    list->first = temp;
    list->size++;
    return true;
}

bool LinkedList_PushBack (LinkedList *list, void *data) {
    Cell *temp;
    if (!(temp = createCell(list, data)))
        return false;

    if (list->size == 0) {
        list->first = list->last = temp;
        temp->next = NULL;
        list->size++;
        return true;
    } else {
        list->last->next = temp;
        list->last = temp;
        list->last->next = NULL;
    }

    list->size++;
    return true;
}

bool LinkedList_PopFront (LinkedList *list, void *backup) {
	if (list->size == 0)
		return false;
    else if (list->size == 1) {
        memcpy(backup, list->first->data, list->dataTypeSizeInBytes);
        LinkedList_Clear(list); // Portanto, apenas limpe a lista!
    } else {
        Cell *nextCpy = list->first->next;
        memcpy(backup, list->first->data, list->dataTypeSizeInBytes);

        free(list->first->data);
        free(list->first);
        list->first = nextCpy;
        list->size--;
    }
    return true;
}

bool LinkedList_PopBack (LinkedList *list, void *backup) {
    if (!list)
        return false;

    // Se a lista tiver apenas 1 elemento
    if (list->size == 1) {
        memcpy(backup, list->first->data, list->dataTypeSizeInBytes);
        LinkedList_Clear(list);
    }
    // A lista possui mais de um elemento
    else {
        Cell *f = NULL, *last = NULL;
        memcpy(backup, list->last->data, list->dataTypeSizeInBytes);

        // Percorre a lista até encontrar o penúltimo elemento da lista
        for (f = list->first; f != list->last; f = f->next)
            last = f;

        // Nesse momento 'last' aponta para o penúltimo elemeto da lista

        free(list->last->data); // Remove o dado do último elemento
        free(list->last); // Remove o último elemento
        list->last = last; // Agora o último elemento se torna 'last'
        list->last->next = NULL; // Portanto last->next deve ser NULL
        list->size--;
    }
    return true;
}

bool LinkedList_Insert (LinkedList *list, void *data, int pos) {

    if (!list || pos < 0 || pos > list->size) // Lista nula ou posição inválida
        return false;

    if (pos == 0) // Insere na primeira posição
        return LinkedList_PushFront(list, data);

    if (pos == list->size) // Insere na última posição
        return LinkedList_PushBack(list, data);

    // Insere no meio
    Cell *f = NULL;
    for (register int i = 0; i < pos; i++) {
        if (i == 0)
            f = list->first;
        else
            f = f->next;
    }
    // Nesse momento 'f' aponta para a posição anterior à que será inserido o elemento

    Cell *temp;
    if (!(temp = createCell(list, data)))
        return false;

    temp->next = f->next;
    f->next = temp;
    list->size++;
    return true;
}

bool LinkedList_Remove (LinkedList *list, int pos, void *backup) {

    if (!list || list->size <= 0 || pos < 0 || pos >= list->size) // Lista vazia ou nula ou posição inválida
        return false;

    // Remove da última posição
    if (pos == list->size - 1)
        return LinkedList_PopBack(list, backup);

    // O usiário deseja remover um elemento do início
    if (pos == 0)
        return LinkedList_PopFront(list, backup);

    // Remover algum elemento no MEIO da lista
    Cell *f = NULL, *nextCpy;
    for (register int i = 0; i < pos; i++) {
        if (i == 0)
            f = list->first;
        else
            f = f->next;
    }
    // Nesse momento 'f' aponta para o elemento anterior ao que será removido

    nextCpy = f->next->next; // 'nextCpy' aponta para o elemento posterior ao que será removido
    memcpy(backup, f->next->data, list->dataTypeSizeInBytes);

    free(f->next->data); // Remove o VALOR do elemento (Célula) entre 'f' e 'nextCpy', que pertence justamente o elemento que o usuário deseja remover
    free(f->next); // Remove o ELEMENTO entre 'f' e 'nextCpy', que é justamente o elemento que o usuário deseja remover
    f->next = nextCpy; // Portanto lique 'f' em 'nextCpy'
    list->size--;
    return true;
}

void LinkedList_Clear (LinkedList *list) {
    if (!list || list->size <= 0 || list->first == NULL)
        return;

    // 'curr' iniciará apontando para o primeiro elemento da lista e next para o segundo
    // (que pode ser nulo caso a lista possua apenas um elemento)

    register Cell *next;
    for (register Cell *p = list->first; p != NULL;) {
        next = p->next;
        free(p->data);
        free(p);
        p = next;
    }

    list->first = NULL;
    list->last = NULL;
    list->size = 0;
}

LinkedList* LinkedList_Duplicate (const LinkedList *list) {
    if (!list)
        return NULL;
    LinkedList *temp = (LinkedList*) malloc(list->dataTypeSizeInBytes);
    LinkedList_Init(temp, list->dataTypeSizeInBytes);
    for (Cell *p = list->first; p != NULL; p = p->next)
        LinkedList_PushBack(temp, p->data);
    return temp;
}

inline void *LinkedList_GetFirst(const LinkedList *list) {
    if (!list)
        return NULL;
    return list->first->data;
}

inline void *LinkedList_GetLast(const LinkedList *list) {
    if (!list)
        return NULL;
    return list->last->data;
}

void LinkedList_Copy(LinkedList *dest, LinkedList *orig) {
    if (!dest || !orig || dest->dataTypeSizeInBytes != orig->dataTypeSizeInBytes)
        return;
    LinkedList_Clear(dest);
    dest->dataTypeSizeInBytes = orig->dataTypeSizeInBytes;
    for (int i = 0; i < orig->size; ++i) {
        LinkedList_PushBack(dest, LinkedList_Get(orig, i));
    }
}

bool LinkedList_Swap (LinkedList *list, int pos1, int pos2) {
    if (!list || pos1 < 0 || pos2 < 0 || pos1 >= list->size || pos2 >= list->size || pos1 == pos2)
        return false;

    void *temp = malloc(list->dataTypeSizeInBytes);
    if (!temp)
        return false;

    memcpy(temp, LinkedList_Get(list, pos1), list->dataTypeSizeInBytes);

    memcpy(LinkedList_Get(list, pos1), LinkedList_Get(list, pos2), list->dataTypeSizeInBytes);
    memcpy(LinkedList_Get(list, pos2), temp, list->dataTypeSizeInBytes);

    free(temp);
    return true;
}

LinkedList* LinkedList_BubbleSort(LinkedList *list, int (*canSwap)(void *data1, void *data2)) {
    if (!list)
        return NULL;
    for (int i = 0; i < list->size - 1; ++i) {
        bool quit = true;
        for (int j = 0; j < list->size - 1; ++j) {
            if (canSwap(LinkedList_Get(list, j), LinkedList_Get(list, j+1)) > 0) {
                LinkedList_Swap(list, j, j+1);
                quit = false;
            }
        }
        if (quit)
            break;
    }
	return list;
}

LinkedList* LinkedList_QuickSort(LinkedList *list, int beg, int end, int (*compare)(void *data1, void *data2)) {
	void* pivot = LinkedList_Get(list, (beg + end) / 2);
	int i = beg;
	int j = end;
	
	while (i < j) {
        while (compare(LinkedList_Get(list, i), pivot) < 0 && i < end)
            i++;

        while (compare(LinkedList_Get(list, j), pivot) > 0 && i < end)
            j--;

        if (i <= j) {
			LinkedList_Swap(list, i, j);
            i++;
            j--;
        }
    }

    if (i < end)
        LinkedList_QuickSort(list, i, end, compare);
    if (j > beg)
        LinkedList_QuickSort(list, beg, j, compare);

    return list;	
}



