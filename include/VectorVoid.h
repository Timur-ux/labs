#ifndef VECTOR_VOID_H_
#define VECTOR_VOID_H_

#include <stddef.h>
#include <stdbool.h>

typedef struct {
	void * data;
	size_t elementSize; // Размер одного элемента
	size_t capacity; // Размер выделенной памяти
	size_t size; // Количество элементов в векторе
} Vector;

Vector Vector_create(size_t elementSize);
void Vector_destroy(Vector *v);

void * Vector_at(Vector* v, size_t i);
void Vector_push_back(Vector* v, void* element);
void Vector_pop_back(Vector* v);

bool Vector_empty(Vector *v);
size_t Vector_size(Vector *v);


#endif // VECTOR_VOID_H_
