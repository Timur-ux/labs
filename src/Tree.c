#include "Tree.h"
#include "ListMacros.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode TreeNode;
LIST(Nodes, TreeNode *);

typedef struct TreeNode {
  double value;
  Nodes *childs;
} TreeNode;

struct Tree {
  TreeNode *root;
  NodesIterator rootIt;
};

Tree *Tree_create() {
  Tree *tree = (Tree *)malloc(sizeof(Tree));
  tree->root = NULL;
  tree->rootIt.item = (NodesItem *)malloc(sizeof(NodesItem));
  tree->rootIt.item->data = NULL;
  tree->rootIt.item->next = NULL;
  tree->rootIt.item->prev = NULL;

  return tree;
}

void TreeNode_destroy(TreeNode *node) {
  if (!node)
    return;

  for (NodesIterator it = Nodes_begin(node->childs);
       !NodesIterator_equal(it, Nodes_end(node->childs));
       it = NodesIterator_next(it)) {
    TreeNode_destroy(*NodesIterator_get(it));
  }

  Nodes_destroy(node->childs);
  free(node);
}

void Tree_destroy(Tree *tree) {
  TreeNode_destroy(tree->root);
  free(tree->rootIt.item);
  free(tree);
}

typedef struct {
  TreeNode *node, *parent;
} SearchData;

SearchData TreeNode_find(TreeNode *node, double value, TreeNode *parent) {
  SearchData data = {.node = NULL, .parent = parent};
  if (!node)
    return data;

  if (node->value == value) {
    data.node = node;
    return data;
  }

  for (NodesIterator it = Nodes_begin(node->childs);
       !NodesIterator_equal(it, Nodes_end(node->childs));
       it = NodesIterator_next(it)) {
    SearchData childData = TreeNode_find(*NodesIterator_get(it), value, node);
    if (childData.node)
      return childData;
  }

  return data;
}

TreeNode *TreeNode_create(double value) {
  TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));

  node->value = value;
  node->childs = Nodes_create();

  return node;
}

bool Tree_insert(Tree *tree, double parentValue, double value) {
  if (!tree->root) {
    tree->root = TreeNode_create(value);
    tree->rootIt.item->data = tree->root;
    return true;
  }
  SearchData data = TreeNode_find(tree->root, parentValue, NULL);
  TreeNode *parent = data.node;
  if (!parent)
    return false;
  Nodes_insert(parent->childs, Nodes_end(parent->childs),
               TreeNode_create(value));
  return true;
}

bool Tree_remove(Tree *tree, double value) {
  if (!tree->root)
    return false;

  SearchData data = TreeNode_find(tree->root, value, NULL);
  TreeNode *node = data.node, *parent = data.parent;
  if (!node)
    return false;

  TreeNode_destroy(node);
  if (!parent)
    return true;
  for (NodesIterator it = Nodes_begin(parent->childs);
       !NodesIterator_equal(it, Nodes_end(parent->childs));
       it = NodesIterator_next(it)) {
    if (*NodesIterator_get(it) == node) {
      Nodes_remove(parent->childs, it);
      break;
    }
  }
  return true;
}

void TreeNode_print(TreeNode *node, FILE *file, int depth) {
  if (!node)
    return;

  for (size_t i = 0; i < depth; ++i)
    fprintf(file, "\t");
  fprintf(file, "%lf\n", node->value);

  for (NodesIterator it = Nodes_begin(node->childs);
       !NodesIterator_equal(it, Nodes_end(node->childs));
       it = NodesIterator_next(it))
    TreeNode_print(*NodesIterator_get(it), file, depth + 1);
}

void Tree_print(Tree *tree, FILE *file) { TreeNode_print(tree->root, file, 0); }

TreeIterator Tree_begin(Tree *tree) {
  TreeIterator it = {.item = tree->rootIt.item};
  return it;
}

TreeIterator Tree_end(Tree *tree) {
  TreeIterator it = {.item = NULL};
  return it;
}

double *TreeIterator_get(TreeIterator it) {
  NodesItem *node = (NodesItem *)it.item;
  if (!node)
    return NULL;
  return &node->data->value;
}

bool TreeIterator_equal(TreeIterator lhs, TreeIterator rhs) {
  return lhs.item == rhs.item;
}

TreeIterator TreeIterator_childBegin(TreeIterator it) {
  NodesItem *node = (NodesItem *)it.item;
  if (!node)
    return it;

  TreeIterator child = {.item = Nodes_begin(node->data->childs).item};
  return child;
}

TreeIterator TreeIterator_childEnd(TreeIterator it) {
  NodesItem *node = (NodesItem *)it.item;
  if (!node)
    return it;

  TreeIterator child = {.item = Nodes_end(node->data->childs).item};
  return child;
}

TreeIterator TreeIterator_next(TreeIterator itChild) {
  NodesItem *child = (NodesItem *)itChild.item;
  if (!child)
    return itChild;

  child = child->next;
  itChild.item = child;
  return itChild;
}
