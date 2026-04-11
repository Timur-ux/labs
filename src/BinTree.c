#include "BinTree.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct Node {
  double value;
  struct Node *left, *right;
} Node;

struct BinTree {
  Node *root;
};

BinTree *BinTree_create() {
  BinTree *tree = (BinTree *)malloc(sizeof(BinTree));
  tree->root = NULL;

  return tree;
}

void Node_free(Node *node) {
  // Здесь корректно уничтожается отдельный узел и его данные
  free(node);
}

void Node_destroy(Node *node) {
  if (!node)
    return;
  Node_destroy(node->left);
  Node_destroy(node->right);
  Node_free(node);
}

void BinTree_destroy(BinTree *tree) {
  Node_destroy(tree->root);
  free(tree);
}

Node *Node_create(double value) {
  Node *result = (Node *)malloc(sizeof(Node));
  result->value = value;
  result->left = NULL;
  result->right = NULL;

  return result;
}

void Node_insertIterate(Node *node, double value) {
  if (node == NULL)
    return;
  while (node->value != value) {
    if (node->value > value) {
      if (node->left != NULL)
        node = node->left;
      else
        break;
    } else {
      if (node->right != NULL)
        node = node->right;
      else
        break;
    }
  }

  if (node->value == value)
    return;

  if (node->value > value)
    node->left = Node_create(value);
  else
    node->right = Node_create(value);
}

Node *Node_insertRecurse(Node *node, double value) {
  if (node == NULL)
    return Node_create(value);

  if (node->value == value)
    return node;

  if (node->value > value)
    node->left = Node_insertRecurse(node->left, value);
  else
    node->right = Node_insertRecurse(node->right, value);

  return node;
}

void BinTree_insert(BinTree *tree, double value) {
  tree->root = Node_insertRecurse(tree->root, value);
}

bool Node_isHaveBothChildren(Node *node) {
  return node && node->left && node->right;
}

void Node_parentReplaceChild(Node *parent, double value, Node *child) {
  if (parent) {
    if (parent->value > value)
      parent->left = child;
    else
      parent->right = child;
  }
}

void Node_remove(BinTree *tree, Node *node, double value, Node *parent) {
  while (node && node->value != value) {
    parent = node;
    if (node->value > value)
      node = node->left;
    else
      node = node->right;
  }

  if (!node)
    return;

  if (!Node_isHaveBothChildren(node)) {
    Node *child = node->left ? node->left : node->right;
    Node_parentReplaceChild(parent, node->value, child);
    Node_free(node);
		if(node == tree->root)
			tree->root = child;
    return;
  }

  Node *right = node->right, *leftMost = right;
  while (leftMost->left)
    leftMost = leftMost->left;

  node->value = leftMost->value;
  Node_remove(tree, right, leftMost->value, node);
}

void BinTree_remove(BinTree *tree, double value) {
  Node_remove(tree, tree->root, value, NULL);
}

void Node_print(Node *node, FILE *file, int depth) {
  if (!node)
    return;

  Node_print(node->right, file, depth + 1);
  for (int i = 0; i < depth; ++i)
    fprintf(file, " ");
  fprintf(file, "%lf\n", node->value);
  Node_print(node->left, file, depth + 1);
}

void BinTree_print(BinTree *tree, FILE *file) {
  Node_print(tree->root, file, 0);
}

BinTreeIterator BinTree_begin(BinTree *tree) {
  BinTreeIterator result = {.item = tree->root};
  return result;
}

BinTreeIterator BinTree_end(BinTree *tree) {
  BinTreeIterator result = {.item = NULL};
  return result;
}

bool BinTreeIterator_equal(BinTreeIterator lhs, BinTreeIterator rhs) {
  return lhs.item == rhs.item;
}

BinTreeIterator BinTreeIterator_left(BinTreeIterator it) {
  Node *node = (Node *)it.item;
  if (!node)
    return it;
  node = node->left;
  it.item = node;
  return it;
}

BinTreeIterator BinTreeIterator_right(BinTreeIterator it) {
  Node *node = (Node *)it.item;
  if (!node)
    return it;
  node = node->right;
  it.item = node;
  return it;
}

double BinTreeIterator_get(BinTreeIterator it) {
  Node *node = (Node *)it.item;
  if (!node)
    return 0;
  return node->value;
}

bool BinTreeIterator_isLeaf(BinTreeIterator it) {
  Node *node = (Node *)it.item;
  return node && !node->left && !node->right;
}
