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
  l->size = 0;
  l->elementSize = elementSize;

  l->terminator = (ListItem *)malloc(sizeof(ListItem));
  l->terminator->next = l->terminator;
  l->terminator->prev = l->terminator;

  return l;
}

void List_destroy(List *l) {
  ListIterator it = List_begin(l);
  while (!ListIterator_equal(it, List_end(l)))
    it = List_remove(l, it);

  free(l->terminator);
  free(l);
}

size_t List_size(List *l) { return l->size; }

bool List_empty(List *l) { return l->size == 0; }

ListIterator List_insert(List *l, ListIterator it, void *data) {
  ListIterator itPrev = ListIterator_prev(it);

  ListItem *newItem = (ListItem *)malloc(sizeof(ListItem));
  newItem->next = it.item;
  newItem->prev = itPrev.item;

  it.item->prev = newItem;
  itPrev.item->next = newItem;

  newItem->data = malloc(l->elementSize);
  memcpy(newItem->data, data, l->elementSize);

  ++l->size;

  ListIterator result = {.item = newItem};
  return result;
}

ListIterator List_remove(List *l, ListIterator it) {
  if (ListIterator_equal(it, List_end(l)))
    return List_end(l);

  ListIterator itPrev = ListIterator_prev(it), itNext = ListIterator_next(it);
  itPrev.item->next = itNext.item;
  itNext.item->prev = itPrev.item;

  free(it.item->data);
  free(it.item);

  --l->size;
  return itNext;
}

void List_swap(List *l, ListIterator it1, ListIterator it2) {
  void *temp = malloc(l->elementSize);

  memcpy(temp, it1.item->data, l->elementSize); // temp = it1.item.data;
  memcpy(it1.item->data, it2.item->data,
         l->elementSize);                       // it1.item.data = it2.item.data
  memcpy(it2.item->data, temp, l->elementSize); // it2.item.data = temp;
  free(temp);
}

ListIterator List_begin(List *l) {
  ListIterator it = List_end(l);
  return ListIterator_next(it); // После терминатора идет первый элемент
}

ListIterator List_end(List *l) {
  ListIterator it = {.item = l->terminator};
  return it;
}

ListIterator ListIterator_next(ListIterator it) {
  it.item = it.item->next;
  return it;
}

ListIterator ListIterator_prev(ListIterator it) {
  it.item = it.item->prev;
  return it;
}

bool ListIterator_equal(ListIterator lhs, ListIterator rhs) {
  return lhs.item == rhs.item;
}

void *ListIterator_get(ListIterator it) { return it.item->data; }
