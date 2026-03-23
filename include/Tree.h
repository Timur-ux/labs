#ifndef TREE_H_
#define TREE_H_
#include <stdbool.h>
#include <stdio.h>
struct Tree;
typedef struct Tree Tree;

typedef struct {
  void *item;
} TreeIterator;

Tree *Tree_create();
void Tree_destroy(Tree *tree);

bool Tree_insert(Tree *tree, double parentValue, double value);
bool Tree_remove(Tree *tree, double value);
void Tree_print(Tree *tree, FILE *file);

TreeIterator Tree_begin(Tree *tree);
TreeIterator Tree_end(Tree *tree);
double *TreeIterator_get(TreeIterator it);
bool TreeIterator_equal(TreeIterator lhs, TreeIterator rhs);

TreeIterator TreeIterator_childBegin(TreeIterator it);
TreeIterator TreeIterator_childEnd(TreeIterator it);
TreeIterator TreeIterator_next(TreeIterator it);

#endif // TREE_H_
