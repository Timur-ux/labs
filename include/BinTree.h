#ifndef BIN_TREE_H_
#define BIN_TREE_H_

typedef struct BinTree BinTree;

BinTree * BinTree_create();
void BinTree_destroy(BinTree *tree);

void BinTree_insert(BinTree *tree, double value);
void BinTree_remove(BinTree *tree, double value);
void BinTree_print(BinTree * tree, FILE *file);


#endif // !BIN_TREE_H_
