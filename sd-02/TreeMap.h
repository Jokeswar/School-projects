#ifndef TREEMAP_H_
#define TREEMAP_H_

#include <stdlib.h>
#include <stdio.h>

#define MAX(a, b) (((a) >= (b))?(a):(b))
#define ABS(a) (((a) < 0) ? (-a) : (a))
#define CHK_ALLOC(ptr) if(ptr == NULL){fprintf(stderr, "Memory allocation\
 failed at line %d\nProgram will exit...\n", __LINE__ - 1); exit(-1);}


//-------------------------------------------------------------------------

typedef struct node{
	void* elem; // Node key
	void* info; // Node satellite information
	struct node *pt; // Parent link
	struct node *lt; // left child link
	struct node *rt; // right child link
	struct node* next; // next link in the list of duplicates
	struct node* prev; // prev link in the list of duplicates
	struct node* end; // pointer to the end of the duplicates list
	long height;
}TreeNode;

typedef struct TTree{
	TreeNode *root; // pointer to the root of the tree
	void* (*createElement)(void*); // create element method
	void (*destroyElement)(void*); // destroy element method
	void* (*createInfo)(void*); // create info method
	void (*destroyInfo)(void*); // destroy info method
	int (*compare)(void*, void*); // comparison method between elements
	long size;
}TTree;


TTree* createTree(void* (*createElement)(void*), void (*destroyElement)(void*),
		void* (*createInfo)(void*), void (*destroyInfo)(void*),
		int compare(void*, void*))
{
	// TODO: Cerinta 1 

	TTree* tree = (TTree*)malloc(sizeof(TTree));
	CHK_ALLOC(tree);

	tree->root = NULL;
	tree->size = 0;

	tree->compare = compare;
	tree->createElement = createElement;
	tree->createInfo = createInfo;
	tree->destroyElement = destroyElement;
	tree->destroyInfo = destroyInfo;

	return tree;
}

int isEmpty(TTree* tree)
{
	// TODO: Cerinta 1
	return (tree->size == 0);
}


TreeNode* search(TTree* tree, TreeNode* x, void* elem)
{
	// TODO: Cerinta 1

	TreeNode* current = tree->root;

	while(current != NULL)
		if(tree->compare(current->elem, elem) == 1)
			current = current->lt;
		else if(tree->compare(current->elem, elem) == -1)
			current = current->rt;
		else
			return current;

	return NULL;
}


TreeNode* minimum(TreeNode* x)
{
	// TODO: Cerinta 1
	// Note: Left-most element is the minimum
	if(x == NULL) return NULL;

	while(x->lt != NULL)
		x = x->lt;

	return x;
}

TreeNode* maximum(TreeNode* x)
{
	// TODO: Cerinta 1
	// Note: Right-most element is the maximum
	if(x == NULL) return NULL;

	while(x->rt != NULL)
		x = x->rt;

	return x;
}

TreeNode* successor(TreeNode* x)
{
	// TODO: Cerinta 1
	// Check function arguments 
	if(x == NULL) return NULL;
	TreeNode* succ = minimum(x->rt);

	if(succ != NULL) return succ;

	TreeNode* parent = x->pt;
	while(parent != NULL && parent->rt == x)
	{
		x = parent;
		parent = parent->pt;
	}

	return parent;
}

TreeNode* predecessor(TreeNode* x)
{
	// TODO: Cerinta 1
	// Check function arguments 
	if(x == NULL) return NULL;
	TreeNode* pred = maximum(x->lt);

	if(pred != NULL) return pred;

	TreeNode* parent = x->pt;
	while(parent != NULL && parent->lt == x)
	{
		x = parent;
		parent = parent->pt;
	}

	return parent;
}

void updateHeight(TreeNode* x)
{
	int leftHeight = 0;
	int rightHeight = 0;

	if(x != NULL){
		if(x->lt != NULL) leftHeight = x->lt->height;
		if(x->rt != NULL) rightHeight = x->rt->height;
		x->height = MAX(leftHeight, rightHeight) + 1;
	}
}

void avlRotateLeft(TTree* tree, TreeNode* x)
{
	// TODO: Cerinta 1
	TreeNode* y = x->rt;
	y->pt = x->pt;
	x->rt = y->lt;
	// fix the righ subtree of y
	if(x->rt != NULL)
		x->rt->pt = x;
	x->pt = y;
	y->lt = x;

	// check if y has parent and fix the links
	if(y->pt != NULL)
		if(y->pt->lt == x) y->pt->lt = y;
		else y->pt->rt = y;
	else
		tree->root = y; // no parent means x was root;

	updateHeight(x);
	updateHeight(y);
}

void avlRotateRight(TTree* tree, TreeNode* y)
{
	// TODO: Cerinta 1
	TreeNode* x = y->lt;
	x->pt = y->pt;
	y->lt = x->rt;
	// fix the left subtree of x
	if(y->lt != NULL)
		y->lt->pt = y;
	y->pt = x;
	x->rt = y;

	//check if x has parent and fix the links
	if(x->pt != NULL)
		if(x->pt->lt == y) x->pt->lt = x;
		else x->pt->rt = x;
	else
		tree->root = x; // no parent means y was root

	updateHeight(y);
	updateHeight(x);

}

/* Get AVL balance factor for node x */
int avlGetBalance(TreeNode *x)
{
	// TODO: Cerinta 1

	int left = 0;
	int right = 0;

	if(x->lt != NULL) left = x->lt->height;
	if(x->rt != NULL) right = x->rt->height;
	
	int diff = left - right;

	return ABS(diff);
}


void avlFixUp(TTree* tree, TreeNode* y)
{
	// TODO: Cerinta 1
	if(y == NULL)
		return;

	TreeNode* parent = y;
	TreeNode* child = y->lt;
	TreeNode* grandchild = NULL;

	// If the parent has at least one child choose the 
	// one with the greater height
	if(y->lt != NULL && y->rt != NULL)
		child = (y->lt->height >= y->rt->height) ? y->lt : y->rt;
	else if(y->lt == NULL)
		child = y->rt;
	
	// If the child has at least one child of it's own choose 
	// the one with the greater height
	if(child != NULL)
		if(child->lt != NULL && child->rt != NULL)
			grandchild = (child->lt->height >= child->rt->height) ? \
			child->lt : child->rt;
		else if(child->lt == NULL)
			grandchild = child->rt;
		else
			grandchild = child->lt;

	while(parent != NULL)
	{
		// Update height
		updateHeight(parent);

		// If the tree is unbalanced find the right case and solve it
		if(ABS(avlGetBalance(parent)) > 1)
		{
			if(parent->lt == child)
				if(child->lt == grandchild)
				{
					// left left
					avlRotateRight(tree, parent);
				}
				else
				{
					// left right
					avlRotateLeft(tree, child);
					avlRotateRight(tree, parent);
				}
			else
				if(child->rt == grandchild)
				{
					// right right
					avlRotateLeft(tree, parent);
				}
				else
				{
					// right left
					avlRotateRight(tree, child);
					avlRotateLeft(tree, parent);	
				}			
		}

		grandchild = child;
		child = parent;
		parent = parent->pt;
	}	
}


TreeNode* createTreeNode(TTree *tree, void* value, void* info)
{
	// Allocate node
	TreeNode* newNode = (TreeNode*) malloc(sizeof(TreeNode));

	// Set its element field
	newNode->elem = tree->createElement(value);

	//Set its info
	newNode->info = tree->createInfo(info);

	// Set its tree links
	newNode->pt = newNode->rt = newNode->lt = NULL;

	// Set its list links
	newNode->next = newNode->prev = newNode->end = NULL;

	/*
	 *  The height of a new node is 1,
	 *  while the height of NULL is 0
	 */
	newNode->height = 1;

	return newNode;
}

void destroyTreeNode(TTree *tree, TreeNode* node)
{
	// Check arguments
	if(tree == NULL || node == NULL) return;

	// Use object methods to de-alocate node fields
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);

	// De-allocate the node
	free(node);
}

void updateList(TreeNode* root)
{
	// Update tree list
	if(root == NULL)
		return;

	updateList(root->lt);
	root->prev = (predecessor(root) != NULL) ? predecessor(root)->end : NULL;
	root->end->next = successor(root);
	updateList(root->rt);
}

void updateTreeHeight(TreeNode* root)
{
	if(root == NULL)
		return;

	updateTreeHeight(root->lt);
	updateTreeHeight(root->rt);
	updateHeight(root);
}

void insert(TTree* tree, void* elem, void* info) 
{
	// NOTE: update succesor for lists end's
	// TODO: Cerinta 1
	TreeNode* newNode = createTreeNode(tree, elem, info);
	CHK_ALLOC(newNode);

	tree->size++;

	if(tree->root == NULL)
	{
		tree->root = newNode;
		tree->root->end = newNode;
		return;
	}

	TreeNode* current = tree->root;
	TreeNode* parent = NULL;

	while(current != NULL)
		if(tree->compare(current->elem, newNode->elem) == 1)
		{
			// Go left
			parent = current;
			current = current->lt;
		}
		else if(tree->compare(current->elem, newNode->elem) == -1)
		{
			// Go right
			parent = current;
			current = current->rt;
		}
		else
		{
			// Item exists, add it to list as the last element
			current->end->next = newNode;
			newNode->prev = current->end;
			current->end = newNode;
			// Last element of the list, it's next
			// is the succesor of the list's head
			newNode->next = successor(current);
			updateList(tree->root);
			return;
		}

		if(tree->compare(parent->elem, newNode->elem) == 1)
			parent->lt = newNode;
		else
			parent->rt = newNode;

		newNode->pt = parent;
		newNode->end = newNode;
		newNode->next = successor(newNode);
		newNode->prev = predecessor(newNode);

		avlFixUp(tree, newNode);
		updateList(tree->root);
}

void delete(TTree* tree, void* elem)
{
	// TODO: Cerinta 1
	TreeNode* node = search(tree, tree->root, elem);
	
	if(node == NULL) 
		return;

	tree->size--;

	if(node->end == node)
	{
		int type = (node->lt != NULL) + (node->rt != NULL);
		TreeNode* parent = node->pt;
		TreeNode* nextNode = NULL;
		TreeNode* fixSubtree = NULL;

		switch(type)
		{
		// leaf
		case 0:
			if(parent != NULL)
				if(parent->rt == node)
					parent->rt = NULL;
				else
					parent->lt = NULL;
			else
				tree->root = NULL;
			


			fixSubtree = parent;

			break;
		
		// 1 child
		case 1:
			// Get the child 
			if(node->rt != NULL)
				nextNode = node->rt;
			else
				nextNode = node->lt;
			

			nextNode->pt = parent;
			
			// Replace the parent-child connection with
			// parent-grandchild
			if(parent != NULL)
				if(parent->rt == node)
					parent->rt = nextNode;
				else 
					parent->lt = nextNode;
			else
				tree->root = nextNode;
			

			fixSubtree = nextNode;

			break;

		// 2 children
		case 2:
			nextNode = minimum(node->rt);

			// The minimum can have a right child. It doesn't matter if
			// if he is NULL. We update it's parent left connection.
			// Edge case: minimum is the right child of the Node.
			if(nextNode != node->rt)
			{
				// Break minimum's links
				nextNode->pt->lt = nextNode->rt;
				
				fixSubtree = nextNode->pt;

				nextNode->pt = parent;
				nextNode->lt = node->lt;
				nextNode->rt = node->rt;
				nextNode->lt->pt = nextNode;
				nextNode->rt->pt = nextNode;
			}
			else
			{
				fixSubtree = nextNode;
				nextNode->pt = parent;
				nextNode->lt = node->lt;
				nextNode->lt->pt = nextNode;			
			}

			if(parent != NULL)
				if(parent->rt == node)
					parent->rt = nextNode;
				else 
					parent->lt = nextNode;
			else
				tree->root = nextNode;

			break;

		default:
			break;
		}

		destroyTreeNode(tree, node);

		updateTreeHeight(tree->root);
		avlFixUp(tree, fixSubtree);
		updateList(tree->root);
	}
	else
	{
		// If key is not unique remove one item from the node's list
		TreeNode* listEnd = node->end;

		if(listEnd->prev != NULL)
			listEnd->prev->next = listEnd->next;

		if(listEnd->next != NULL)
			listEnd->next->prev = listEnd->prev;

		node->end = listEnd->prev;

		destroyTreeNode(tree, listEnd);
	}
}

void destroyTree(TTree* tree)
{
	// TODO: Cerinta 1
	/*
	 * Note: You can use the tree list to
	 * deallocate the hole tree.
	 */

	// The head of the tree list is at the minimum of the tree
	TreeNode* curr = minimum(tree->root);
	TreeNode* prev = NULL;

	while(curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		destroyTreeNode(tree, prev);
	}

	free(tree);
}


#endif /* TREEMAP_H_ */
