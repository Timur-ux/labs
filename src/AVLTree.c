#include "AVLTree.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
	double value;
	struct Node *left, *right;
	int balance; // h_r - h_l
} Node;

struct AVLTree {
	Node * root;
};

AVLTree * AVLTree_create() {
	AVLTree * tree = (AVLTree*)malloc(sizeof(AVLTree));
	tree->root = NULL;

	return tree;
}

static inline void Node_free(Node *node) {
	// Здесь корректно уничтожается отдельный узел и его данные
	free(node);
}

static inline void Node_destroy(Node *node) {
	if(!node)
		return;
	Node_destroy(node->left);
	Node_destroy(node->right);
	Node_free(node);
}

void AVLTree_destroy(AVLTree *tree) {
	Node_destroy(tree->root);
	free(tree);
}

static inline Node* Node_create(double value) {
	Node *result = (Node*)malloc(sizeof(Node));
	result->value = value;
	result->left = NULL;
	result->right = NULL;
	result->balance = 0;

	return result;
}

/*
 *		(x)2
 *				(y)-1
 *			(z)
 *
 * ---------------------------------
 *  		(x)
 *  			(z)
 *  				(y)
 *
 *
 *
 */


Node *rotateLeft(Node *node) {
	Node *right = node->right;
	node->right = right->left; // x->right = beta
	right->left = node; // y->left = x
	return right;
}

Node *rotateRight(Node *node) {
	Node *left = node->left;
	node->left = left->right; // x->left = beta
	left->right = node; // y->right = x
	return left;
}

Node* RebalanceRR(Node *node) {
	Node *right = node->right;
	if(right->balance == 1) {
		rotateLeft(node);
		node->balance = 0;
		right->balance = 0;
		return right;
	}
	Node *rightLeft = right->left;
	rotateRight(right);
	rotateLeft(node);

	if(rightLeft->balance == -1) {
		node->balance = 0;
		right->balance = 1;
	}
	else if(rightLeft->balance == 0) {
		node->balance = 0;
		right->balance = 0;
	}
	else {
		node->balance = -1;
		right->balance = 0;
	}
	rightLeft->balance = 0;
	return rightLeft;
}

Node* RebalanceLL(Node *node) {
}


static inline Node * Node_insertRecurse(Node * node, double value, bool *rebalanceNeed) {
	if(node == NULL) {
		*rebalanceNeed = true;
		return Node_create(value);
	}

	if(node->value == value)
		return node;

	if(node->value > value) {
		node->left = Node_insertRecurse(node->left, value, rebalanceNeed);
		if(*rebalanceNeed)
			node->balance -= 1;
	} else {
		node->right = Node_insertRecurse(node->right, value, rebalanceNeed);
		if(*rebalanceNeed)
			node->balance += 1;
	}
	if(node->balance == 0)
		*rebalanceNeed = false;
	// else if(node->balance == 1 || node->balance == -1) *rebalanceNeed = true;
	else if(node->balance == 2) {
		node = RebalanceRR(node);
		*rebalanceNeed = false;
	}
	else if(node->balance == -2) {
		node = RebalanceLL(node);
		*rebalanceNeed = false;
	}

	return node;
}

void AVLTree_insert(AVLTree *tree, double value) {
	bool rebalanceNeed;
	tree->root = Node_insertRecurse(tree->root, value, &rebalanceNeed);
}

static inline bool Node_isHaveBothChildren(Node *node) { 
	return node && node->left && node->right;
}


static inline void Node_parentReplaceChild(Node *parent, double value, Node *child) {
	if(parent) {
		if(parent->value > value)
			parent->left = child;
		else
			parent->right = child;
	}
}

static inline void Node_remove(Node *node, double value, Node *parent) {
	while(node && node->value != value) {
		parent = node;
		if(node->value > value)
			node = node->left;
		else
			node = node->right;
	}

	if(!node)
		return;

	if(!Node_isHaveBothChildren(node)) {
		Node * child = NULL;
		if(!node->left || !node->right) 
			child = node->left ? node->left : node->right;

		Node_parentReplaceChild(parent, node->value, child);
		Node_free(node);
		return;
	}

	Node * right = node->right, *leftMost = right;
	while(leftMost->left)
		leftMost = leftMost->left;

	node->value = leftMost->value;
	Node_remove(right, leftMost->value, node);
}

void AVLTree_remove(AVLTree *tree, double value) {
	Node_remove(tree->root, value, NULL);
}

static inline void Node_print(Node *node, FILE *file, int depth) {
	if(!node)
		return;

	Node_print(node->right, file, depth + 1);
	for(int i = 0; i < depth; ++i) 
		fprintf(file, "\t");
	fprintf(file, "%lf\n", node->value);
	Node_print(node->left, file, depth + 1);
}

void AVLTree_print(AVLTree * tree, FILE *file) {
	Node_print(tree->root, file, 0);
}

AVLTreeIterator AVLTree_begin(AVLTree * tree) {
	AVLTreeIterator result = {.item = tree->root};
	return result;
}
AVLTreeIterator AVLTree_end(AVLTree * tree) {
	AVLTreeIterator result = {.item = NULL};
	return result;
}

bool AVLTreeIterator_equal(AVLTreeIterator lhs, AVLTreeIterator rhs) {
	return lhs.item == rhs.item;
}

AVLTreeIterator AVLTreeIterator_left(AVLTreeIterator it) {
	Node * node = (Node*)it.item;
	if(!node)
		return it;
	node = node->left;
	it.item = node;
	return it;
}
AVLTreeIterator AVLTreeIterator_right(AVLTreeIterator it) {
	Node * node = (Node*)it.item;
	if(!node)
		return it;
	node = node->right;
	it.item = node;
	return it;
}

double AVLTreeIterator_get(AVLTreeIterator it) {
	Node * node = (Node*)it.item;
	if(!node)
		return 0;
	return node->value;

}
bool AVLTreeIterator_isLeaf(AVLTreeIterator it) {
	Node * node = (Node*)it.item;
	return node && !node->left && !node->right;
}
