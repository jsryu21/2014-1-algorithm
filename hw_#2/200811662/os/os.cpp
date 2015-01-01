//Homework2 skeleton code

#include<cstdio>
#include<iostream>
#include<stdlib.h>

struct Node
{
	enum COLOR
	{
		RED,
		BLACK,
	};
	Node()
		: key(0)
		, size(0)
		, p(NULL)
		, left(NULL)
		, right(NULL)
		, color(BLACK)
	{
	}
	int key;
	int size;
	Node* p;
	Node* left;
	Node* right;
	COLOR color;
};

typedef struct orderStatisticTree
{
	orderStatisticTree()
	{
		nil = new Node();
		root = nil;
	}
	Node* nil;
	Node* root;
} OSTree;

OSTree T;

void LEFT_ROTATE(Node* x)
{
	// Same as Pseudocode
	Node* y = x->right;
	x->right = y->left;
	if (y->left != T.nil)
	{
		y->left->p = x;
	}
	y->p = x->p;
	if (x->p == T.nil)
	{
		T.root = y;
	}
	else if (x == x->p->left)
	{
		x->p->left = y;
	}
	else
	{
		x->p->right = y;
	}
	y->left = x;
	x->p = y;
	y->size = x->size;
	x->size = x->left->size + x->right->size + 1;
}

void RIGHT_ROTATE(Node* y)
{
	// Same as Pseudocode
	Node* x = y->left;
	y->left = x->right;
	if (x->right != T.nil)
	{
		x->right->p = y;
	}
	x->p = y->p;
	if (y->p == T.nil)
	{
		T.root = x;
	}
	else if (y == y->p->left)
	{
		y->p->left = x;
	}
	else
	{
		y->p->right = x;
	}
	x->right = y;
	y->p = x;
	x->size = y->size;
	y->size = y->left->size + y->right->size + 1;
}

void RB_INSERT_FIXUP(Node* z)
{
	// Same as Pseudocode
	while (z->p->color == Node::RED)
	{
		if (z->p == z->p->p->left)
		{
			Node* y = z->p->p->right;
			if (y->color == Node::RED)
			{
				z->p->color = Node::BLACK;
				y->color = Node::BLACK;
				z->p->p->color = Node::RED;
				z = z->p->p;
			}
			else
			{
				if (z == z->p->right)
				{
					z = z->p;
					LEFT_ROTATE(z);
				}
				z->p->color = Node::BLACK;
				z->p->p->color = Node::RED;
				RIGHT_ROTATE(z->p->p);
			}
		}
		else
		{
			Node* y = z->p->p->left;
			if (y->color == Node::RED)
			{
				z->p->color = Node::BLACK;
				y->color = Node::BLACK;
				z->p->p->color = Node::RED;
				z = z->p->p;
			}
			else
			{
				if (z == z->p->left)
				{
					z = z->p;
					RIGHT_ROTATE(z);
				}
				z->p->color = Node::BLACK;
				z->p->p->color = Node::RED;
				LEFT_ROTATE(z->p->p);
			}
		}
	}
	T.root->color = Node::BLACK;
}

void RB_INSERT(Node* z)
{
	// Same as Pseudocode
	Node* y = T.nil;
	Node* x = T.root;
	while (x != T.nil)
	{
		y = x;
		if (z->key < x->key)
		{
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}
	z->p = y;
	if (y == T.nil)
	{
		T.root = z;
	}
	else if (z->key < y->key)
	{
		y->left = z;
	}
	else
	{
		y->right = z;
	}
	z->left = T.nil;
	z->right = T.nil;
	z->color = Node::RED;
	// update size
	Node* i = z;
	while (i != T.root)
	{
		i->size++;
		i = i->p;
	}
	i->size++;
	RB_INSERT_FIXUP(z);
}

Node* TREE_SEARCH(Node* x, int k)
{
	// Same as Pseudocode
	if (x == T.nil || k == x->key)
	{
		return x;
	}
	if (k < x->key)
	{
		return TREE_SEARCH(x->left, k);
	}
	else
	{
		return TREE_SEARCH(x->right, k);
	}
}

Node* TREE_MINIMUM(Node* x)
{
	// Same as Pseudocode
	while (x->left != T.nil)
	{
		x = x->left;
	}
	return x;
}

Node* TREE_MAXIMUM(Node* x)
{
	// Same as Pseudocode
	while (x->right != T.nil)
	{
		x = x->right;
	}
	return x;
}

void RB_TRANSPLANT(Node* u, Node* v)
{
	// Same as Pseudocode
	if (u->p == T.nil)
	{
		T.root = v;
	}
	else if (u == u->p->left)
	{
		u->p->left = v;
	}
	else
	{
		u->p->right = v;
	}
	v->p = u->p;
}

void RB_DELETE_FIXUP(Node* x)
{
	// Same as Pseudocode
	while (x != T.root && x->color == Node::BLACK)
	{
		if (x == x->p->left)
		{
			Node* w = x->p->right;
			if (w->color == Node::RED)
			{
				w->color = Node::BLACK;
				x->p->color = Node::RED;
				LEFT_ROTATE(x->p);
				w = x->p->right;
			}
			if (w->left->color == Node::BLACK && w->right->color == Node::BLACK)
			{
				w->color = Node::RED;
				x = x->p;
			}
			else
			{
				if (w->right->color == Node::BLACK)
				{
					w->left->color = Node::BLACK;
					w->color = Node::RED;
					RIGHT_ROTATE(w);
					w = x->p->right;
				}
				w->color = x->p->color;
				x->p->color = Node::BLACK;
				w->right->color = Node::BLACK;
				LEFT_ROTATE(x->p);
				x = T.root;
			}
		}
		else
		{
			Node* w = x->p->left;
			if (w->color == Node::RED)
			{
				w->color = Node::BLACK;
				x->p->color = Node::RED;
				RIGHT_ROTATE(x->p);
				w = x->p->left;
			}
			if (w->right->color == Node::BLACK && w->left->color == Node::BLACK)
			{
				w->color = Node::RED;
				x = x->p;
			}
			else
			{
				if (w->left->color == Node::BLACK)
				{
					w->right->color = Node::BLACK;
					w->color = Node::RED;
					LEFT_ROTATE(w);
					w = x->p->left;
				}
				w->color = x->p->color;
				x->p->color = Node::BLACK;
				w->left->color = Node::BLACK;
				RIGHT_ROTATE(x->p);
				x = T.root;
			}
		}
	}
	x->color = Node::BLACK;
}

void RB_DELETE(Node* z)
{
	// Same as Pseudocode
	Node* y = z;
	Node::COLOR y_original_color = y->color;
	Node* x;
	if (z->left == T.nil)
	{
		// update size
		Node* d = y;
		while (d != T.root)
		{
			d->size--;
			d = d->p;
		}
		d->size--;
		x = z->right;
		RB_TRANSPLANT(z, z->right);
	}
	else if (z->right == T.nil)
	{
		// update size
		Node* d = y;
		while (d != T.root)
		{
			d->size--;
			d = d->p;
		}
		d->size--;
		x = z->left;
		RB_TRANSPLANT(z, z->left);
	}
	else
	{
		y = TREE_MINIMUM(z->right);
		// update size
		Node* d = y;
		while (d != T.root)
		{
			d->size--;
			d = d->p;
		}
		d->size--;
		y_original_color = y->color;
		x = y->right;
		if (y->p == z)
		{
			x->p = y;
		}
		else
		{
			RB_TRANSPLANT(y, y->right);
			y->right = z->right;
			y->right->p = y;
		}
		RB_TRANSPLANT(z, y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
		y->size = z->size;
	}
	if (y_original_color == Node::BLACK)
	{
		RB_DELETE_FIXUP(x);
	}
	free(z);
}

int OSInsert(int x)
{
	if (TREE_SEARCH(T.root, x) != T.nil)
	{
		// x is already in
		return 0;
	}
	Node* z = new Node();
	z->key = x;
	RB_INSERT(z);
	return x;
}

int OSDelete(int x)
{
	Node* z = TREE_SEARCH(T.root, x);
	if (z == T.nil)
	{
		// x not found
		return 0;
	}
	RB_DELETE(z);
	return x;
}

int OSSelect(Node* x, int i)
{
	// Same as Pseudocode
	int r = x->left->size + 1;
	if (i == r)
	{
		return x->key;
	}
	else if (i < r)
	{
		return OSSelect(x->left, i);
	}
	else
	{
		return OSSelect(x->right, i - r);
	}
}

int OSSelect(int i)
{
	if (i > T.root->size)
	{
		// i is too big
		return 0;
	}
	return OSSelect(T.root, i);
}

int OSRank(Node* x)
{
	// Same as Pseudocode
	int r = x->left->size + 1;
	Node* y = x;
	while (y != T.root)
	{
		if (y == y->p->right)
		{
			r = r + y->p->left->size + 1;
		}
		y = y->p;
	}
	return r;
}

int OSRank(int x)
{
	Node* z = TREE_SEARCH(T.root, x);
	if (z == T.nil)
	{
		// x not found.
		return 0;
	}
	return OSRank(z);
}

int Operation(char Arg1, int Arg2)
{
	int ret;
	switch (Arg1)
	{
	case 'I':
		ret = OSInsert(Arg2);
		break;
	case 'D':
		ret = OSDelete(Arg2);
		break;
	case 'S':
		ret = OSSelect(Arg2);
		break;
	case 'R':
		ret = OSRank(Arg2);
		break;
	default:
		//error
		break;
	}
	return ret;
}

int main(int argc, char** argv)
{
	FILE* inFile = fopen(argv[1], "r");
	FILE* outFile = fopen(argv[2], "w");
	char Arg1;
	int Arg2;

	while (fscanf(inFile, "%c %d ", &Arg1, &Arg2) != EOF)
	{
		fprintf(outFile, "%d ", Operation(Arg1, Arg2));
	}
	fclose(outFile);
	fclose(inFile);
	return 0;
}
