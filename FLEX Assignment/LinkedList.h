#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>
#include <stdio.h>
#include "SymbolTable.h"

typedef struct node 
{ 
	Node_tree *data;
	struct node *next;
}node;

typedef struct LinkedList 
{
	node* Start;
	int Size;
}LinkedList;

LinkedList CreateList();
Data *SearchIn_List(LinkedList *L, char *key);
Data *Check_In_Top_node(LinkedList *L, char *key);
void AddEntryTopTable(LinkedList *List, char *Name, eRoles Role, eCategories Category, char *Type, char *Subtype, int Size);
void Push(LinkedList *List);
void Pop(LinkedList *L);
void PrintList(LinkedList *L);

#endif