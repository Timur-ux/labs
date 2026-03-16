#ifndef LIST_MACROS_H_
#define LIST_MACROS_H_
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define LIST(alias, T)                                                         \
  typedef struct alias##Item {                                                 \
    struct alias##Item *next, *prev;                                           \
    T data;                                                                    \
  } alias##Item;                                                               \
                                                                               \
  typedef struct alias {                                                       \
    alias##Item *terminator;                                                   \
    size_t size;                                                               \
  } alias;                                                                     \
                                                                               \
  typedef struct {                                                             \
    alias##Item *item;                                                         \
  } alias##Iterator;                                                           \
                                                                               \
  static inline alias *alias##_create();                                       \
  static inline void alias##_destroy(alias *l);                                \
                                                                               \
  static inline size_t alias##_size(alias *l);                                 \
  static inline bool alias##_empty(alias *l);                                  \
                                                                               \
  static inline alias##Iterator alias##_insert(alias *l, alias##Iterator it,   \
                                               T data);                        \
  static inline alias##Iterator alias##_remove(alias *l, alias##Iterator it);  \
                                                                               \
  static inline void alias##_swap(alias *l, alias##Iterator it1,               \
                                  alias##Iterator it2);                        \
                                                                               \
  static inline alias##Iterator alias##_begin(alias *l);                       \
  static inline alias##Iterator alias##_end(alias *l);                         \
                                                                               \
  static inline alias##Iterator alias##Iterator_next(alias##Iterator it);      \
  static inline alias##Iterator alias##Iterator_prev(alias##Iterator it);      \
  static inline bool alias##Iterator_equal(alias##Iterator lhs,                \
                                           alias##Iterator rhs);               \
  static inline T *alias##Iterator_get(alias##Iterator it);                    \
                                                                               \
  alias *alias##_create() {                                                    \
    alias *l = (alias *)malloc(sizeof(alias));                                 \
    l->size = 0;                                                               \
                                                                               \
    l->terminator = (alias##Item *)malloc(sizeof(alias##Item));                \
    l->terminator->next = l->terminator;                                       \
    l->terminator->prev = l->terminator;                                       \
                                                                               \
    return l;                                                                  \
  }                                                                            \
                                                                               \
  void alias##_destroy(alias *l) {                                             \
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
  alias##Iterator alias##_insert(alias *l, alias##Iterator it, T data) {       \
    alias##Iterator itPrev = alias##Iterator_prev(it);                         \
                                                                               \
    alias##Item *newItem = (alias##Item *)malloc(sizeof(alias##Item));         \
    newItem->next = it.item;                                                   \
    newItem->prev = itPrev.item;                                               \
                                                                               \
    it.item->prev = newItem;                                                   \
    itPrev.item->next = newItem;                                               \
                                                                               \
    newItem->data = data;                                                      \
    ++l->size;                                                                 \
                                                                               \
    alias##Iterator result = {.item = newItem};                                \
    return result;                                                             \
  }                                                                            \
                                                                               \
  alias##Iterator alias##_remove(alias *l, alias##Iterator it) {               \
    if (alias##Iterator_equal(it, alias##_end(l)))                             \
      return alias##_end(l);                                                   \
                                                                               \
    alias##Iterator itPrev = alias##Iterator_prev(it),                         \
                    itNext = alias##Iterator_next(it);                         \
    itPrev.item->next = itNext.item;                                           \
    itNext.item->prev = itPrev.item;                                           \
                                                                               \
    free(it.item);                                                             \
                                                                               \
    --l->size;                                                                 \
    return itNext;                                                             \
  }                                                                            \
                                                                               \
  void alias##_swap(alias *l, alias##Iterator it1, alias##Iterator it2) {      \
    T temp = it1.item->data;                                                   \
    it1.item->data = it2.item->data;                                           \
    it2.item->data = temp;                                                     \
  }                                                                            \
                                                                               \
  alias##Iterator alias##_begin(alias *l) {                                    \
    alias##Iterator it = alias##_end(l);                                       \
    return alias##Iterator_next(it);                                           \
  }                                                                            \
  alias##Iterator alias##_end(alias *l) {                                      \
    alias##Iterator it = {.item = l->terminator};                              \
    return it;                                                                 \
  }                                                                            \
                                                                               \
  alias##Iterator alias##Iterator_next(alias##Iterator it) {                   \
    it.item = it.item->next;                                                   \
    return it;                                                                 \
  }                                                                            \
                                                                               \
  alias##Iterator alias##Iterator_prev(alias##Iterator it) {                   \
    it.item = it.item->prev;                                                   \
    return it;                                                                 \
  }                                                                            \
                                                                               \
  bool alias##Iterator_equal(alias##Iterator lhs, alias##Iterator rhs) {       \
    return lhs.item == rhs.item;                                               \
  }                                                                            \
                                                                               \
  T *alias##Iterator_get(alias##Iterator it) { return &it.item->data; }

#endif // !LIST_MACROS_H_
