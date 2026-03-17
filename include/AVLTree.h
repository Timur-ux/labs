#ifndef BIN_TREE_H_
#define BIN_TREE_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct AVLTree AVLTree;

typedef struct {
	void *item;
} AVLTreeIterator;

AVLTree * AVLTree_create();
void AVLTree_destroy(AVLTree *tree);

void AVLTree_insert(AVLTree *tree, double value);
void AVLTree_remove(AVLTree *tree, double value);
void AVLTree_print(AVLTree * tree, FILE *file);

AVLTreeIterator AVLTree_begin(AVLTree * tree);
AVLTreeIterator AVLTree_end(AVLTree * tree);

bool AVLTreeIterator_equal(AVLTreeIterator lhs, AVLTreeIterator rhs);

AVLTreeIterator AVLTreeIterator_left(AVLTreeIterator it);
AVLTreeIterator AVLTreeIterator_right(AVLTreeIterator it);
double AVLTreeIterator_get(AVLTreeIterator it);
bool AVLTreeIterator_isLeaf(AVLTreeIterator it);

#endif // !BIN_TREE_H_
