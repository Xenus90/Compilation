#include "LinkedList.h"

node *CreateNode()
{
	node *item = (node*)malloc(sizeof(node));
	item->data = NULL;
	item->next = NULL;

	return item;
}

Data *SearchIn_List(LinkedList *L, char *key)
{
	node *temp;
	Node_tree* Tree_Node;
	temp = L->Start;

	while (temp != NULL)
	{
		Tree_Node = search(temp->data, key);
		if (Tree_Node != NULL)
			return &(Tree_Node->key);
		temp = temp->next;
	}
	return NULL;
}

Data *Check_In_Top_node(LinkedList *L, char *key)
{
	if (L->Start != NULL)
	{
		return &(search(L->Start->data, key)->key);
	}
	return NULL;
}

void AddEntryTopTable(LinkedList *List, char *Name, eRoles Role, eCategories Category, char *Type, char *Subtype, int Size)
{
	List->Start->data = insert(List->Start->data, Name, Role, Category, Type, Subtype, Size);
}

void Push(LinkedList *List)
{
	node *item = CreateNode();

	item->next = List->Start;
	List->Start = item;
	List->Size++;
}

void Pop(LinkedList *L)
{
	node *Temp = L->Start;
	if (Temp != NULL)
	{
		L->Start = L->Start->next;
		DeleteTree(Temp->data);

		L->Size--;
	}
}

LinkedList CreateList()
{
	LinkedList L;
	L.Size = 0;
	L.Start = NULL;

	return L;
}

void PrintList(LinkedList *L)
{
	int table_number = 1;
	while (L->Start != NULL)
	{
		printf("Table Number: %d\n", table_number++);
		PrintTable(L->Start->data);
		L->Start = L->Start->next;
		printf("\n");
	}
}