#ifndef TOKEN_H
#define TOKEN_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

extern FILE *yyin, *yyout;
FILE *fOut;

typedef enum eTOKENS
{
	KEYWORD_BEGIN = 1,
	KEYWORD_END,
	KEYWORD_BLOCK,
	KEYWORD_TYPE,
	KEYWORD_IS,
	KEYWORD_OF,
	KEYWORD_DO,
	KEYWORD_DEFAULT,
	KEYWORD_WHEN,
	KEYWORD_END_WHEN,
	KEYWORD_FOR,
	KEYWORD_END_FOR,
	KEYWORD_MALLOC,
	KEYWORD_SIZE_OF,
	KEYWORD_ARRAY,
	KEYWORD_FREE,
	KEYWORD_UNSIGNED_INTEGER,
	KEYWORD_REAL,
	TOKEN_ID,
	TOKEN_UNSIGNED_INTEGER,
	TOKEN_REAL,
	TOKEN_PLUS_OPERATION,				
	TOKEN_MINUS_OPERATION,				
	TOKEN_MULTIPLICATION_OPERATION,	
	TOKEN_DIVISION_OPERATION,
	TOKEN_POWER_OPERATION,
	TOKEN_INCREMENT_OPERATION,	
	TOKEN_GREATER_THAN,				
	TOKEN_LESSER_THAN,				
	TOKEN_GREATER_EQUALS_THAN,	
	TOKEN_LESSER_EQUALS_THAN,		
	TOKEN_EQUALS,					
	TOKEN_NOT_EQUALS,				
	TOKEN_ASSIGNMENT_SIGN,	
	TOKEN_POINTER,
	TOKEN_ADDRESS_OF,
	TOKEN_COMMA,						
	TOKEN_SEMICOLON,					
	TOKEN_COLON,						
	TOKEN_PARENTHESES_OPEN,	
	TOKEN_PARENTHESES_CLOSE,	
	TOKEN_BRACKETS_OPEN,		
	TOKEN_BRACKETS_CLOSE,	
	TOKEN_EOF
}eTOKENS;

typedef struct Token
{
	eTOKENS kind;
	char *lexeme;
	int lineNumber;
}Token;

typedef struct Node
{
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;
}Node;

void create_and_store_token(eTOKENS kind, char *lexeme, int numOfLine);
void preparationForParser();
void free_memory();
Token *next_token();
Token *back_token(); 

#endif