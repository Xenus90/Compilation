#include "Token.h"

#define TOKEN_ARRAY_SIZE 100

int flag = 0; //Determines the file number for output.
int currentIndex = 0;
Node *currentNode = NULL;
Node *head = NULL;

void create_and_store_token(eTOKENS kind, char *lexeme, int numOfLine)
{ 
	if (currentNode == NULL)
	{
		head = currentNode = (Node*)malloc(sizeof(Node));
	
		if (currentNode == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory!\n"); 
			exit(0);
		}

		currentNode->tokensArray = (Token*)calloc(TOKEN_ARRAY_SIZE, sizeof(Token));

		if (currentNode->tokensArray == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory!\n"); 
			exit(0);
		}

		currentNode->prev = NULL;
		currentNode->next = NULL;
	}
	else
	{
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if (currentNode == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory!\n"); 
				exit(0);
			}

			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)calloc(TOKEN_ARRAY_SIZE, sizeof(Token));

			if (currentNode->tokensArray == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory!\n"); 
				exit(0);
			}

			currentNode->next = NULL;
		}
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;
	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char)*(strlen(lexeme)+1));
	strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;
}

void preparationForParser() //Going to the start of list of the tokens.
{
	while (currentNode->prev != NULL)
	{
		currentNode = currentNode->prev;
	}

	currentIndex = -1;

	if (flag == 0)
	{
		fOut = fopen("c:\\temp\\test1_203771621_321924466_syntactic.txt", "w");
		flag = 1;
	}
	else
	{
		fOut = fopen("c:\\temp\\test2_203771621_321924466_syntactic.txt", "w");
	}
}

void free_memory()
{
	fprintf(yyout, "\n\n*** Starting freeing memory process! ***");

	Node *tmp = head;
	Node *prev;

	int index;

	while (tmp)
	{
		for (index = 0; index <= ((tmp == currentNode) ? currentIndex : TOKEN_ARRAY_SIZE - 1); ++index)
			free(tmp->tokensArray[index].lexeme);

		free(tmp->tokensArray);
		prev = tmp;
		tmp = tmp->next;
		free(prev);
	}

	currentNode = NULL;
	currentIndex = 0;
	fprintf(yyout, "\n*** Successfully freed all nodes! ***");
}

Token *next_token()
{
	if ((currentIndex + 1) < TOKEN_ARRAY_SIZE && currentNode->tokensArray[currentIndex + 1].kind != 0) //If next token in the same node & it's not an empty cell.
	{
		currentIndex++;
		return &currentNode->tokensArray[currentIndex];
	}
	else if ((currentIndex + 1) >= TOKEN_ARRAY_SIZE && currentNode->next != NULL) //If next token in the next node.
	{
		currentIndex = 0;
		currentNode = currentNode->next;
		return &currentNode->tokensArray[currentIndex];
	}
}

Token *back_token()
{
	if (currentIndex > 0) //If previous token in the same node.
	{
		currentIndex--;
		return &currentNode->tokensArray[currentIndex];
	}
	else if (currentIndex == 0 && currentNode->prev != NULL) //If previous token in different (previous) node.
	{
		currentIndex = 99;
		currentNode = currentNode->prev;
		return &currentNode->tokensArray[currentIndex];
	}
	else
	{
		fprintf(yyout, "\nNo previous node (so is token) exists!\n");
		exit(0);
	}
}