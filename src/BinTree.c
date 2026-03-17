#include "BinTree.h"

#include <stdlib.h>

typedef struct Node {
	double value;
	struct Node *left, *right;
} Node;

struct BinTree {
	Node * root;
};

BinTree * BinTree_create() {
	BinTree * tree = (BinTree*)malloc(sizeof(BinTree));
	tree->root = NULL;

	return tree;
}

void Node_destroy(Node *node);
void BinTree_destroy(BinTree *tree) {
	Node_destroy(tree->root);
	free(tree);
}

Node* Node_create(double value) {
	Node *result = (Node*)malloc(sizeof(Node));
	result->value = value;
	result->left = NULL;
	result->right = NULL;

	return result;
}


void Node_insertIterate(Node * node, double value) {
	if(node == NULL)
		return;
	while(node->value != value) {
		if(node->value > value) {
			if(node->left != NULL)
				node = node->left;
			else
				break;
		} else {
			if(node->right != NULL)
				node = node->right;
			else
				break;
		}
	}

	if(node->value == value)
		return;

	if(node->value > value)
		node->left = Node_create(value);
	else
		node->right = Node_create(value);
}

Node * Node_insertRecurse(Node * node, double value) {
	if(node == NULL)
		return Node_create(value);

	if(node->value == value)
		return node;

	if(node->value > value) 
			node->left = Node_insertRecurse(node->left, value);
	else
		node->right = Node_insertRecurse(node->right, value);

	return node;
}

void BinTree_insert(BinTree *tree, double value) {
	tree->root = Node_insertRecurse(tree->root, value);
}

void BinTree_remove(BinTree *tree, double value) {

}
void BinTree_print(BinTree * tree, FILE *file);
