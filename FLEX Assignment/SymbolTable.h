#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum eRoles
{
	Variable,
	UserDefinedType
}eRoles;

typedef enum eCategories
{
	None,
	Basic,
	Array,
	Pointer
}eCategories;

typedef struct Data
{
	char *Name;
	eRoles Role;
	eCategories Category;
	char *Type;
	char *Subtype;
	int Size;
}Data;

typedef struct Node_tree
{
	Data key;
	struct Node_tree *left;
	struct Node_tree *right;
	int height;
}Node_tree;

int height(struct Node_tree *N);
struct Node_tree *insert(struct Node_tree *node, char *key, eRoles Role, eCategories Category, char *Type, char *Subtype, int Size);
struct Node_tree *deleteNode(struct Node_tree *root, char *key);
struct Node_tree *search(Node_tree *root, char *key);
void DeleteTree(struct Node_tree *root);
void PrintTable(struct Node_tree *root);
char *enum_to_Role(eRoles Role);
char *enum_to_Category(eCategories Category);

#endif