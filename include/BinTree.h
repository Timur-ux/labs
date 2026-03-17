#ifndef BIN_TREE_H_
#define BIN_TREE_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct BinTree BinTree;

typedef struct {
	void *item;
} BinTreeIterator;

BinTree * BinTree_create();
void BinTree_destroy(BinTree *tree);

void BinTree_insert(BinTree *tree, double value);
void BinTree_remove(BinTree *tree, double value);
void BinTree_print(BinTree * tree, FILE *file);

BinTreeIterator BinTree_begin(BinTree * tree);
BinTreeIterator BinTree_end(BinTree * tree);

bool BinTreeIterator_equal(BinTreeIterator lhs, BinTreeIterator rhs);

BinTreeIterator BinTreeIterator_left(BinTreeIterator it);
BinTreeIterator BinTreeIterator_right(BinTreeIterator it);
double BinTreeIterator_get(BinTreeIterator it);
bool BinTreeIterator_isLeaf(BinTreeIterator it);

#endif // !BIN_TREE_H_
