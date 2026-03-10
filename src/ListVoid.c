#include "ListVoid.h"

#include <stdlib.h>
#include <string.h>

struct ListItem {
  struct ListItem *next, *prev;
  void *data;
};

struct List {
  ListItem *terminator;
  size_t size;
  size_t elementSize;
};

List *List_create(size_t elementSize) {
  List *l = (List *)malloc(sizeof(List));
  l->elementSize = elementSize;
  l->size = 0;
  l->terminator = (ListItem *)malloc(sizeof(ListItem));
  l->terminator->next = l->terminator;
  l->terminator->prev = l->terminator;

  return l;
}

size_t List_size(List *l) { return l->size; }

bool List_empty(List *l) { return l->size == 0; }

ListIterator List_begin(List *l) {
  ListIterator it = {.item = l->terminator->next};
  return it;
}

ListIterator List_end(List *l) {
  ListIterator it = {.item = l->terminator};
  return it;
}

ListIterator ListIterator_next(ListIterator it) {
  ListIterator newIt = {.item = it.item->next};
  return newIt;
}

ListIterator ListIterator_prev(ListIterator it) {
  ListIterator newIt = {.item = it.item->prev};
  return newIt;
}

bool ListIterator_equal(ListIterator lhs, ListIterator rhs) {
  return lhs.item == rhs.item;
}

void *ListIterator_get(ListIterator it) { return it.item->data; }

ListIterator List_insert(List *l, ListIterator it, void *data) {
  ListItem *newItem = (ListItem *)malloc(sizeof(ListItem));
  newItem->data = malloc(l->elementSize);
  memcpy(newItem->data, data, l->elementSize);

	ListIterator itPrev = ListIterator_prev(it);

	newItem->next = it.item;
	newItem->prev = itPrev.item;

	it.item->prev = newItem;
	itPrev.item->next = newItem;
	
	++l->size;
	ListIterator itNewItem = {.item = newItem};
	return itNewItem;
}

ListIterator List_remove(List *l, ListIterator it) {
	if(ListIterator_equal(it, List_end(l)))
		return List_end(l);

	ListIterator itNext = ListIterator_next(it);
	ListIterator itPrev = ListIterator_prev(it);
	itPrev.item->next = itNext.item;
	itNext.item->prev = itPrev.item;

	free(it.item->data);
	free(it.item);
	--l->size;

	return itNext;
}

void List_delete(List *l) {
	ListIterator it = List_begin(l);
	while(!ListIterator_equal(it, List_end(l))) 
		it = List_remove(l, it);

	free(l->terminator);
	free(l);
}












