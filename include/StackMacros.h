#ifndef STACK_H_
#define STACK_H_

#include "VectorMacros.h"
#define STACK(alias, T)                                                        \
  VECTOR(alias##Items, T);                                                     \
  typedef struct {                                                             \
    alias##Items items;                                                        \
  } alias;                                                                     \
                                                                               \
  static inline alias alias##_Create() {                                                     \
    alias stack = {.items = alias##Items_create()};                            \
    return stack;                                                              \
  }                                                                            \
  static inline void alias##_Destroy(alias *stack) { alias##Items_destroy(&stack->items); }  \
                                                                               \
  static inline size_t alias##_Size(alias *stack) {                                          \
    return alias##Items_size(&stack->items);                                   \
  }                                                                            \
  static inline size_t alias##_Empty(alias *stack) {                                         \
    return alias##Items_empty(&stack->items);                                  \
  }                                                                            \
                                                                               \
  static inline void alias##_Push(alias *stack, T value) {                                   \
    alias##Items_push_back(&stack->items, value);                              \
  }                                                                            \
  static inline void alias##_Pop(alias *stack) { alias##Items_pop_back(&stack->items); }     \
                                                                               \
  static inline T *alias##_Top(alias *stack) {                                               \
    size_t itemsSize = alias##Items_size(&stack->items);                       \
    if (itemsSize == 0)                                                        \
      return NULL;                                                             \
    return alias##Items_at(&stack->items, itemsSize - 1);                      \
  }

#endif // !STACK_H_
