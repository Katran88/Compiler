#include "lexAnaliz.h"

bool tokenAnaliz(const char* token, const int strNumber, LT::LexTable& lexTable, IT::IdTable& idTable, const KeyTokens* keyTokens) 
{
	static bool paramFlag = false;					//для параметров
	static TypeFlag typeFlag;						//для определения типа данных
	static ParrentBlock* tempBlock;
	static std::stack<ParrentBlock> blocksStack;	//стек блоков программы

	static int IFcounter; //счетчик блоков if
	
	if (blocksStack.size() == 0)
	{
		tempBlock = new ParrentBlock;
		blocksStack.push(*tempBlock);
	}
	
	for (int i = 0; i < keyTokens_size; i++)
		if (strcmp(keyTokens[i].keyToken, token) == 0)
		{
			if (i == 0)
				typeFlag = TypeFlag::integer;

			if (i == 1)
				typeFlag = TypeFlag::string;

			if (i == 2)
				typeFlag = TypeFlag::ubyte;

			if (i == 3)
				typeFlag = TypeFlag::logic;

			if (keyTokens[i].lex == LEX_MAIN)
			{
				if (idTable.IsId(token) == -1)
				{
					tempBlock = new ParrentBlock(token);
					blocksStack.push(*tempBlock);
				}
				else
					throw ERROR_THROW_IN(119, strNumber, -1);
			}

			if (keyTokens[i].lex == LEX_IF)
			{				
				char temp[2]{};
				itoa(++IFcounter, temp, 10);
				tempBlock = new ParrentBlock(keyTokens[i].keyToken);
				strcat(tempBlock->name, temp);
				blocksStack.push(*tempBlock);

				idTable.Add({ blocksStack.top().name, tempBlock->name, IT::IDDATATYPE::DEF, IT::IDTYPE::C });
				lexTable.Add({ keyTokens[i].lex, strNumber, idTable.current_size - 1 });
				return true;
			}

			//начало записи параметров
			if (keyTokens[i].lex == LEX_LEFTHESIS && idTable.table[lexTable.table[lexTable.current_size - 1].idxTI].idtype == IT::IDTYPE::F)
				paramFlag = true;

			//конец записи параметров
			if (keyTokens[i].lex == LEX_RIGHTHESIS) 
				paramFlag = false;

			// конец области видимости последнего блока в стеке
			if (keyTokens[i].lex == LEX_RIGHTBRACE) 
				blocksStack.pop();

			lexTable.Add({ keyTokens[i].lex, strNumber, LT_TI_NULLXDX });
			return true;
		}

	FST::FST *identificator = new FST::FST(A_IDENTIFICATOR(token));
	if (FST::execute(*identificator))
	{
		bool alreadyChecked = false;
		//для функций
		if (lexTable.table[lexTable.current_size - 1].lexema == LEX_FUNCTION)
		{
			if (idTable.IsId(token) == -1)
			{
				if (typeFlag == TypeFlag::integer)
					idTable.Add({ blocksStack._Get_container()[0].name, token, IT::IDDATATYPE::INT, IT::IDTYPE::F });

				if (typeFlag == TypeFlag::string)
					idTable.Add({ blocksStack._Get_container()[0].name, token, IT::IDDATATYPE::STR, IT::IDTYPE::F });

				if (typeFlag == TypeFlag::ubyte)
					idTable.Add({ blocksStack._Get_container()[0].name, token, IT::IDDATATYPE::UBYTE, IT::IDTYPE::F });

				if (typeFlag == TypeFlag::logic)
					idTable.Add({ blocksStack._Get_container()[0].name, token, IT::IDDATATYPE::LOGIC, IT::IDTYPE::F });

				lexTable.Add({ LEX_ID, strNumber, idTable.current_size - 1 });
				alreadyChecked = true;

				typeFlag = TypeFlag::def;

				tempBlock = new ParrentBlock(token);

				blocksStack.push(*tempBlock);
			}					
			else
				throw ERROR_THROW_IN(123, strNumber, -1);
		}

		//для переменной/параметра(с проверкой переопределения)
		if (!alreadyChecked && lexTable.table[lexTable.current_size - 1].lexema == LEX_TYPE)
		{
			if (lexTable.table[lexTable.current_size - 2].lexema == LEX_GLOBAL)
			{
				if (idTable.IsId(token) == -1)
				{
					if (typeFlag == TypeFlag::integer)
						idTable.Add({ blocksStack._Get_container()[blocksStack.size() - 1].name, token, IT::IDDATATYPE::INT, IT::IDTYPE::V });

					if (typeFlag == TypeFlag::string)
						idTable.Add({ blocksStack._Get_container()[blocksStack.size() - 1].name, token, IT::IDDATATYPE::STR, IT::IDTYPE::V });
				
					if (typeFlag == TypeFlag::ubyte)
						idTable.Add({ blocksStack._Get_container()[blocksStack.size() - 1].name, token, IT::IDDATATYPE::UBYTE, IT::IDTYPE::V });

					if (typeFlag == TypeFlag::logic)
						idTable.Add({ blocksStack._Get_container()[blocksStack.size() - 1].name, token, IT::IDDATATYPE::LOGIC, IT::IDTYPE::V });
				}
				else
					throw ERROR_THROW_IN(123, strNumber, -1);
			}
			else
			{
				if (searchingForIDinStack(idTable, blocksStack, token) == -1)
				{
					if (paramFlag)
					{
						if (typeFlag == TypeFlag::integer)
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::INT, IT::IDTYPE::P });

						if (typeFlag == TypeFlag::string)
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::STR, IT::IDTYPE::P });

						if (typeFlag == TypeFlag::ubyte)
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::UBYTE, IT::IDTYPE::P });

						if (typeFlag == TypeFlag::logic)
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::LOGIC, IT::IDTYPE::P });
					}
					else
					{
						if (typeFlag == TypeFlag::integer)
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::INT, IT::IDTYPE::V });

						if (typeFlag == TypeFlag::string)
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::STR, IT::IDTYPE::V });

						if (typeFlag == TypeFlag::ubyte)
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::UBYTE, IT::IDTYPE::V });

						if (typeFlag == TypeFlag::logic)
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::LOGIC, IT::IDTYPE::V });
					}
				}
				else
					throw ERROR_THROW_IN(123, strNumber, -1);
			}
			
			lexTable.Add({ LEX_ID, strNumber, idTable.current_size - 1 });
			typeFlag = TypeFlag::def;
			alreadyChecked = true;
		}
		
		//добавление идентификаторов
		if (!alreadyChecked)
		{
			int tempIndex = idTable.IsId(token);
			if (tempIndex != -1)
				lexTable.Add({ LEX_ID, strNumber, tempIndex });
			else
				throw ERROR_THROW_IN(129, strNumber, -1);
					
		}
			
		delete identificator;
		identificator = NULL;
		return true;			
	}
	else
	{	
		delete identificator;
		identificator = NULL;

		FST::FST* integer_literal = new FST::FST(INTEGER_LITERAL(token));
		if (FST::execute(*integer_literal))
		{
			int i = idTable.IsLit(token);
			if(i != LT_TI_NULLXDX)
				lexTable.Add({ LEX_LITERAL, strNumber, i });
			else
			{
				idTable.Add({ '\0', '\0', IT::IDDATATYPE::INT,  IT::IDTYPE::L });
				idTable.table[idTable.current_size - 1].value.vint = atoi(token);
				lexTable.Add({ LEX_LITERAL, strNumber, idTable.current_size - 1 });
			}

			delete integer_literal;
			integer_literal = NULL;
			return true;
		}
		else
		{
			delete integer_literal;
			integer_literal = NULL;

			FST::FST* string_literal = new FST::FST(STRING_LITERAL(token));
			if (FST::execute(*string_literal))
			{
				int i = idTable.IsLit(token);
				if (i != LT_TI_NULLXDX)
					lexTable.Add({ LEX_LITERAL, strNumber, i });
				else
				{
					idTable.Add({ '\0', '\0', IT::IDDATATYPE::STR,  IT::IDTYPE::L });

					idTable.table[idTable.current_size - 1].value.vstr.len = 0;
					int i = 0, j = 0;
					for (; token[i] != '\0'; i++)
					{
						if (token[i] != '\"')
						{
							idTable.table[idTable.current_size - 1].value.vstr.str[j] = token[i];
							idTable.table[idTable.current_size - 1].value.vstr.len++;
							j++;
						}
					}
					idTable.table[idTable.current_size - 1].value.vstr.str[j] = '\0';

					lexTable.Add({ LEX_LITERAL, strNumber, idTable.current_size - 1 });
				}

				delete string_literal;
				string_literal = NULL;
				return true;
			}

			delete string_literal;
			string_literal = NULL;
		}
	}

	return false;
}

void lexAnaliz(const In::IN& source, LT::LexTable& lexTable, IT::IdTable& idTable)
{
	const KeyTokens keyTokens[keyTokens_size]
	{
		{"int",			LEX_TYPE},
		{"str",			LEX_TYPE},
		{"ubyte",		LEX_TYPE},
		{"logic",		LEX_TYPE},
		{"global",		LEX_GLOBAL},
		{"if",			LEX_IF},
		{"main",		LEX_MAIN},
		{"func",		LEX_FUNCTION},
		{"return",		LEX_RETURN},
		{"cprint",		LEX_PRINT},
		{";",			LEX_SEMICOLON},
		{",",			LEX_COMMA},
		{"{",			LEX_LEFTBRACE},
		{"}",			LEX_RIGHTBRACE},
		{"(",			LEX_LEFTHESIS},
		{")",			LEX_RIGHTHESIS},
		{"+",			LEX_PLUS},
		{"-",			LEX_MINUS},
		{"*",			LEX_STAR},
		{"/",			LEX_DIRSLASH},
		{"=",			LEX_EQUAL_SIGN},
		{"<",			LEX_LESS_SIGN},
		{">",			LEX_MORE_SIGN},
		{"==",			LEX_SAME_SIGN},
		{"!=",			LEX_DIFFERENT_SIGN},
		{"<=",			LEX_LESS_SAME_SIGN},
		{">=",			LEX_MORE_SAME_SIGN},
		{"or",			LEX_OR_SIGN},
		{"and",			LEX_AND_SIGN}

	};

	char* temp = new char[50]{};
	int strNum = 0;
	int posInStr = 0;

	for (int i = 0, j = 0; i < source.size; i++)
	{
		if ((source.text[i] >= 'A' && source.text[i] <= 'Z') ||
			(source.text[i] >= 'a' && source.text[i] <= 'z') ||
			(source.text[i] >= '0' && source.text[i] <= '9'))
		{
			temp[j++] = source.text[i];
			posInStr++;
			continue;
		}
		else
		{
			if (j != 0)
			{
				i--;
				temp[j] = '\0';
				if (tokenAnaliz(temp, strNum, lexTable, idTable, keyTokens))
				{
					temp[0] = '\0'; j = 0;
					continue;
				}
				else
					throw ERROR_THROW_IN(121, strNum, posInStr);
			}
			else
			{
				if (source.text[i] == '\"')
				{
					temp[j++] = source.text[i++];
					for (int c = 0; source.text[i] != '\"'; c++)
					{
						if (c <= TI_STR_MAXSIZE)
						{
							temp[j++] = source.text[i++];
						}
						else
							throw ERROR_THROW_IN(126, strNum, posInStr);
					}
					if (source.text[i] == '\"')
					{
						temp[j] = source.text[i];
						if (tokenAnaliz(temp, strNum, lexTable, idTable, keyTokens))
						{
							temp[0] = '\0'; j = 0;
							continue;
						}
						else
							throw ERROR_THROW_IN(121, strNum, posInStr);
					}
					else
						throw ERROR_THROW_IN(127, strNum, posInStr);
				}

				if (source.text[i] != '\n')
				{
					if (source.text[i] == ' ' || source.text[i] == '\t')
					{
						posInStr++;
						continue;
					}

					temp[0] = source.text[i];
					
					if (source.text[i + 1] == '=')
					{
						temp[1] = source.text[i++];
						temp[2] = '/0';
					}
					else
						temp[1] = '\0';

					if (tokenAnaliz(temp, strNum, lexTable, idTable, keyTokens))
						posInStr++;
					else
						throw ERROR_THROW_IN(121, strNum, posInStr);

					temp[0] = '/0'; j = 0;
				}
			}
		}

		if (source.text[i] == '\n')
		{
			strNum++;
			posInStr = 0;
		}

		
	}

	delete[] temp;
}

int searchingForIDinStack(IT::IdTable& idTable, std::stack<ParrentBlock>& stack,const char* token)
{
	int result = -1;
	for (int i = stack.size() - 1; i >= 0 && result == -1; i--)
		result = idTable.IsId(token, stack._Get_container()[i].name);

	return result;
}