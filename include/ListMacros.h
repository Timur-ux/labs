#ifndef LIST_MACROS_H
#define LIST_MACROS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define LIST(alias, T)                                                         \
  typedef struct alias##Item alias##Item;                                      \
  typedef struct alias alias;                                                  \
                                                                               \
  typedef struct {                                                             \
    alias##Item *item;                                                         \
  } alias##Iterator;                                                           \
                                                                               \
  struct alias##Item {                                                         \
    struct alias##Item *next, *prev;                                           \
    T data;                                                                    \
  };                                                                           \
                                                                               \
  struct alias {                                                               \
    alias##Item *terminator;                                                   \
    size_t size;                                                               \
  };                                                                           \
                                                                               \
  alias *alias##_create();                                                     \
  void alias##_delete(alias *l);                                               \
                                                                               \
  size_t alias##_size(alias *l);                                               \
  bool alias##_empty(alias *l);                                                \
                                                                               \
  alias##Iterator alias##_begin(alias *l);                                     \
  alias##Iterator alias##_end(alias *l);                                       \
                                                                               \
  alias##Iterator alias##Iterator_next(alias##Iterator it);                    \
  alias##Iterator alias##Iterator_prev(alias##Iterator it);                    \
                                                                               \
  bool alias##Iterator_equal(alias##Iterator lhs, alias##Iterator rhs);        \
  T *alias##Iterator_get(alias##Iterator it);                                  \
                                                                               \
  alias##Iterator alias##_insert(alias *l, alias##Iterator it, T data);        \
  alias##Iterator alias##_remove(alias *l, alias##Iterator it);                \
                                                                               \
  alias *alias##_create() {                                                    \
    alias *l = (alias *)malloc(sizeof(alias));                               \
    l->size = 0;                                                               \
    l->terminator = (alias##Item *)malloc(sizeof(alias##Item));                \
    l->terminator->next = l->terminator;                                       \
    l->terminator->prev = l->terminator;                                       \
                                                                               \
    return l;                                                                  \
  }                                                                            \
                                                                               \
  void alias##_delete(alias *l) {                                              \
    alias##Iterator it = alias##_begin(l);                                     \
    while (!alias##Iterator_equal(it, alias##_end(l)))                         \
      it = alias##_remove(l, it);                                              \
                                                                               \
    free(l->terminator);                                                       \
    free(l);                                                                   \
  }                                                                            \
                                                                               \
  size_t alias##_size(alias *l) { return l->size; }                            \
                                                                               \
  bool alias##_empty(alias *l) { return l->size == 0; }                        \
                                                                               \
  alias##Iterator alias##_begin(alias *l) {                                    \
    alias##Iterator it = {.item = l->terminator->next};                        \
    return it;                                                                 \
  }                                                                            \
                                                                               \
  alias##Iterator alias##_end(alias *l) {                                      \
    alias##Iterator it = {.item = l->terminator};                              \
    return it;                                                                 \
  }                                                                            \
                                                                               \
  alias##Iterator alias##Iterator_next(alias##Iterator it) {                   \
    alias##Iterator newIt = {.item = it.item->next};                           \
    return newIt;                                                              \
  }                                                                            \
                                                                               \
  alias##Iterator alias##Iterator_prev(alias##Iterator it) {                   \
    alias##Iterator newIt = {.item = it.item->prev};                           \
    return newIt;                                                              \
  }                                                                            \
                                                                               \
  bool alias##Iterator_equal(alias##Iterator lhs, alias##Iterator rhs) {       \
    return lhs.item == rhs.item;                                               \
  }                                                                            \
                                                                               \
  T *alias##Iterator_get(alias##Iterator it) { return &it.item->data; }        \
                                                                               \
  alias##Iterator alias##_insert(alias *l, alias##Iterator it, T data) {       \
    alias##Item *newItem = (alias##Item *)malloc(sizeof(alias##Item));         \
    newItem->data = data;                                                      \
                                                                               \
    alias##Iterator itPrev = alias##Iterator_prev(it);                         \
                                                                               \
    newItem->next = it.item;                                                   \
    newItem->prev = itPrev.item;                                               \
                                                                               \
    it.item->prev = newItem;                                                   \
    itPrev.item->next = newItem;                                               \
                                                                               \
    ++l->size;                                                                 \
    alias##Iterator itNewItem = {.item = newItem};                             \
    return itNewItem;                                                          \
  }                                                                            \
                                                                               \
  alias##Iterator alias##_remove(alias *l, alias##Iterator it) {               \
    if (alias##Iterator_equal(it, alias##_end(l)))                             \
      return alias##_end(l);                                                   \
                                                                               \
    alias##Iterator itNext = alias##Iterator_next(it);                         \
    alias##Iterator itPrev = alias##Iterator_prev(it);                         \
    itPrev.item->next = itNext.item;                                           \
    itNext.item->prev = itPrev.item;                                           \
                                                                               \
    free(it.item);                                                             \
    --l->size;                                                                 \
                                                                               \
    return itNext;                                                             \
  }

#endif // !LIST_MACROS_H
