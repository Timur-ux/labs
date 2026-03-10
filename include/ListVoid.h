#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct ListItem ListItem;
typedef struct List List;

typedef struct {
	ListItem * item;
} ListIterator;

List *List_create(size_t elementSize);
void List_delete(List *l);

size_t List_size(List *l);
bool List_empty(List *l);

ListIterator List_begin(List *l);
ListIterator List_end(List *l);

ListIterator ListIterator_next(ListIterator it);
ListIterator ListIterator_prev(ListIterator it);

bool ListIterator_equal(ListIterator lhs, ListIterator rhs);
void *ListIterator_get(ListIterator it);

ListIterator List_insert(List *l, ListIterator it, void *data);
ListIterator List_remove(List *l, ListIterator it);

#endif // !LIST_H
