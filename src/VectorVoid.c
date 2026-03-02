#include "VectorVoid.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

Vector Vector_create(size_t elementSize) {
	Vector result;

	result.elementSize = elementSize;
	result.size = 0;
	result.capacity = 5;

	result.data = malloc(result.capacity * elementSize);
	return result;
}

void Vector_destroy(Vector *v) {
	if(v->data == NULL) 
		return;

	free(v->data);
	v->data = NULL;
	v->size = 0;
	v->capacity = 0;
	v->elementSize = 0;
}


void * Vector_at(Vector* v, size_t i) {
	assert(("Index out of range", i < v->size));
	return v->data + i * v->elementSize;
}

void Vector_push_back(Vector* v, void* element) {
	if(v->size >= v->capacity) { // Нужно увеличить размер
		v->capacity *= 2;
		v->data = realloc(v->data, v->capacity * v->elementSize);
	}
	memcpy(v->data + v->size * v->elementSize, element, v->elementSize); // v->data[v->size] = *element
	++v->size;
}

void Vector_pop_back(Vector* v) {
	if(Vector_empty(v))
		return;

	--v->size;
}


bool Vector_empty(Vector *v) {
	return (v->size == 0);
}

size_t Vector_size(Vector *v) {
	return v->size;
}


