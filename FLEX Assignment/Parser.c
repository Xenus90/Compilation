#include "Parser.h"
#include "Token.h"
#include "SymbolTable.h" 
#include "LinkedList.h"

FILE *SemanticOutput = NULL;
int numOfFile = 1;

const char* listOfTokens[] = {
	"KEYWORD_BEGIN",
	"KEYWORD_END",
	"KEYWORD_BLOCK",
	"KEYWORD_TYPE",
	"KEYWORD_IS",
	"KEYWORD_OF",
	"KEYWORD_DO",
	"KEYWORD_DEFAULT",
	"KEYWORD_WHEN",
	"KEYWORD_END_WHEN",
	"KEYWORD_FOR",
	"KEYWORD_END_FOR",
	"KEYWORD_MALLOC",
	"KEYWORD_SIZE_OF",
	"KEYWORD_ARRAY",
	"KEYWORD_FREE",
	"KEYWORD_UNSIGNED_INTEGER",
	"KEYWORD_REAL",
	"TOKEN_ID",
	"TOKEN_UNSIGNED_INTEGER",
	"TOKEN_REAL",
	"TOKEN_PLUS_OPERATION",
	"TOKEN_MINUS_OPERATION",
	"TOKEN_MULTIPLICATION_OPERATION",
	"TOKEN_DIVISION_OPERATION",
	"TOKEN_POWER_OPERATION",
	"TOKEN_INCREMENT_OPERATION",
	"TOKEN_GREATER_THAN",
	"TOKEN_LESSER_THAN",
	"TOKEN_GREATER_EQUALS_THAN",
	"TOKEN_LESSER_EQUALS_THAN",
	"TOKEN_EQUALS",
	"TOKEN_NOT_EQUALS",
	"TOKEN_ASSIGNMENT_SIGN",
	"TOKEN_POINTER",
	"TOKEN_ADDRESS_OF",
	"TOKEN_COMMA",
	"TOKEN_SEMICOLON",
	"TOKEN_COLON",
	"TOKEN_PARENTHESES_OPEN",
	"TOKEN_PARENTHESES_CLOSE",
	"TOKEN_BRACKETS_OPEN",
	"TOKEN_BRACKETS_CLOSE",
	"TOKEN_EOF" };

Token* errorMessageAndRecovery(char *expectedListOfTokens, Token *currToken)
{
	fprintf(yyout, "\nExpected token of type '{%s}' at line : {%d} \nActual token of type '{%s}', lexeme: '{%s}'",
		expectedListOfTokens, currToken->lineNumber, listOfTokens[currToken->kind - 1], currToken->lexeme);

	while (currToken->kind != TOKEN_SEMICOLON && currToken->kind != TOKEN_EOF)
	{
		currToken = next_token();
	}

	if (currToken->kind == TOKEN_EOF)
	{
		fprintf(yyout, "\nTOKEN_EOF reached!");
		exit(0);
	}

	return currToken;
}

void creationOfSemanticOutputFile()
{
	if (SemanticOutput == NULL)
	{
		SemanticOutput = fopen("c:\\temp\\test1_203771621_321924466_semantic.txt", "w");
		numOfFile = 2;
	}
	else if (SemanticOutput != NULL && numOfFile == 2)
	{
		fclose(SemanticOutput);
		SemanticOutput = fopen("c:\\temp\\test2_203771621_321924466_semantic.txt", "w");
		numOfFile += 1;
	}
	else if (numOfFile == 3)
	{
		fclose(SemanticOutput);
	}
}

void match(eTOKENS enumOfToken)
{
	Token *currToken = next_token();

	if (currToken->kind != enumOfToken)
	{
		fprintf(yyout, "\nExpected token of type '{%s}' at line : {%d} \nActual token of type '{%s}', lexeme: '{%s}'",
			listOfTokens[enumOfToken - 1], currToken->lineNumber, listOfTokens[currToken->kind - 1], currToken->lexeme);
	}
}

void parser()
{
	preparationForParser();
	creationOfSemanticOutputFile();
	SymbolTable = CreateList();
	parseProgram();
	match(TOKEN_EOF);
	
	if (numOfFile == 3)
	{
		creationOfSemanticOutputFile(); // Closing all input files.
	}
}

void parseProgram()
{
	fprintf(yyout, "\n{PROGRAM -> BLOCK}");
	parseBlock();
}

void parseBlock()
{
	char *expectedListOfTokens = "KEYWORD_BLOCK";
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case KEYWORD_BLOCK:
		Push(&SymbolTable);
		AddEntryTopTable(&SymbolTable, "integer", UserDefinedType, Basic, NULL, "integer", -1);
		AddEntryTopTable(&SymbolTable, "real", UserDefinedType, Basic, NULL, "real", -1);
		fprintf(yyout, "\n{BLOCK -> block DEFINITIONS; begin COMMANDS; end}");
		parseDefinitions();
		currToken = next_token();
		currToken = back_token();
		match(TOKEN_SEMICOLON);
		currToken = next_token();
		currToken = back_token();
		match(KEYWORD_BEGIN);
		parseCommands();
		currToken = next_token();
		currToken = back_token();
		match(TOKEN_SEMICOLON);
		currToken = next_token();
		currToken = back_token();
		match(KEYWORD_END);
		Pop(&SymbolTable);
		break;
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		currToken = back_token();
		break;
	}
}

void parseDefinitions()
{
	fprintf(yyout, "\n{DEFINITIONS -> DEFINITION DEFINITIONS_EXT}");
	parseDefinition();
	parseDefinitionsExt();
}

void parseDefinitionsExt()
{
	char *expectedListOfTokens = "TOKEN_ID, KEYWORD_TYPE, TOKEN_SEMICOLON, KEYWORD_BEGIN";
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case TOKEN_SEMICOLON:
		currToken = next_token();

		switch (currToken->kind)
		{
		case TOKEN_ID:
			fprintf(yyout, "\n{DEFINITIONS_EXT -> ; DEFINITION DEFINITIONS_EXT}");
			currToken = back_token();
			parseDefinition();
			parseDefinitionsExt();
			break;
		case KEYWORD_TYPE:
			fprintf(yyout, "\n{DEFINITIONS_EXT -> ; DEFINITION DEFINITIONS_EXT}");
			currToken = back_token();
			parseDefinition();
			parseDefinitionsExt();
			break;
		default: 
			fprintf(yyout, "\n{DEFINITIONS_EXT -> nullable}");
			currToken = back_token();
			currToken = back_token();
			break;
		}
		break;
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

void parseDefinition()
{
	char *expectedListOfTokens = "TOKEN_ID, KEYWORD_TYPE";
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case TOKEN_ID:
		fprintf(yyout, "\n{DEFINITION -> VAR_DEFINITION}");
		currToken = back_token();
		parseVarDefinition();
		break;
	case KEYWORD_TYPE:
		fprintf(yyout, "\n{DEFINITION -> TYPE_DEFINITION}");
		currToken = back_token();
		parseTypeDefinition();
		break;
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

void parseVarDefinition()
{
	char *expectedListOfTokens = "TOKEN_ID";
	char *varType = NULL;
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case TOKEN_ID:
		fprintf(yyout, "\n{VAR_DEFINITION -> id : VAR_DEFINITION_EXT}");
		match(TOKEN_COLON);
		parseVarDefinitionExt(&varType);

		if (varType != NULL)
		{
			if (Check_In_Top_node(&SymbolTable, currToken->lexeme) == NULL)
				AddEntryTopTable(&SymbolTable, currToken->lexeme, Variable, None, varType, NULL, -1);
			else
				fprintf(SemanticOutput, "Error! Line {%d}: Var {%s} already defined!\n", currToken->lineNumber, currToken->lexeme);
		}
		break;
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

void parseVarDefinitionExt(char **VarType)
{
	char *expectedListOfTokens = "KEYWORD_UNSIGNED_INTEGER, KEYWORD_REAL, TOKEN_ID";
	Data *tableEntry;
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case KEYWORD_UNSIGNED_INTEGER:
		fprintf(yyout, "\n{VAR_DEFINITION_EXT -> BASIC_TYPE}");
		currToken = back_token();
		parseBasicType(VarType);
		break;
	case KEYWORD_REAL:
		fprintf(yyout, "\n{VAR_DEFINITION_EXT -> BASIC_TYPE}");
		currToken = back_token();
		parseBasicType(VarType);
		break;
	case TOKEN_ID:
		fprintf(yyout, "\n{VAR_DEFINITION_EXT -> typeName}");
		tableEntry = SearchIn_List(&SymbolTable, currToken->lexeme);
		if (tableEntry != NULL && tableEntry->Role == UserDefinedType)
			(*VarType) = tableEntry->Name;
		else
			fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", currToken->lineNumber, currToken->lexeme);
		break;
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

void parseTypeDefinition()
{
	char *expectedListOfTokens = "KEYWORD_TYPE";
	char *TypeSubType, *TypeName;
	int TypeSize;
	eCategories TypeCategory;
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case KEYWORD_TYPE:
		fprintf(yyout, "\n{TYPE_DEFINITION -> type typeName is TYPE_INDICATOR}");

		if (syntacticCheck(TOKEN_ID, currToken->lexeme) != 0)
		{
			TypeName = currToken->lexeme;

			if (syntacticCheck(KEYWORD_IS, currToken->lexeme) != 0)
			{
				parseTypeIndicator(&TypeCategory, &TypeSubType, &TypeSize);

				if (Check_In_Top_node(&SymbolTable, TypeName) == NULL)
					AddEntryTopTable(&SymbolTable, TypeName, UserDefinedType, TypeCategory, NULL, TypeSubType, TypeSize);
				else
					fprintf(SemanticOutput, "Error! Line {%d}: the string {%s} is already defined!\n", currToken->lineNumber, TypeName);
				break;
			}
		}
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

void parseTypeIndicator(eCategories *TypeCategory, char **TypeSubType, int *TypeSize)
{
	char *expectedListOfTokens = "KEYWORD_UNSIGNED_INTEGER, KEYWORD_REAL, KEYWORD_ARRAY, TOKEN_POINTER";
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case KEYWORD_UNSIGNED_INTEGER:
		fprintf(yyout, "\n{TYPE_INDICATOR -> BASIC_TYPE}");
		currToken = back_token();
		parseBasicType(TypeSubType);
		(*TypeCategory) = Basic;
		(*TypeSize) = -1;
		break;
	case KEYWORD_REAL:
		fprintf(yyout, "\n{TYPE_INDICATOR -> BASIC_TYPE}");
		currToken = back_token();
		parseBasicType(TypeSubType);
		(*TypeCategory) = Basic;
		(*TypeSize) = -1;
		break;
	case KEYWORD_ARRAY:
		fprintf(yyout, "\n{TYPE_INDICATOR -> ARRAY_TYPE}");
		currToken = back_token();
		parseArrayType(TypeSubType, TypeSize);
		(*TypeCategory) = Array;
		break;
	case TOKEN_POINTER:
		fprintf(yyout, "\n{TYPE_INDICATOR -> POINTER_TYPE}");
		currToken = back_token();
		parsePointerType(TypeSubType);
		(*TypeCategory) = Pointer;
		(*TypeSize) = -1;
		break;
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

void parseBasicType(char **basicType)
{
	char *expectedListOfTokens = "KEYWORD_UNSIGNED_INTEGER, KEYWORD_REAL";
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case KEYWORD_UNSIGNED_INTEGER:
		fprintf(yyout, "\n{BASIC_TYPE -> integer}");
		(*basicType) = "integer";
		break;
	case KEYWORD_REAL:
		fprintf(yyout, "\n{BASIC_TYPE -> real}");
		(*basicType) = "real";
		break;
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

void parseArrayType(char **TypeSubType, int *TypeSize)
{
	char *expectedListOfTokens = "KEYWORD_ARRAY";
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case KEYWORD_ARRAY:
		fprintf(yyout, "\n{ARRAY_TYPE -> array [SIZE] of BASIC_TYPE}");
		match(TOKEN_BRACKETS_OPEN);
		parseSize(TypeSize);
		match(TOKEN_BRACKETS_CLOSE);
		match(KEYWORD_OF);
		parseBasicType(TypeSubType);
		break;
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

void parsePointerType(char **TypeSubType)
{
	char *expectedListOfTokens = "TOKEN_POINTER";
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case TOKEN_POINTER:
		fprintf(yyout, "\n{POINTER_TYPE -> ^ VAR_DEFINITION_EXT}");
		//parseVarDefinitionExt(TypeSubType);
		parsePointerTypeExt(TypeSubType);
		break;
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

/*Не моё*/
void parsePointerTypeExt(char **TypeSubType) 
{
	Data *tableEntry;
	Token *currToken = next_token();	

	switch (currToken->kind)
	{
	case (TOKEN_UNSIGNED_INTEGER):
		fprintf(yyout, "{POINTER_TYPE' -> BASIC_TYPE  }\n");
		back_token();
		parseBasicType(TypeSubType);
		break;
	case (TOKEN_REAL):
		fprintf(yyout, "{POINTER_TYPE' -> BASIC_TYPE  }\n");
		back_token();
		parseBasicType(TypeSubType);
		break;
	case (TOKEN_ID):
		fprintf(yyout, "{POINTER_TYPE' -> typeName }\n");
		tableEntry = SearchIn_List(&SymbolTable, currToken->lexeme);

		if (tableEntry != NULL && tableEntry->Role == UserDefinedType)
			(*TypeSubType) = tableEntry->Name;
		else
		{
			(*TypeSubType) = NULL;
			fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", currToken->lineNumber, currToken->lexeme);
		}
		break;
	default:
		break;
	}
}

void parseSize(int *TypeSize)
{
	char *expectedListOfTokens = "TOKEN_UNSIGNED_INTEGER";
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case TOKEN_UNSIGNED_INTEGER:
		fprintf(yyout, "\n{SIZE -> int_num}");
		(*TypeSize) = atoi(currToken->lexeme);
		break;
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

void parseCommands()
{
	fprintf(yyout, "\n{COMMANDS -> COMMAND COMMANDS_EXT}");
	parseCommand();
	parseCommandsExt();
}

void parseCommandsExt()
{
	char *expectedListOfTokens = "TOKEN_SEMICOLON, KEYWORD_DEFAULT, KEYWORD_END_WHEN, KEYWORD_END_FOR, KEYWORD_END";
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case TOKEN_SEMICOLON:
		currToken = next_token();

		switch (currToken->kind)
		{
		case TOKEN_ID:
			fprintf(yyout, "\n{COMMANDS_EXT -> ; COMMAND COMMANDS_EXT}");
			currToken = back_token();
			parseCommand();
			parseCommandsExt();
			break;
		case KEYWORD_WHEN:
			fprintf(yyout, "\n{COMMANDS_EXT -> ; COMMAND COMMANDS_EXT}");
			currToken = back_token();
			parseCommand();
			parseCommandsExt();
			break;
		case KEYWORD_FOR:
			fprintf(yyout, "\n{COMMANDS_EXT -> ; COMMAND COMMANDS_EXT}");
			currToken = back_token();
			parseCommand();
			parseCommandsExt();
			break;
		case KEYWORD_FREE:
			fprintf(yyout, "\n{COMMANDS_EXT -> ; COMMAND COMMANDS_EXT}");
			currToken = back_token();
			parseCommand();
			parseCommandsExt();
			break;
		case KEYWORD_BLOCK:
			fprintf(yyout, "\n{COMMANDS_EXT -> ; COMMAND COMMANDS_EXT}");
			currToken = back_token();
			parseCommand();
			parseCommandsExt();
			break;
		default:
			fprintf(yyout, "\n{DEFINITIONS_EXT -> nullable}");
			currToken = back_token();
			currToken = back_token();
			break;
		}
		break;
	/*case KEYWORD_DEFAULT:
		fprintf(yyout, "\n{COMMANDS_EXT -> nullable}");
		currToken = back_token();
		currToken = back_token();
		break;
	case KEYWORD_END_WHEN:
		fprintf(yyout, "\n{COMMANDS_EXT -> nullable}");
		currToken = back_token();
		currToken = back_token();
		break;
	case KEYWORD_END_FOR:
		fprintf(yyout, "\n{COMMANDS_EXT -> nullable}");
		currToken = back_token();
		currToken = back_token();
		break;
	case KEYWORD_END:
		fprintf(yyout, "\n{COMMANDS_EXT -> nullable}");
		currToken = back_token();
		currToken = back_token();
		break;*/
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

void parseCommand()
{
	char *commandExtType, *typeName;
	char *constInt = "integer";
	Data *tableEntry, *tableEntry2;	
	Token *currToken = next_token(), *currToken2, *varName;

	switch (currToken->kind)
	{
	case (TOKEN_ID):
		fprintf(yyout, "{COMMAND -> id COMMAND' }\n");
		tableEntry = SearchIn_List(&SymbolTable, currToken->lexeme);

		if (tableEntry != NULL)
		{
			if (tableEntry->Role == Variable)
				commandExtType = tableEntry->Type;
			else
			{
				fprintf(SemanticOutput, "Error! Line {%d}: the {%s} has to be var!\n", currToken->lineNumber, currToken->lexeme);
				commandExtType = "error_type";
			}
		}
		else
		{
			fprintf(SemanticOutput, "Error! Line {%d}: var {%s} not defined!\n", currToken->lineNumber, currToken->lexeme);
			commandExtType = "error_type";
		}

		parseCommandExt(&commandExtType);
		break;
	case (KEYWORD_WHEN):
		fprintf(yyout, "{COMMAND -> when (EXPRESSION rel_op EXPRESSION) do COMMANDS; default COMMANDS; end_when }\n");
		match(TOKEN_PARENTHESES_OPEN);
		parseExpression(&constInt);
		currToken2 = next_token();

		switch (currToken2->kind)
		{
		case(TOKEN_EQUALS):
		case(TOKEN_LESSER_THAN):
		case(TOKEN_GREATER_THAN):
		case(TOKEN_LESSER_EQUALS_THAN):
		case(TOKEN_GREATER_EQUALS_THAN):
		case(TOKEN_NOT_EQUALS):
			parseExpression(&constInt);
			match(TOKEN_PARENTHESES_CLOSE);
			match(KEYWORD_DO);
			parseCommands();
			match(TOKEN_SEMICOLON);
			match(KEYWORD_DEFAULT);
			parseCommands();
			match(TOKEN_SEMICOLON);
			match(KEYWORD_END_WHEN);
			break;
		default:
			break;
		}
		break;
	case (KEYWORD_FOR):
		fprintf(yyout, "{COMMAND -> for (id = EXPRESSION; id rel_op EXPRESSION; id++) COMMANDS; end_for }\n");
		match(TOKEN_PARENTHESES_OPEN);
		match(TOKEN_ID);
		varName = currToken;
		match(TOKEN_ASSIGNMENT_SIGN);
		tableEntry = SearchIn_List(&SymbolTable, varName->lexeme);

		if (tableEntry != NULL)
		{
			if (tableEntry->Role == Variable)
			{
				typeName = tableEntry->Type;
			}
			else
			{
				fprintf(SemanticOutput, "Error! Line {%d}: the {%s} has to be var!\n", currToken->lineNumber, currToken->lexeme);
				typeName = "error_type";
			}
		}
		else
		{
			fprintf(SemanticOutput, "Error! Line {%d}: var {%s} not defined!\n", currToken->lineNumber, currToken->lexeme);
			typeName = "error_type";
		}

		parseExpression(&typeName);
		match(TOKEN_SEMICOLON);
		match(TOKEN_ID);
		varName = currToken;
		currToken2 = next_token();

		switch (currToken2->kind)
		{
		case(TOKEN_EQUALS):
		case(TOKEN_LESSER_THAN):
		case(TOKEN_GREATER_THAN):
		case(TOKEN_LESSER_EQUALS_THAN):
		case(TOKEN_GREATER_EQUALS_THAN):
		case(TOKEN_NOT_EQUALS):
			tableEntry = SearchIn_List(&SymbolTable, varName->lexeme);

			if (tableEntry != NULL)
			{
				if (tableEntry->Role == Variable)
				{
					typeName = tableEntry->Type;
				}
				else
				{
					fprintf(SemanticOutput, "Error! Line {%d}: the {%s} has to be var!\n", currToken->lineNumber, currToken->lexeme);
					typeName = "error_type";
				}
			}
			else
			{
				fprintf(SemanticOutput, "Error! Line {%d}: var {%s} not defined!\n", currToken->lineNumber, currToken->lexeme);
				typeName = "error_type";
			}

			parseExpression(&typeName);
			match(TOKEN_SEMICOLON);
			match(TOKEN_ID);
			varName = currToken;
			tableEntry = SearchIn_List(&SymbolTable, varName->lexeme);

			if (tableEntry != NULL)
			{
				if (tableEntry->Role == Variable)
				{
					if (tableEntry->Type != "integer" && tableEntry->Type != "real")
					{
						tableEntry2 = SearchIn_List(&SymbolTable, tableEntry->Type);
						if (tableEntry2->Subtype != "integer" && tableEntry2->Subtype != "real")
							fprintf(SemanticOutput, "Error! Line {%d}: arithmetic error -> {%s} is not a number!\n", currToken->lineNumber, varName);
					}
				}
				else
					fprintf(SemanticOutput, "Error! Line {%d}: the {%s} has to be var!\n", currToken->lineNumber, currToken->lexeme);
			}
			else
				fprintf(SemanticOutput, "Error! Line {%d}: var {%s} not defined!\n", currToken->lineNumber, currToken->lexeme);

			match(TOKEN_INCREMENT_OPERATION);
			match(TOKEN_PARENTHESES_CLOSE);
			parseCommands();
			match(TOKEN_SEMICOLON);
			match(KEYWORD_END_FOR);
			break;
		default:
			break;
		}
		break;
	case (KEYWORD_FREE):
		fprintf(yyout, "{COMMAND -> free(id) }\n");
		match(TOKEN_PARENTHESES_OPEN);
		match(TOKEN_ID);
		varName = currToken;
		match(TOKEN_PARENTHESES_CLOSE);
		tableEntry = SearchIn_List(&SymbolTable, varName->lexeme);

		if (tableEntry != NULL)
		{
			if (tableEntry->Category != Pointer)
				fprintf(SemanticOutput, "Error! Line {%d}: {%s} has to be of pointer type!\n", varName->lineNumber, varName->lexeme);
		}
		else
			fprintf(SemanticOutput, "Error! Line {%d}: var {%s} not defined!\n", varName->lineNumber, varName->lexeme);
		break;
	case (KEYWORD_BLOCK):
		fprintf(yyout, "{COMMAND -> BLOCK }\n");
		back_token();
		parseBlock();		
	default:
		break;
	}
}

/*Не моё*/ 
void parseCommandExt(char **commandExtType)
{
	Data *tableEntry, *tableEntry2;
	Token* currToken = next_token(), *currToken2, *typeName;

	switch (currToken->kind)
	{
	case (TOKEN_ASSIGNMENT_SIGN):
		currToken2 = next_token();

		switch (currToken2->kind)
		{
		case(KEYWORD_MALLOC):
			fprintf(yyout, "{COMMAND' -> = malloc(size_of(typeName)) }\n");
			match(TOKEN_PARENTHESES_OPEN);
			match(KEYWORD_SIZE_OF);
			match(TOKEN_PARENTHESES_OPEN);
			match(TOKEN_ID);
			typeName = currToken;
			match(TOKEN_PARENTHESES_CLOSE);
			match(TOKEN_PARENTHESES_CLOSE);
			tableEntry = SearchIn_List(&SymbolTable, typeName->lexeme);
			tableEntry2 = SearchIn_List(&SymbolTable, *commandExtType);

			if (tableEntry != NULL)
			{
				if (tableEntry->Role == UserDefinedType)
				{
					if (tableEntry2 != NULL)
					{
						if (tableEntry2->Category == Pointer)
						{
							if (tableEntry2->Subtype != tableEntry->Subtype)
								fprintf(SemanticOutput, "Error! Line {%d}: va has to be of subtype {%s}!\n", typeName->lineNumber, typeName->lexeme);
						}
						else
							fprintf(SemanticOutput, "Error! Line {%d}: type mismatch -> var has to be pointer!\n", typeName->lineNumber);
					}
				}
				else
				{
					fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", typeName->lineNumber, typeName->lexeme);
				}
			}
			else
			{
				fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", typeName->lineNumber, typeName->lexeme);
			}
			break;

		case(TOKEN_UNSIGNED_INTEGER):
		case(TOKEN_REAL):
		case(TOKEN_ADDRESS_OF):
		case(KEYWORD_SIZE_OF):
		case(TOKEN_ID):
			fprintf(yyout, "{COMMAND' -> RECEIVER' = EXPRESSION  }\n");
			back_token();
			back_token();
			parseReceiverExt(commandExtType);
			match(TOKEN_ASSIGNMENT_SIGN);
			parseExpression(commandExtType);
			break;
		default:
			break;
		}
		break;
	case (TOKEN_BRACKETS_OPEN):
	case (TOKEN_POINTER):
		fprintf(yyout, "{COMMAND' -> RECEIVER' = EXPRESSION  }\n");
		back_token();
		parseReceiverExt(commandExtType);
		match(TOKEN_ASSIGNMENT_SIGN);
		parseExpression(commandExtType);
		break;
	default:
		break;
	}
}

void parseExpression(char **ExpressionExtType)
{
	char *expectedListOfTokens = "TOKEN_UNSIGNED_INTEGER, TOKEN_REAL, TOKEN_ADDRESS_OF, KEYWORD_SIZE_OF, TOKEN_ID";
	char *typeName;
	Data *tableEntry;
	Token *varName;
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case TOKEN_UNSIGNED_INTEGER:
		fprintf(yyout, "\n{EXPRESSION -> int_num}");
		CompareTypes(*ExpressionExtType, "integer", currToken->lineNumber);
		break;
	case TOKEN_REAL:
		fprintf(yyout, "\n{EXPRESSION -> real_num}");
		CompareTypes(*ExpressionExtType, "real", currToken->lineNumber);
		break;
	case TOKEN_ADDRESS_OF:
		fprintf(yyout, "\n{EXPRESSION -> &id}");
		match(TOKEN_ID);
		varName = currToken;
		tableEntry = SearchIn_List(&SymbolTable, varName->lexeme);

		if (tableEntry != NULL)
		{
			if (tableEntry->Role == Variable)
			{
				Data *tableEntry2;
				tableEntry2 = SearchIn_List(&SymbolTable, *ExpressionExtType);

				if (tableEntry2 != NULL)
				{
					if (tableEntry2->Category == Pointer)
					{
						if (tableEntry2->Subtype != tableEntry->Type)
						{
							fprintf(SemanticOutput, "Error! Line {%d}: var has to be of subtype {%s}!\n", varName->lineNumber, tableEntry->Type);
						}
					}
					else
						fprintf(SemanticOutput, "Error! Line {%d}: type mismatch -> var has to be pointer!\n", varName->lineNumber);
				}
				else
					fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", varName->lineNumber, *ExpressionExtType);

			}
			else
				fprintf(SemanticOutput, "Error! Line {%d}: the {%s} has to be var!\n", varName->lineNumber, varName->lexeme);
		}
		else
			fprintf(SemanticOutput, "Error! Line {%d}: var {%s} not defined!\n", varName->lineNumber, varName->lexeme);

		break;
	case KEYWORD_SIZE_OF:
		fprintf(yyout, "\n{EXPRESSION -> size_of(typeName)}");
		match(TOKEN_PARENTHESES_OPEN);
		match(TOKEN_ID);
		varName = currToken;
		match(TOKEN_PARENTHESES_CLOSE);
		tableEntry = SearchIn_List(&SymbolTable, varName->lexeme);

		if (tableEntry != NULL)
		{
			if (tableEntry->Role == UserDefinedType)
			{
				CompareTypes(*ExpressionExtType, "integer", currToken->lineNumber);
			}
			else
			{
				fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", varName->lineNumber, varName->lexeme);
			}
		}
		else
		{
			fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", varName->lineNumber, varName->lexeme);
		}
		break;
	case TOKEN_ID:
		fprintf(yyout, "\n{EXPRESSION -> id EXPRESSION_EXT}");
		tableEntry = SearchIn_List(&SymbolTable, currToken->lexeme);

		if (tableEntry != NULL)
		{
			if (tableEntry->Role == Variable)
			{
				typeName = tableEntry->Type;
			}
			else
			{
				fprintf(SemanticOutput, "Error! Line {%d}: the {%s} has to be var!\n", currToken->lineNumber, currToken->lexeme);
				typeName = "error_type";
			}
		}
		else
		{
			fprintf(SemanticOutput, "Error! Line {%d}: var {%s} not defined!\n", currToken->lineNumber, currToken->lexeme);
			typeName = "error_type";
		}

		parseExpressionExt(&typeName);
		CompareTypes(*ExpressionExtType, typeName, currToken->lineNumber);
		*ExpressionExtType = typeName;
		break;
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

void parseExpressionExt(char **ExpressionExtType)
{
	char *expectedListOfTokens = "TOKEN_BRACKETS ([ | ]), TOKEN_POINTER, TOKEN_arithmetic_OPERATION (+ | - | * | /), TOKEN_POWER_OPERATION, TOKEN_OPERATION (== | != | < | <= | > | >=), TOKEN_PARENTHESES_CLOSE, TOKEN_SEMICOLON";
	char *typeName, *constInt = "integer";
	Data *tableEntry;
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case TOKEN_BRACKETS_OPEN:
		fprintf(yyout, "\n{EXPRESSION_EXT -> [EXPRESSION]}");
		parseExpression(&constInt);
		match(TOKEN_BRACKETS_CLOSE);		
		tableEntry = SearchIn_List(&SymbolTable, *ExpressionExtType);
		
		if (tableEntry != NULL)
		{
			if (tableEntry->Category != Array)
				fprintf(SemanticOutput, "Error! Line {%d}: Type mismatch -> var has to be array!\n", currToken->lineNumber);
			else
			{
				*ExpressionExtType = tableEntry->Subtype;
			}
		}
		else
			fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", currToken->lineNumber, *ExpressionExtType);

		if (constInt != "integer")
			fprintf(SemanticOutput, "Error! Line {%d}: type mismatch -> index must be integer!\n", currToken->lineNumber);
		break;	
	case TOKEN_POINTER:
		fprintf(yyout, "\n{EXPRESSION_EXT -> ^}");
		tableEntry = SearchIn_List(&SymbolTable, *ExpressionExtType);

		if (tableEntry != NULL)
		{
			if (tableEntry->Category != Pointer)
				fprintf(SemanticOutput, "Error! Line {%d}: Type mismatch -> var has to be pointer!\n", currToken->lineNumber);
			else
			{
				*ExpressionExtType = tableEntry->Subtype;
			}
		}
		else
			fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", currToken->lineNumber, *ExpressionExtType);
		break;
	case TOKEN_PLUS_OPERATION:
		fprintf(yyout, "\n{EXPRESSION_EXT -> ar_op EXPRESSION}");
		tableEntry = SearchIn_List(&SymbolTable, *ExpressionExtType);
		if (tableEntry != NULL)
		{
			if (tableEntry->Category == Basic)
			{
				(*ExpressionExtType) = tableEntry->Subtype;
				typeName = (*ExpressionExtType);
			}
			else
			{
				fprintf(SemanticOutput, "Error! Line {%d}: arithmetic error -> var is not a number!\n", currToken->lineNumber);
				typeName = "error_type";
			}
		}
		else
		{
			fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", currToken->lineNumber, *ExpressionExtType);
			typeName = "error_type";
		}
		parseExpression(&typeName);
		break;
	case TOKEN_MINUS_OPERATION:
		fprintf(yyout, "\n{EXPRESSION_EXT -> ar_op EXPRESSION}");
		tableEntry = SearchIn_List(&SymbolTable, *ExpressionExtType);
		if (tableEntry != NULL)
		{
			if (tableEntry->Category == Basic)
			{
				(*ExpressionExtType) = tableEntry->Subtype;
				typeName = (*ExpressionExtType);
			}
			else
			{
				fprintf(SemanticOutput, "Error! Line {%d}: arithmetic error -> var is not a number!\n", currToken->lineNumber);
				typeName = "error_type";
			}
		}
		else
		{
			fprintf(SemanticOutput, "Error! Lline {%d}: the type {%s} is not defined!\n", currToken->lineNumber, *ExpressionExtType);
			typeName = "error_type";
		}
		parseExpression(&typeName);
		break;
		break;
	case TOKEN_MULTIPLICATION_OPERATION:
		fprintf(yyout, "\n{EXPRESSION_EXT -> ar_op EXPRESSION}");
		tableEntry = SearchIn_List(&SymbolTable, *ExpressionExtType);
		if (tableEntry != NULL)
		{
			if (tableEntry->Category == Basic)
			{
				(*ExpressionExtType) = tableEntry->Subtype;
				typeName = (*ExpressionExtType);
			}
			else
			{
				fprintf(SemanticOutput, "Error! Line {%d}: arithmetic error -> var is not a number!\n", currToken->lineNumber);
				typeName = "error_type";
			}
		}
		else
		{
			fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", currToken->lineNumber, *ExpressionExtType);
			typeName = "error_type";
		}
		parseExpression(&typeName);
		break;
		break;
	case TOKEN_DIVISION_OPERATION:
		fprintf(yyout, "\n{EXPRESSION_EXT -> ar_op EXPRESSION}");
		tableEntry = SearchIn_List(&SymbolTable, *ExpressionExtType);
		if (tableEntry != NULL)
		{
			if (tableEntry->Category == Basic)
			{
				(*ExpressionExtType) = tableEntry->Subtype;
				typeName = (*ExpressionExtType);
			}
			else
			{
				fprintf(SemanticOutput, "Error! Line {%d}: arithmetic error -> var is not a number!\n", currToken->lineNumber);
				typeName = "error_type";
			}
		}
		else
		{
			fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", currToken->lineNumber, *ExpressionExtType);
			typeName = "error_type";
		}
		parseExpression(&typeName);
		break;
		break;
	case TOKEN_POWER_OPERATION:
		fprintf(yyout, "\n{EXPRESSION_EXT -> ar_op EXPRESSION}");
		tableEntry = SearchIn_List(&SymbolTable, *ExpressionExtType);
		if (tableEntry != NULL)
		{
			if (tableEntry->Category == Basic)
			{
				(*ExpressionExtType) = tableEntry->Subtype;
				typeName = (*ExpressionExtType);
			}
			else
			{
				fprintf(SemanticOutput, "Error! Line {%d}: arithmetic error -> var is not a number!\n", currToken->lineNumber);
				typeName = "error_type";
			}
		}
		else
		{
			fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", currToken->lineNumber, *ExpressionExtType);
			typeName = "error_type";
		}
		parseExpression(&typeName);
		break;
		break;
	case TOKEN_EQUALS:
		fprintf(yyout, "\n{EXPRESSION_EXT -> nullable}");
		currToken = back_token();
		break;
	case TOKEN_NOT_EQUALS:
		fprintf(yyout, "\n{EXPRESSION_EXT -> nullable}");
		currToken = back_token();
		break;
	case TOKEN_LESSER_THAN:
		fprintf(yyout, "\n{EXPRESSION_EXT -> nullable}");
		currToken = back_token();
		break;
	case TOKEN_LESSER_EQUALS_THAN:
		fprintf(yyout, "\n{EXPRESSION_EXT -> nullable}");
		currToken = back_token();
		break;
	case TOKEN_GREATER_THAN:
		fprintf(yyout, "\n{EXPRESSION_EXT -> nullable}");
		currToken = back_token();
		break;
	case TOKEN_GREATER_EQUALS_THAN:
		fprintf(yyout, "\n{EXPRESSION_EXT -> nullable}");
		currToken = back_token();
		break;
	case TOKEN_BRACKETS_CLOSE:
		fprintf(yyout, "\n{EXPRESSION_EXT -> nullable}");
		currToken = back_token();
		break;
	case TOKEN_PARENTHESES_CLOSE:
		fprintf(yyout, "\n{EXPRESSION_EXT -> nullable}");
		currToken = back_token();
		break;
	case TOKEN_SEMICOLON:
		fprintf(yyout, "\n{EXPRESSION_EXT -> nullable}");
		currToken = back_token();
		break;
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

void parseReceiver()
{
	char *expectedListOfTokens = "TOKEN_ID";
	char **ReceiverExtType = NULL;
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case TOKEN_ID:
		fprintf(yyout, "\n{RECEIVER -> id RECEIVER_EXT}");
		parseReceiverExt(ReceiverExtType);
		break;
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

void parseReceiverExt(char **ReceiverExtType)
{
	char *expectedListOfTokens = "TOKEN_BRACKETS_OPEN, TOKEN_POINTER, TOKEN_ASSIGNMENT_SIGN";
	char *typeName;
	Data *tableEntry;
	Token *currToken = next_token();

	switch (currToken->kind)
	{
	case TOKEN_BRACKETS_OPEN:
		fprintf(yyout, "\n{RECEIVER_EXT -> [EXPRESSION]}");
		tableEntry = SearchIn_List(&SymbolTable, (*ReceiverExtType));
		if (tableEntry != NULL)
		{
			if (tableEntry->Category == Array)
			{
				typeName = "integer";
				(*ReceiverExtType) = tableEntry->Subtype;
			}
			else
			{
				fprintf(SemanticOutput, "Error! Line {%d}: type mismatch -> var has to be an array!\n", currToken->lineNumber);
				typeName = "error_type";
			}
		}
		else
		{
			fprintf(SemanticOutput, "Error! Line {%d}: type mismatch -> var has to be an array!\n", currToken->lineNumber);
			typeName = "error_type";
		}

		parseExpression(&typeName);
		match(TOKEN_BRACKETS_CLOSE);
		break;
	case TOKEN_POINTER:
		fprintf(yyout, "\n{RECEIVER_EXT -> ^}");
		tableEntry = SearchIn_List(&SymbolTable, (*ReceiverExtType));
		if (tableEntry != NULL)
		{
			if (tableEntry->Category != Pointer)
				fprintf(SemanticOutput, "Error! Line {%d}: type mismatch -> var has to be pointer!\n", currToken->lineNumber);
			else
				(*ReceiverExtType) = tableEntry->Subtype;
		}
		else
			fprintf(SemanticOutput, "Error! Line {%d}: type mismatch -> var has to be pointer!\n", currToken->lineNumber);

		break;
	case TOKEN_ASSIGNMENT_SIGN:
		fprintf(yyout, "\n{RECEIVER_EXT -> nullable}");
		currToken = back_token();
		break;
	default:
		currToken = errorMessageAndRecovery(expectedListOfTokens, currToken);
		break;
	}
}

int CompareTypes(char *type1, char *type2, int lineNumber)
{
	Data *T1, *T2;
	char *realType1, *realType2;
	eCategories C1, C2;

	if (basicTypeCheck(type1))
	{
		realType1 = type1;
		C1 = Basic;
	}
	else
	{
		T1 = SearchIn_List(&SymbolTable, type1);

		if (T1 == NULL)
		{
			fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", lineNumber, type1);
			return 0;
		}
		else
		{
			realType1 = T1->Subtype;
			C1 = T1->Category;
		}
	}

	if (basicTypeCheck(type2))
	{
		realType2 = type2;
		C2 = Basic;
	}
	else
	{
		T2 = SearchIn_List(&SymbolTable, type2);

		if (T2 == NULL)
		{
			fprintf(SemanticOutput, "Error! Line {%d}: the type {%s} is not defined!\n", lineNumber, type2);
			return 0;
		}
		else
		{
			realType2 = T2->Subtype;
			C2 = T2->Category;
		}
	}

	if (C1 == Array || C2 == Array)
	{
		fprintf(SemanticOutput, "Error! Line {%d}: array assignment is forbidden!\n", lineNumber);
		return 0;
	}

	if (realType2 == realType1 && C1 == C2)
		return 1;
	else
		fprintf(SemanticOutput, "Error! Line {%d}: type mismatch -> has to be of type {%s}!\n", lineNumber, realType1);
	return 0;
}

int syntacticCheck(eTOKENS enumOfToken, char *lexeme)
{
	Token *currToken = next_token();
	int result = 1;

	if (enumOfToken != currToken->kind)
	{
		result = 0;
	}

	return result;
}

int basicTypeCheck(char *type)
{
	if (type == "integer" || type == "real")
		return 1;
	else
		return 0;
}