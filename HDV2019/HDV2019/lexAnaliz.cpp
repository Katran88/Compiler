#include "lexAnaliz.h"
static bool flagForMain = false;

bool tokenAnaliz(const char* token, const int strNumber, LT::LexTable& lexTable, IT::IdTable& idTable, const KeyTokens* keyTokens) 
{
	static bool paramFlag = false;					//для параметров
	static IT::IDDATATYPE typeFlag;						//для определения типа данных
	static ParrentBlock* tempBlock;
	static std::stack<ParrentBlock> blocksStack;	//стек блоков программы

	static int IFcounter; //счетчик блоков if
	static int LOOPcounter; //счетчик блоков loop
	static bool flagForLoopIterator = false; //флаг для определения итератора цикла
	
	if (blocksStack.size() == 0)
	{
		tempBlock = new ParrentBlock;
		blocksStack.push(*tempBlock);
	}
	
	for (int i = 0; i < keyTokens_size; i++)
		if (strcmp(keyTokens[i].keyToken, token) == 0)
		{
			if (i == 0)
				typeFlag = IT::IDDATATYPE::INT;

			if (i == 1)
				typeFlag = IT::IDDATATYPE::STR;

			if (i == 2)
				typeFlag = IT::IDDATATYPE::UBYTE;

			if (i == 3)
				typeFlag = IT::IDDATATYPE::LOGIC;

			//true
			if (i == 11)
			{
				idTable.Add({ '\0', '\0', IT::IDDATATYPE::LOGIC, IT::IDTYPE::L });
				idTable.table[idTable.current_size - 1].value.vlogic = true;
				lexTable.Add({ keyTokens[i].lex, strNumber, idTable.current_size - 1 });
				return true;
			}

			//false
			if (i == 12)
			{
				idTable.Add({ '\0', '\0', IT::IDDATATYPE::LOGIC, IT::IDTYPE::L });
				idTable.table[idTable.current_size - 1].value.vlogic = false;
				lexTable.Add({ keyTokens[i].lex, strNumber, idTable.current_size - 1 });
				return true;
			}

			if (keyTokens[i].lex == LEX_MAIN)
			{
				if (idTable.IsId(token) == -1)
				{
					tempBlock = new ParrentBlock(token, IT::IDDATATYPE::INT);
					blocksStack.push(*tempBlock);
					flagForMain = true;
				}
				else
					throw ERROR_THROW_IN(119, strNumber, -1);
			}

			if (keyTokens[i].lex == LEX_RETURN)
			{
				for (int i = blocksStack.size()-1; i >= 0; i--)
					if (blocksStack._Get_container()[i].returnType != IT::IDDATATYPE::DEF)
					{
						idTable.Add({ blocksStack._Get_container()[i].name, "r", blocksStack._Get_container()[i].returnType, IT::IDTYPE::S });
						idTable.table[idTable.current_size - 1].paramsCount = idTable.table[idTable.IsId(blocksStack._Get_container()[i].name)].paramsCount;
						break;
					}
				lexTable.Add({ keyTokens[i].lex, strNumber, idTable.current_size - 1 });
				return true;
			}

			if (keyTokens[i].lex == LEX_IF)
			{				
				char temp[2]{};
				itoa(++IFcounter, temp, 10);
				tempBlock = new ParrentBlock(keyTokens[i].keyToken, IT::IDDATATYPE::DEF);
				strcat(tempBlock->name, "_block");
				strcat(tempBlock->name, temp);
				blocksStack.push(*tempBlock);

				idTable.Add({ blocksStack.top().name, tempBlock->name, IT::IDDATATYPE::DEF, IT::IDTYPE::S });
				lexTable.Add({ keyTokens[i].lex, strNumber, idTable.current_size - 1 });
				return true;
			}

			if (keyTokens[i].lex == LEX_LOOP)
			{
				char temp[2]{};
				itoa(++LOOPcounter, temp, 10);
				tempBlock = new ParrentBlock(keyTokens[i].keyToken, IT::IDDATATYPE::DEF);
				strcat(tempBlock->name, temp);
				blocksStack.push(*tempBlock);

				idTable.Add({ blocksStack.top().name, tempBlock->name, IT::IDDATATYPE::DEF, IT::IDTYPE::S });
				lexTable.Add({ keyTokens[i].lex, strNumber, idTable.current_size - 1 });
				flagForLoopIterator = true;
				return true;
			}

			if (keyTokens[i].lex == LEX_LIBRARY)
			{
				IT::IdTable::isLibraryIncluded = true;
				idTable.Add({ blocksStack._Get_container()[0].name, "getDate", IT::IDDATATYPE::STR, IT::IDTYPE::F });
				idTable.Add({ blocksStack._Get_container()[0].name, "getTime", IT::IDDATATYPE::STR, IT::IDTYPE::F });
			}

			//начало записи параметров
			if (keyTokens[i].lex == LEX_LEFTHESIS && idTable.table[lexTable.table[lexTable.current_size - 1].idxTI].idtype == IT::IDTYPE::F)
			{
				paramFlag = true;
				IT::Entry::FuncParams::currentCount = 0;
			}
			//конец записи параметров
			if (keyTokens[i].lex == LEX_RIGHTHESIS) 
				paramFlag = false;

			// конец области видимости последнего блока в стеке
			if (keyTokens[i].lex == LEX_RIGHTBRACE)
			{
				idTable.Add({ blocksStack.top().name, "}", IT::IDDATATYPE::DEF, IT::IDTYPE::S });
				lexTable.Add({ keyTokens[i].lex, strNumber, idTable.current_size-1 });
				blocksStack.pop();
				return true;
			}

			lexTable.Add({ keyTokens[i].lex, strNumber, LT_TI_NULLXDX });
			return true;
		}

	//идентификатор
	FST::FST *identificator = new FST::FST(A_IDENTIFICATOR(token));
	if (FST::execute(*identificator))
	{
		bool alreadyChecked = false;
		//для функций
		if (lexTable.table[lexTable.current_size - 1].lexema == LEX_FUNCTION)
		{
			if (idTable.IsId(token) == -1)
			{
				if (typeFlag == IT::IDDATATYPE::INT)
					idTable.Add({ blocksStack._Get_container()[0].name, token, IT::IDDATATYPE::INT, IT::IDTYPE::F });

				if (typeFlag == IT::IDDATATYPE::STR)
					idTable.Add({ blocksStack._Get_container()[0].name, token, IT::IDDATATYPE::STR, IT::IDTYPE::F });

				if (typeFlag == IT::IDDATATYPE::UBYTE)
					idTable.Add({ blocksStack._Get_container()[0].name, token, IT::IDDATATYPE::UBYTE, IT::IDTYPE::F });

				if (typeFlag == IT::IDDATATYPE::LOGIC)
					idTable.Add({ blocksStack._Get_container()[0].name, token, IT::IDDATATYPE::LOGIC, IT::IDTYPE::F });

				lexTable.Add({ LEX_ID, strNumber, idTable.current_size - 1 });
				alreadyChecked = true;
				tempBlock = new ParrentBlock(token, typeFlag);
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
					if (typeFlag == IT::IDDATATYPE::INT)
						idTable.Add({ blocksStack._Get_container()[0].name, token, IT::IDDATATYPE::INT, IT::IDTYPE::V });

					if (typeFlag == IT::IDDATATYPE::STR)
						idTable.Add({ blocksStack._Get_container()[0].name, token, IT::IDDATATYPE::STR, IT::IDTYPE::V });
				
					if (typeFlag == IT::IDDATATYPE::UBYTE)
						idTable.Add({ blocksStack._Get_container()[0].name, token, IT::IDDATATYPE::UBYTE, IT::IDTYPE::V });

					if (typeFlag == IT::IDDATATYPE::LOGIC)
						idTable.Add({ blocksStack._Get_container()[0].name, token, IT::IDDATATYPE::LOGIC, IT::IDTYPE::V });
				}
				else
					throw ERROR_THROW_IN(123, strNumber+1, -1);
			}
			else
			{
				if (searchingForIDinStack(idTable, blocksStack, token) == -1)
				{
					if (paramFlag) //parameters
					{
						if (typeFlag == IT::IDDATATYPE::INT)
						{
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::INT, IT::IDTYPE::P });
							idTable.table[idTable.IsId(blocksStack.top().name)].funcParams[IT::Entry::FuncParams::currentCount].AddParam(token, IT::IDDATATYPE::INT, strNumber);
							idTable.table[idTable.IsId(blocksStack.top().name)].paramsCount++;
						}

						if (typeFlag == IT::IDDATATYPE::STR)
						{
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::STR, IT::IDTYPE::P });
							idTable.table[idTable.IsId(blocksStack.top().name)].funcParams[IT::Entry::FuncParams::currentCount].AddParam(token, IT::IDDATATYPE::STR, strNumber);
							idTable.table[idTable.IsId(blocksStack.top().name)].paramsCount++;
						}
						if (typeFlag == IT::IDDATATYPE::UBYTE)
						{
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::UBYTE, IT::IDTYPE::P });
							idTable.table[idTable.IsId(blocksStack.top().name)].funcParams[IT::Entry::FuncParams::currentCount].AddParam(token, IT::IDDATATYPE::UBYTE, strNumber);
							idTable.table[idTable.IsId(blocksStack.top().name)].paramsCount++;
						}
						if (typeFlag == IT::IDDATATYPE::LOGIC)
						{
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::LOGIC, IT::IDTYPE::P });
							idTable.table[idTable.IsId(blocksStack.top().name)].funcParams[IT::Entry::FuncParams::currentCount].AddParam(token, IT::IDDATATYPE::LOGIC, strNumber);
							idTable.table[idTable.IsId(blocksStack.top().name)].paramsCount++;
						}
					}
					else //variables
					{
						if (typeFlag == IT::IDDATATYPE::INT)
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::INT, IT::IDTYPE::V });

						if (typeFlag == IT::IDDATATYPE::STR)
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::STR, IT::IDTYPE::V });

						if (typeFlag == IT::IDDATATYPE::UBYTE)
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::UBYTE, IT::IDTYPE::V });

						if (typeFlag == IT::IDDATATYPE::LOGIC)
							idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::LOGIC, IT::IDTYPE::V });
					}
				}
				else
					throw ERROR_THROW_IN(123, strNumber+1, -1);
			}
			
			lexTable.Add({ LEX_ID, strNumber, idTable.current_size - 1 });
			alreadyChecked = true;
		}
		
		typeFlag = IT::IDDATATYPE::DEF;

		//для итератора цикла
		if (!alreadyChecked && flagForLoopIterator)
		{
			int tempIndex = searchingForIDinStack(idTable, blocksStack, token);
			if (tempIndex == -1)
			{
				idTable.Add({ blocksStack.top().name, token, IT::IDDATATYPE::INT, IT::IDTYPE::V });
				lexTable.Add({ LEX_ID, strNumber, idTable.current_size-1 });
				flagForLoopIterator = false;
				alreadyChecked = true;
			}
			else
				throw ERROR_THROW_IN(129, strNumber+1, -1);
		}

		//добавление идентификаторов
		if (!alreadyChecked)
		{

			int tempIndex = idTable.IsId(token);

			if (tempIndex != -1 && idTable.table[tempIndex].idtype == IT::IDTYPE::F)
				lexTable.Add({ LEX_ID, strNumber, tempIndex });
			else
			{
				tempIndex = searchingForIDinStack(idTable, blocksStack, token);
				if (tempIndex != -1)
					lexTable.Add({ LEX_ID, strNumber, tempIndex });
				else
					throw ERROR_THROW_IN(129, strNumber+1, -1);
			}
					
		}

		delete identificator;
		identificator = NULL;
		return true;			
	}
	else
	{	
		delete identificator;
		identificator = NULL;
		//целочисленный литерал
		FST::FST* integer_literal = new FST::FST(INTEGER_LITERAL(token));
		if (FST::execute(*integer_literal))
		{
			int i = idTable.IsLit(token);
			if(i != LT_TI_NULLXDX)
				lexTable.Add({ LEX_LITERAL, strNumber, i });
			else
			{
				int tempValue = atoi(token);
				if (tempValue > TI_INT_MAX_VALUE && tempValue < TI_INT_MIN_VALUE)
					throw ERROR_THROW_IN(409, strNumber + 1, -1);

				if(tempValue >= TI_UBYTE_MIN_VALUE && tempValue <= TI_UBYTE_MAX_VALUE)
					idTable.Add({ '\0', '\0', IT::IDDATATYPE::UBYTE,  IT::IDTYPE::L });
				else
					idTable.Add({ '\0', '\0', IT::IDDATATYPE::INT,  IT::IDTYPE::L });

				idTable.table[idTable.current_size - 1].value.vint = tempValue;
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

			//стринговый литерал
			FST::FST* string_literal = new FST::FST(STRING_LITERAL(token));
			if (FST::execute(*string_literal))
			{
				int i = idTable.IsLit(token);
				if (i != LT_TI_NULLXDX)
					lexTable.Add({ LEX_LITERAL, strNumber, i });
				else
				{
					if(strlen(token)-2 > TI_STR_MAXSIZE)
						throw ERROR_THROW_IN(410, strNumber + 1, -1);

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
			else
			{
				delete string_literal;
				string_literal = NULL;

				int tempLit;
				//двоичная сс
				FST::FST* binary_literal = new FST::FST(BINARY_LITERAL(token));
				if (FST::execute(*binary_literal))
				{
					int i = idTable.IsLit(token);
					if (i != LT_TI_NULLXDX)
						lexTable.Add({ LEX_LITERAL, strNumber, i });
					else
					{
						int tempValue = fromBaseTo10(token, 2);
						if (tempValue < TI_INT_MIN_VALUE && tempValue > TI_INT_MAX_VALUE)
							throw ERROR_THROW_IN(409, strNumber + 1, -1);

						if (tempValue >= TI_UBYTE_MIN_VALUE && tempValue <= TI_UBYTE_MAX_VALUE)
							idTable.Add({ '\0', '\0', IT::IDDATATYPE::UBYTE,  IT::IDTYPE::L });
						else
							idTable.Add({ '\0', '\0', IT::IDDATATYPE::INT,  IT::IDTYPE::L });

						idTable.table[idTable.current_size - 1].value.vint = tempValue;
						lexTable.Add({ LEX_LITERAL, strNumber, idTable.current_size - 1 });
					}

					delete binary_literal;
					binary_literal = NULL;
					return true;
				}
				else
				{
					delete binary_literal;
					binary_literal = NULL;

					//восьмиричная сс
					FST::FST* eight_literal = new FST::FST(EIGHT_LITERAL(token));
					if (FST::execute(*eight_literal))
					{
						int i = idTable.IsLit(token);
						if (i != LT_TI_NULLXDX)
							lexTable.Add({ LEX_LITERAL, strNumber, i });
						else
						{
							int tempValue = fromBaseTo10(token, 8);
							if (tempValue < TI_INT_MIN_VALUE && tempValue > TI_INT_MAX_VALUE)
								throw ERROR_THROW_IN(409, strNumber + 1, -1);

							if (tempValue >= TI_UBYTE_MIN_VALUE && tempValue <= TI_UBYTE_MAX_VALUE)
								idTable.Add({ '\0', '\0', IT::IDDATATYPE::UBYTE,  IT::IDTYPE::L });
							else
								idTable.Add({ '\0', '\0', IT::IDDATATYPE::INT,  IT::IDTYPE::L });

							idTable.table[idTable.current_size - 1].value.vint = tempValue;
							lexTable.Add({ LEX_LITERAL, strNumber, idTable.current_size - 1 });
						}

						delete eight_literal;
						eight_literal = NULL;
						return true;
					}
					else
					{
						delete eight_literal;
						eight_literal = NULL;

						//шестнадтацеричная сс
						FST::FST* hex_literal = new FST::FST(HEX_LITERAL(token));
						if (FST::execute(*hex_literal))
						{
							int i = idTable.IsLit(token);
							if (i != LT_TI_NULLXDX)
								lexTable.Add({ LEX_LITERAL, strNumber, i });
							else
							{
								int tempValue = fromBaseTo10(token, 16);
								if (tempValue < TI_INT_MIN_VALUE && tempValue > TI_INT_MAX_VALUE)
									throw ERROR_THROW_IN(409, strNumber + 1, -1);

								if (tempValue >= TI_UBYTE_MIN_VALUE && tempValue <= TI_UBYTE_MAX_VALUE)
									idTable.Add({ '\0', '\0', IT::IDDATATYPE::UBYTE,  IT::IDTYPE::L });
								else
									idTable.Add({ '\0', '\0', IT::IDDATATYPE::INT,  IT::IDTYPE::L });

								idTable.table[idTable.current_size - 1].value.vint = tempValue;
								lexTable.Add({ LEX_LITERAL, strNumber, idTable.current_size - 1 });
							}

							delete hex_literal;
							hex_literal = NULL;
							return true;
						}
						else
						{
							delete hex_literal;
							hex_literal = NULL;
						}
					}

				}
			}
			
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
		{"cprintl",		LEX_PRINTL},
		{"_connect",	LEX_INCLUDE},
		{"true",		LEX_LITERAL},
		{"false",		LEX_LITERAL},
		{libName,		LEX_LIBRARY},
		{"loop",		LEX_LOOP},
		{"~",			LEX_TILDA},
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
		{"&",			LEX_AND_SIGN},
		{"|",			LEX_OR_SIGN},
		{"!",			LEX_NOT_SIGN}
	};

	char* temp = new char[50]{};
	int strNum = 0;
	int posInStr = 0;

	for (int i = 0, j = 0; i < source.size; i++)
	{
		if ((source.text[i] >= 'A' && source.text[i] <= 'Z') ||
			(source.text[i] >= 'a' && source.text[i] <= 'z') ||
			(source.text[i] >= '0' && source.text[i] <= '9') ||
			(source.text[i] == '-' && lexTable.table[lexTable.current_size-1].idxTI == -1 && j == 0) ||
			 source.text[i] == '_')
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
				//для строковых литералов
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
						temp[j + 1] = '\0';
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

					//== >= <= !=
					if (source.text[i + 1] == '=')
					{
						temp[1] = source.text[(i++)+1];
						temp[2] = '\0';
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

	if (!flagForMain)
		throw ERROR_THROW(401);
}

int searchingForIDinStack(IT::IdTable& idTable, std::stack<ParrentBlock>& stack,const char* token)
{
	int result = TI_NULLIDX;
	for (int i = stack.size() - 1; i >= 0 && result == -1; i--)
	{
		result = idTable.IsId(token, stack._Get_container()[i].name);
		if (result != TI_NULLIDX)
			return result;
	}

	return result;
}

int fromBaseTo10(const char* str, int base)
{
	int n = 0;
	int k;
	str += 1;
	for (; *str != '\0'; str++)
	{
		if (*str <= '9' && *str >= '0') k = *str - '0';
		else if (*str >= 'A' && *str <= 'Z') k = *str - 'A' + 10;
		else continue;
		n = base * n + k;
	}

	return n;
}