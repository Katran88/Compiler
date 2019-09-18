#include "division.h"

bool tokenAnaliz(const char* token, const int strNumber, LT::LexTable& lexTable, IT::IdTable& idTable, const KeyTokens* keyTokens) {
	for (int i = 0; i < keyTokens_size; i++)
		if (strcmp(keyTokens[i].keyToken, token) == 0)
		{
			lexTable.Add({ keyTokens[i].lex, strNumber, LT_TI_NULLXDX });
			return true;
		}

	FST::FST *identificator = new FST::FST(A_IDENTIFICATOR(token));
	if (FST::execute(*identificator))
	{
		bool alreadyChecked = false;
		//для функций
		if (strcmp(token, "main") == 0 ||
			(lexTable.GetEntry(lexTable.current_size - 1).lexema == LEX_FUNCTION &&
			 (lexTable.GetEntry(lexTable.current_size - 2).lexema == LEX_INTEGER || lexTable.GetEntry(lexTable.current_size - 2).lexema == LEX_STRING)))
		{
			if (idTable.IsId(token) == -1)
			{
				if (lexTable.GetEntry(lexTable.current_size - 2).lexema == LEX_INTEGER)
					idTable.Add({ '\0', token, IT::IDDATATYPE::INT, IT::IDTYPE::F });

				if (lexTable.GetEntry(lexTable.current_size - 2).lexema == LEX_STRING)
					idTable.Add({ '\0', token, IT::IDDATATYPE::STR, IT::IDTYPE::F });

				if (strcmp(token, "main") == 0)
					idTable.Add({ '\0', token, IT::IDDATATYPE::INT, IT::IDTYPE::F });

					lexTable.Add({ LEX_ID, strNumber, idTable.current_size - 1 });
					alreadyChecked = true;
			}					
			else
				throw ERROR_THROW_IN(123, strNumber, -1);
		}

		//для переменной(с проверкой переопределения)
			if (!alreadyChecked &&
				((lexTable.GetEntry(lexTable.current_size - 1).lexema == LEX_INTEGER || lexTable.GetEntry(lexTable.current_size - 1).lexema == LEX_STRING)
				  && lexTable.GetEntry(lexTable.current_size - 2).lexema == LEX_DECLARE))
			{
				bool isLeftBraceWas = false;
				for (int i = lexTable.current_size - 1; i > 0; i--)
				{
					if (lexTable.GetEntry(i).lexema == LEX_LEFTBRACE)
						isLeftBraceWas = true;

					if (isLeftBraceWas &&
						lexTable.GetEntry(i).lexema == LEX_ID &&
						idTable.GetEntry(lexTable.GetEntry(i).idxTI).idtype == IT::IDTYPE::F)
					{
						if (idTable.IsId(token, idTable.GetEntry(lexTable.GetEntry(i).idxTI).id) == -1)
						{
							if (lexTable.GetEntry(lexTable.current_size - 1).lexema == LEX_INTEGER)
								idTable.Add({ idTable.GetEntry(lexTable.GetEntry(i).idxTI).id, token, IT::IDDATATYPE::INT, IT::IDTYPE::V });

							if (lexTable.GetEntry(lexTable.current_size - 1).lexema == LEX_STRING)
								idTable.Add({ idTable.GetEntry(lexTable.GetEntry(i).idxTI).id, token, IT::IDDATATYPE::STR, IT::IDTYPE::V });
						
							lexTable.Add({ LEX_ID, strNumber, idTable.current_size - 1 });
							alreadyChecked = true;
							break;
						}
						else
							throw ERROR_THROW_IN(123, strNumber, -1);
							
					}
				}
			}
			
		//для параметра функции
		if (!alreadyChecked && (lexTable.GetEntry(lexTable.current_size - 1).lexema == LEX_INTEGER || lexTable.GetEntry(lexTable.current_size - 1).lexema == LEX_STRING))
			for (int i = lexTable.current_size - 1; i > 0; i--)
			{
						if (lexTable.GetEntry(i).lexema == LEX_ID &&
							idTable.GetEntry(lexTable.GetEntry(i).idxTI).idtype == IT::IDTYPE::F)
						{
							if (lexTable.GetEntry(i - 1).lexema == LEX_FUNCTION &&
								(lexTable.GetEntry(i - 2).lexema == LEX_INTEGER ||
								 lexTable.GetEntry(i - 2).lexema == LEX_STRING))
							{
								if (idTable.IsId(token, idTable.GetEntry(lexTable.GetEntry(i).idxTI).id) == -1)
								{
									if(lexTable.GetEntry(lexTable.current_size - 1).lexema == LEX_INTEGER)
										idTable.Add({ idTable.GetEntry(lexTable.GetEntry(i).idxTI).id, token, IT::IDDATATYPE::INT, IT::IDTYPE::P });
									
									if (lexTable.GetEntry(lexTable.current_size - 1).lexema == LEX_STRING)
										idTable.Add({ idTable.GetEntry(lexTable.GetEntry(i).idxTI).id, token, IT::IDDATATYPE::STR, IT::IDTYPE::P });

									lexTable.Add({ LEX_ID, strNumber, idTable.current_size - 1 });
									alreadyChecked = true;
									break;
								}
								else
									throw ERROR_THROW_IN(123, strNumber, -1);
							}
						}
			}
		
		//добавление идентификаторов с учетом области видимости
		if (!alreadyChecked)
		{
			bool isLeftBraceWas = false;
			for (int i = lexTable.current_size - 1; i > 0; i--)
			{
				if (lexTable.GetEntry(i).lexema == LEX_LEFTBRACE)
					isLeftBraceWas = true;

				if (isLeftBraceWas &&
					lexTable.GetEntry(i).lexema == LEX_ID &&
					idTable.GetEntry(lexTable.GetEntry(i).idxTI).idtype == IT::IDTYPE::F)
				{
					int tempIndex = idTable.IsId(token, idTable.GetEntry(lexTable.GetEntry(i).idxTI).id);
					if (tempIndex != -1)
					{
						lexTable.Add({ LEX_ID, strNumber, tempIndex });
						break;
					}
					else
					{
						tempIndex = idTable.IsId(token);
						if (tempIndex != -1 &&
							idTable.GetEntry(tempIndex).idtype == IT::IDTYPE::F)
						{
							lexTable.Add({ LEX_ID, strNumber, tempIndex });
							break;
						}
						else
							throw ERROR_THROW_IN(129, strNumber, -1);
					}
						
				}
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

void divisionIntoTokens(const In::IN& source, LT::LexTable& lexTable, IT::IdTable& idTable)
{
	const KeyTokens keyTokens[keyTokens_size]
	{
		{"integer",		LEX_INTEGER},
		{"string",		LEX_STRING},
		{"function",	LEX_FUNCTION},
		{"declare",		LEX_DECLARE},
		{"return",		LEX_RETURN},
		{"print",		LEX_PRINT},
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
		{"=",			LEX_EQUAL_SIGN}
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

					temp[0] = source.text[i]; temp[1] = '\0';
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