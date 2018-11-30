// Hunter Johnson
// COP 3502, Fall 2018
// NID | hu066164

#include <stdio.h>
#include <stdlib.h>
#include "KindredSpirits.h"

double difficultyRating(void)
{
	return 3.0;
}

double hoursSpent(void)
{
	return 11.0;
}

// Helper function that determines the preorder traversal
void preorderTraversal(node *root, int *tmp, int *i)
{
	if(root == NULL)
		return;

	// Save data into index pre
	tmp[(*i)++] = root->data;

	// Recursively loop through tree in preorder
	preorderTraversal(root->left, tmp, i);
	preorderTraversal(root->right, tmp, i);
}

// Helper function that determines postorder traversal
void postorderTraversal(node *root, int *tmp, int *i)
{
	if(root == NULL)
		return;

	// Recursively loop through tree in postorder
	preorderTraversal(root->left, tmp, i);
	preorderTraversal(root->right, tmp, i);

	// Save data into index post
	tmp[(*i)++] = root->data;
}

// Helper function that counts number of nodes in a tree
int size(node *root)
{
	if(root == NULL)
		return 0;

	return 1 + size(root->left) + size(root->right);
}

// Helper function that creates new node
// From testcase01 but pretty standard
node *createNode(int data)
{
	node *n = malloc(sizeof(node));

	n->data = data;
	n->left = n->right = NULL;

	return n;
}

// Determines whether trees at a and b are reflections of each other
// Returns 1 if trees the same, 0 otherwise
int isReflection(node *a, node *b)
{
	if(a == NULL && b == NULL)
		return 1;

	// Short circuit check if one is empty but not the other
	if(a == NULL || b == NULL)
		return 0;

	// Check if data in the nodes are equal to each other
	if(a->data != b->data)
		return 0;

	// Recursively check through both trees
	return (isReflection(a->left, b->right) && isReflection(a->right, b->left));
}

// Creates a tree that is a reflection of tree at root
// Returns pointer to root of new tree
node *makeReflection(node *root)
{
	node *reflection;

	if(root == NULL)
		return NULL;

	// Create a node and insert data into the reflection tree
	reflection = createNode(0);
	reflection->data = root->data;

	// Recursively go through each half of the tree
	reflection->left = makeReflection(root->right);
	reflection->right = makeReflection(root->left);

	return reflection;
}

// Determines whether trees at a and b are kindred spirits
// Returns 1 if so and 0 otherwise
int kindredSpirits(node *a, node *b)
{
	int i = 0, sizeA, sizeB, *arrA = NULL, *arrB = NULL;

	// Empty trees are kindred spirits
	if(a == NULL && b == NULL)
		return 1;

	if(a == NULL || b == NULL)
		return 0;

	// Both trees are the same therefore not kindred spirits
	if(a == b)
		return 0;

	// Check values in cases there is only one node
	if(a->left == NULL && a->right == NULL && b->left == NULL && b->right == NULL)
	{
		if(a->data != b->data)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

	// Get the size of each tree
	sizeA = size(a);
	sizeB = size(b);

	// Returns 0 if difference in number of nodes
	if(sizeA != sizeB)
		return 0;

	arrA = calloc(sizeA, sizeof(int));
	arrB = calloc(sizeB, sizeof(int));

	// Get pre and post order traversals
	preorderTraversal(a, arrA, &i);
	i = 0;
	preorderTraversal(a, arrB, &i);

	for(i = 0; i < sizeA; i++)
	{
		if(arrA[i] != arrB[i])
		{
			return 0;
		}
	}

	// Clean up after ourselves
	free(arrA);
	free(arrB);

	return 1;
}
