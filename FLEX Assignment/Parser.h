#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "LinkedList.h"

LinkedList SymbolTable;

typedef enum eGrammarVariables
{
	VarProgramm,
	VarBlock,
	VarDefinitions,
	VarDefinitionsExt,
	VarDefinition,
	//VarVAR_DEFINITION,
	//VarVAR_DEFINITION_Tag,
	VarTypeDefinition,
	VarTypeIndicator,
	VarBasicType,
	VarArrayType,
	VarPointerType,
	VarPointerTypeExt,
	VarSize,
	VarCommands,
	VarCommandsExt,
	VarCommand,
	VarCommandExt,
	VarReceiver,
	VarReceiverExt,
	VarExpression,
	VarExpressionExt
}eGrammarVariables;

Token* errorMessageAndRecovery(char* enumOfToken, Token *currToken);
void creationOfSemanticOutputFile();
void match(eTOKENS enumOfToken);
void parser();
void parseProgram();
void parseBlock();
void parseDefinitions();
void parseDefinitionsExt();
void parseDefinition();
void parseVarDefinition();
void parseVarDefinitionExt(char**);
void parseTypeDefinition();
void parseTypeIndicator(eCategories*, char**, int*);
void parseBasicType(char**);
void parseArrayType(char**, int*);
void parsePointerType(char**);
void parsePointerTypeExt(char**);
void parseSize(int*);
void parseCommands();
void parseCommandsExt();
void parseCommand();
void parseCommandExt(char**);
void parseExpression(char**);
void parseExpressionExt(char**);
void parseReceiver();
void parseReceiverExt(char**);
int CompareTypes(char*, char*, int);
int syntacticCheck(eTOKENS, char*);
int basicTypeCheck(char*);

#endif