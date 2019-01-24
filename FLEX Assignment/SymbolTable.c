#include "SymbolTable.h"

int height(struct Node_tree *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

int Max(int a, int b)
{
	if (a>b)
		return a;
	return b;
}

Node_tree *newNode(char *Name, eRoles Role, eCategories Category, char *Type, char *Subtype, int Size)
{
	Node_tree* node = (struct Node_tree*)malloc(sizeof(struct Node_tree));
	node->key.Name = Name;
	node->key.Role = Role;
	node->key.Category = Category;
	node->key.Type = Type;
	node->key.Subtype = Subtype;
	node->key.Size = Size;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return(node);
}

struct Node_tree *rightRotate(struct Node_tree *y)
{
	struct Node_tree *x = y->left;
	struct Node_tree *T2 = x->right;

	x->right = y;
	y->left = T2;
	y->height = Max(height(y->left), height(y->right)) + 1;
	x->height = Max(height(x->left), height(x->right)) + 1;

	return x;
}

struct Node_tree *leftRotate(struct Node_tree *x)
{
	struct Node_tree *y = x->right;
	struct Node_tree *T2 = y->left;

	y->left = x;
	x->right = T2;
	x->height = Max(height(x->left), height(x->right)) + 1;
	y->height = Max(height(y->left), height(y->right)) + 1;

	return y;
}

int getBalance(struct Node_tree *N)
{
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}

struct Node_tree *insert(struct Node_tree *node, char *key, eRoles Role, eCategories Category, char *Type, char *Subtype, int Size)
{
	int balance;

	if (node == NULL)
		return(newNode(key, Role, Category, Type, Subtype, Size));

	if (strcmp(key, node->key.Name) < 0)
		node->left = insert(node->left, key, Role, Category, Type, Subtype, Size);
	else if (strcmp(key, node->key.Name) > 0)
		node->right = insert(node->right, key, Role, Category, Type, Subtype, Size);
	else 
		return node;

	node->height = 1 + Max(height(node->left), height(node->right));
	balance = getBalance(node);

	if (balance > 1 && getBalance(node->left) >= 0)
		return rightRotate(node);

	if (balance > 1 && getBalance(node->left) < 0)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	if (balance < -1 && getBalance(node->right) <= 0)
		return leftRotate(node);

	if (balance < -1 && getBalance(node->right) > 0)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

struct Node_tree *minValueNode(struct Node_tree *node)
{
	struct Node_tree* current = node;

	while (current->left != NULL)
		current = current->left;

	return current;
}

struct Node_tree *deleteNode(struct Node_tree *root, char *key)
{
	int balance;

	if (root == NULL)
		return root;

	if (strcmp(key, root->key.Name)<0)
		root->left = deleteNode(root->left, key);
	else if (strcmp(key, root->key.Name)>0)
		root->right = deleteNode(root->right, key);
	else
	{
		if ((root->left == NULL) || (root->right == NULL))
		{
			struct Node_tree *temp = root->left ? root->left :
				root->right;

			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else 
				*root = *temp;

			free(temp);
		}
		else
		{
			struct Node_tree* temp = minValueNode(root->right);
			root->key = temp->key;
			root->right = deleteNode(root->right, temp->key.Name);
		}
	}

	if (root == NULL)
		return root;

	root->height = 1 + Max(height(root->left), height(root->right));
	balance = getBalance(root);

	if (balance > 1 && getBalance(root->left) >= 0)
		return rightRotate(root);

	if (balance > 1 && getBalance(root->left) < 0)
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	if (balance < -1 && getBalance(root->right) <= 0)
		return leftRotate(root);

	if (balance < -1 && getBalance(root->right) > 0)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}

void DeleteTree(struct Node_tree *root)
{
	if (root != NULL)
	{
		DeleteTree(root->left);
		DeleteTree(root->right);
		free(root);
	}
}

Node_tree *search(Node_tree *root, char *key)
{
	if (root == NULL || strcmp(root->key.Name, key) == 0)
		return root;

	if (strcmp(key, root->key.Name)>0)
		return search(root->right, key);
	else
		return search(root->left, key);
}

void PrintTable(struct Node_tree *root)
{
	if (root != NULL)
	{
		PrintTable(root->left);
		printf("Name: %s, Role: %s, Type: %s, Category: %s, Subtype: %s, Size: %d\n",
			root->key.Name, enum_to_Role(root->key.Role), root->key.Type,
			enum_to_Category(root->key.Category), root->key.Subtype, root->key.Size);
		PrintTable(root->right);
	}
}

char *enum_to_Category(eCategories Category)
{
	switch (Category)
	{
	case None:
		return "None";
	case Basic:
		return "Basic";
	case Array:
		return "Array";
	case Pointer:
		return "Pointer";
	}
}

char *enum_to_Role(eRoles Role)
{
	switch (Role)
	{
	case Variable:
		return "Variable";
	case UserDefinedType:
		return "UserDefinedType";
	}
}