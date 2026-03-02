#ifndef VECTOR_MACROS_H_
#define VECTOR_MACROS_H_

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define VECTOR(alias, type)                                                    \
  typedef struct {                                                             \
    type *data;                                                                \
    size_t capacity;                                                           \
    size_t size;                                                               \
  } alias;                                                                     \
                                                                               \
  static inline alias alias##_create() {                                       \
    alias result;                                                              \
    result.size = 0;                                                           \
    result.capacity = 5;                                                       \
    result.data = malloc(result.capacity * sizeof(type));                      \
    return result;                                                             \
  }                                                                            \
                                                                               \
  static inline void alias##_destroy(alias *v) {                               \
    if (v->data == NULL)                                                       \
      return;                                                                  \
                                                                               \
    free(v->data);                                                             \
    v->data = NULL;                                                            \
    v->size = 0;                                                               \
    v->capacity = 0;                                                           \
  }                                                                            \
                                                                               \
  static inline type *alias##_at(alias *v, size_t i) {                         \
    assert(("Index out of range", i < v->size));                               \
    return &v->data[i];                                                        \
  }                                                                            \
                                                                               \
  static inline void alias##_push_back(alias *v, type element) {               \
    if (v->size >= v->capacity) {                                              \
      v->capacity *= 2;                                                        \
      v->data = realloc(v->data, v->capacity * sizeof(type));                  \
    }                                                                          \
    v->data[v->size] = element;                                                \
    ++v->size;                                                                 \
  }                                                                            \
                                                                               \
  static inline bool alias##_empty(alias *v) { return (v->size == 0); }        \
                                                                               \
  static inline void alias##_pop_back(alias *v) {                              \
    if (alias##_empty(v))                                                      \
      return;                                                                  \
                                                                               \
    --v->size;                                                                 \
  }                                                                            \
                                                                               \
  static inline size_t alias##_size(alias *v) { return v->size; }

#endif // VECTOR_MACROS_H_
