#include "Generation.h"

void generation(LT::LexTable& LexTable, IT::IdTable& IdTable, std::stack<MFST::MFSTState>& storestate)
{
	std::ofstream* file = new std::ofstream("..\\HDV2019_asm\\HDV2019.asm");
	if (file->is_open())
	{
		*file << STANDART_BEGIN;

		*file << STANDART_LIB;

		if (IT::IdTable::isLibraryIncluded)
			*file << LIB_INCLUDE;

		*file << STACK(4096);

		*file << LITERALS_CONSTANTS_begin;
		*file << DEFAULT_CONSTANTS;
		for (int i = 0; i < IdTable.current_size; i++)
		{
			if(IdTable.table[i].idtype == IT::IDTYPE::L)
				switch(IdTable.table[i].iddatatype)
				{
					case IT::IDDATATYPE::INT:  *file	<<	'L'	<< i << "\tsdword\t"  << IdTable.table[i].value.vint << "\t ; integer literal" << '\n'; break;
					
					case IT::IDDATATYPE::LOGIC: *file	<<	'L'	<< i << "\tbyte\t"   << IdTable.table[i].value.vlogic << "\t ; logic literal" << '\n'; break;

					case IT::IDDATATYPE::STR:
					{
						*file << 'L' << i << "\tbyte\t\'";

						for( int j = 0; IdTable.table[i].value.vstr.str[j] != '\0'; j++)
							*file << IdTable.table[i].value.vstr.str[j];

						*file << "\', " << TI_STR_MAXSIZE - IdTable.table[i].value.vstr.len << " dup(0)" << "\t ; string literal" << '\n';
						break;
					}
					case IT::IDDATATYPE::UBYTE: *file	<<	'L'	<< i << "\tbyte\t" << IdTable.table[i].value.vint << "\t ; ubyte literal" << '\n'; break;
				}
		}
		*file << LITERALS_CONSTANTS_end;

		*file << VARIABLES_begin;
		*file << DEFAULT_VARS; //переписать представление идентификаторов как имя функц. + id !!!
		for (int i = 0; i < IdTable.current_size; i++)
		{
			if (IdTable.table[i].idtype == IT::IDTYPE::V || IdTable.table[i].idtype == IT::IDTYPE::P)
			{
				*file << IdTable.table[i].parrentFunc << "_" << IdTable.table[i].id;
				switch (IdTable.table[i].iddatatype)
				{
				case IT::IDDATATYPE::INT:  *file << "\tsdword\t" << IdTable.table[i].value.vint << "\t ; integer var" << '\n'; break;

				case IT::IDDATATYPE::LOGIC: *file << "\tbyte\t" << IdTable.table[i].value.vlogic << "\t ; logic var" << '\n'; break;

				case IT::IDDATATYPE::STR: *file << "\tbyte\t" << TI_STR_MAXSIZE << " dup(0)" << "\t ; string var" << '\n'; break;

				case IT::IDDATATYPE::UBYTE: *file << "\tbyte\t" << IdTable.table[i].value.vint << "\t ; ubyte var" << '\n'; break;
				}
			}
		}
		*file << VARIABLES_end
		
		*file << CODE_BLOCK;

		*file << FUNCTIONS_begin;

		int currentCheckedPos = 0;
		int currentfuncID = 0; // id в табл ИДЕНТИФИКАТОРОВ для данных в момент разбираемой функции


		char* currentBlockName = NULL; // ссылка на имя текущего блока(цикла или условного оператора)
		int idOfLoopIterator = 0; // для доступа к итератору цикла (id в лекс табл)
		loopFlag loopflag;

		//генереция кода связана с номерами правила и цепочки в грейбах(изменять аккуратно!)
		for(auto& state : storestate._Get_container())
		{	//Предотвращение повторного разбора правил
			if (currentCheckedPos > state.posInLent || (state.nRule == 1 && (state.nRuleChain >= 0 && state.nRuleChain <= 1)))
				continue;
			
			//'шапка' функции
			if (state.nRule == 0 && (state.nRuleChain == 3 || state.nRuleChain == 4))
			{
				//конец функции
				if (state.posInLent - 1 > 0 &&
					IdTable.table[LexTable.table[state.posInLent - 1].idxTI].id[0] == '}' &&
					(strcmp(IdTable.table[LexTable.table[state.posInLent - 1].idxTI].parrentFunc, IdTable.table[LexTable.table[currentCheckedPos + 1].idxTI].parrentFunc) == 0))
				{
					*file << "\n" << IdTable.table[currentfuncID].id << " endp\n\n";
					currentfuncID = 0;
				}

 				funcSignature(file, IdTable, LexTable.table[state.posInLent + 2].idxTI);
				currentfuncID = LexTable.table[state.posInLent + 2].idxTI;

				int i = 0;
				for (i = state.posInLent; LexTable.table[i].lexema != LEX_LEFTBRACE;)
					i++;

				currentCheckedPos = i;

				continue;
			}
			//тело функции
			if (state.nRule == 1)
			{
				if (currentBlockName)
				{
					if (strcmp(currentBlockName, IdTable.table[LexTable.table[state.posInLent - 1].idxTI].parrentFunc) == 0)
					{
						//для цикла
						if (currentBlockName[0] == 'l')
						{
							switch (loopflag)
							{
								case loopFlag::incFlag:
								{
									*file << "inc " << IdTable.table[LexTable.table[idOfLoopIterator].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idOfLoopIterator].idxTI].id << "\n";
									*file << "cmp " << IdTable.table[LexTable.table[idOfLoopIterator].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idOfLoopIterator].idxTI].id << ", " << 'L' << LexTable.table[idOfLoopIterator + 4].idxTI << "\n";
									*file << "jg " << IdTable.table[LexTable.table[idOfLoopIterator].idxTI].parrentFunc << "\tloop end\n\n";
									break;
								}

								case loopFlag::decrFlag:
								{
									*file << "dec " << IdTable.table[LexTable.table[idOfLoopIterator].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idOfLoopIterator].idxTI].id << "\n";
									*file << "cmp " << IdTable.table[LexTable.table[idOfLoopIterator].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idOfLoopIterator].idxTI].id << ", " << 'L' << LexTable.table[idOfLoopIterator + 4].idxTI << "\n";
									*file << "jl " << IdTable.table[LexTable.table[idOfLoopIterator].idxTI].parrentFunc << "\tloop end\n\n";
									break;
								}
							}

							currentBlockName = NULL;
						}
					}
				}

#pragma	region assigment
				//присваивание какому-то идентификатору
				if (state.nRuleChain >= 2 && state.nRuleChain <= 5)
				{
					//для правильного просмотра типа переменной, которой будет все присваиваться
					int offsetFromBegin = 0;
					if (state.nRuleChain == 2 || state.nRuleChain == 3)
						offsetFromBegin = 1;
					if (state.nRuleChain == 4 || state.nRuleChain == 5)
						offsetFromBegin = 0;

					int idForRET = state.posInLent + offsetFromBegin; //номер идентификатора в таблице лексем кому присваивать все это
					int i = idForRET + 2; // пропускаем '=' и становимся на 1й эл-т
					int paramsCounterForConcating = 1; // счетчик стринговых переменных для вызова функции для их конкатонации
					
					for (; LexTable.table[i].lexema != LEX_SEMICOLON; i++)
					{
						if (LexTable.table[i].idxTI != -1)
						{
							switch (IdTable.table[LexTable.table[i].idxTI].iddatatype)
							{
								case IT::IDDATATYPE::UBYTE:
								case IT::IDDATATYPE::LOGIC:
								{
									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::L)
										*file << "\nmovzx eax, " << "L" << LexTable.table[i].idxTI
											  << "\npush eax\n";

									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::V || IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::P)
										* file << "\nmovzx eax, " << IdTable.table[LexTable.table[i].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[i].idxTI].id
											   << "\npush eax\n";

									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::F)
										* file << "\ncall " << IdTable.table[LexTable.table[i].idxTI].id
											   << "\npush eax\n";
									break;
								}

								case IT::IDDATATYPE::INT:
								{
									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::L)
										* file << "\nmov eax, " << "L" << LexTable.table[i].idxTI
											   << "\npush eax\n";

									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::V || IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::P)
										* file << "\nmov eax, " << IdTable.table[LexTable.table[i].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[i].idxTI].id
											   << "\npush eax\n";

									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::F)
										* file << "\ncall " << IdTable.table[LexTable.table[i].idxTI].id
											   << "\npush eax\n";
									break;
								}

								case IT::IDDATATYPE::STR:
								{
									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::L)
										* file << "\npush offset " << "L" << LexTable.table[i].idxTI << "\n";

									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::V || IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::P)
										* file << "\npush offset " << IdTable.table[LexTable.table[i].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[i].idxTI].id << "\n";

									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::F)
										* file << "\ncall " << IdTable.table[LexTable.table[i].idxTI].id
										   	   << "\npush eax\n";
									break;
								}
							}		
						}
						else
						{
							switch (LexTable.table[i].lexema)
							{
								case LEX_PLUS:
								{//INT, UBYTE, STR
									switch (IdTable.table[LexTable.table[i - 1].idxTI].iddatatype)
									{
										case IT::IDDATATYPE::UBYTE:
										case IT::IDDATATYPE::INT:
										{
											*file << "\npop ebx" << "; \t+\n"
												  << "pop eax\n"
												  << "add eax, ebx\n"
												  << "push eax\n";
											break;
										}
										case IT::IDDATATYPE::STR:
										{
											paramsCounterForConcating++;
											break;
										}
									}
									break;
								}
								case LEX_MINUS:
								{//INT, UBYTE
									*file << "\npop ebx" << "; \t-\n"
										  << "pop eax\n"
										  << "sub eax, ebx\n"
										  << "push eax\n";
									break;
								}
								case LEX_STAR:
								{//INT, UBYTE
									*file << "\npop ebx" << "; \t*\n"
										  << "pop eax\n"
										  << "imul eax, ebx\n"
										  << "push eax\n";
									break;
								}
								case LEX_DIRSLASH:
								{//INT, UBYTE
									*file << "\npop ebx" << "; \t/\n"
										  << "pop eax\n"
										  << "cdq\n"
										  << "cmp ebx, 0\n"
										  << "jne NOzero\n"
										  << "\npush 0 \t;DIVISION_BY_ZERO\n"
										  << "push 0\n"
										  << "push 0\n"
										  << "push sizeof DIVISION_BY_ZERO_text\n"
										  << "push offset DIVISION_BY_ZERO_text\n"
										  << "push consoleHandle\n"
										  << "call WriteConsoleA\n"
										  << "NOzero:\n"
										  << "idiv ebx\n"
										  << "push eax\n";
									break;
								}
								case LEX_OR_SIGN:
								{//INT, UBYTE, LOGIC
									*file << "\npop ebx" << "; \t|\n"
										  << "pop eax\n"
										  << "or eax, ebx\n"
										  << "push eax\n";
									break;
								}
								case LEX_AND_SIGN:
								{//INT, UBYTE, LOGIC
									*file << "\npop ebx" << "; \t&\n"
										  << "pop eax\n"
										  << "or eax, ebx\n"
										  << "push eax\n";
									break;
								}
								case LEX_NOT_SIGN:
								{
									*file << "\npop eax" << "; \t!\n"
										  << "not eax\n"
										  << "push eax\n";
									break;
								}
								default: break;
							}
						}
					}
					//assigment finish
					switch (IdTable.table[LexTable.table[idForRET].idxTI].iddatatype)
					{
						case IT::IDDATATYPE::INT:
						{
							*file << "\npop eax" << "\t;assigment to INT begin\n"
								  << "cmp eax, 2147483647\n"
								  << "jl isOverflow\n"
								  << "cmp eax, -2147483648\n"
								  << "jg isOverflow\n" 
								  << "mov " << IdTable.table[LexTable.table[idForRET].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idForRET].idxTI].id <<", eax\n"
								  << "jmp isNotOverflow\n"
								  << "isOverflow:\n"
								  << "\npush 0 \t;INT OVERFLOW\n"
								  << "push 0\n"
								  << "push 0\n"
								  << "push sizeof INTOVERFLOW_text\n"
								  << "push offset INTOVERFLOW_text\n"
								  << "push consoleHandle\n"
								  << "call WriteConsoleA\n"
								  << "push 0\n"
								  << "call ExitProcess \t;INT OVERFLOW\n\n"
							  	  << "isNotOverflow: \t;assigment to INT done\n\n";
							break;
						}
						case IT::IDDATATYPE::LOGIC:
						{
							*file << "\npop eax" << "\t;assigment to LOGIC begin\n"
								  << "cmp eax, 0\n"
								  << "je zeroEqual\n"
								  << "mov " << IdTable.table[LexTable.table[idForRET].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idForRET].idxTI].id << ", 1\n"
								  << "jmp ZeroNOTequal\n"
								  << "zeroEqual:\n"
								  << "mov V" << idForRET << ", 0\n"
								  << "zeroNOTequal: \t;assigment to LOGIC done\n\n";
							break;
						}
						case IT::IDDATATYPE::STR:
						{
							*file << "\npush offset " << IdTable.table[LexTable.table[idForRET].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idForRET].idxTI].id << "\t;assigment to STR begin\n"
								  << "call concat" << paramsCounterForConcating << "\n"
								  << "cmp " << IdTable.table[LexTable.table[idForRET].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idForRET].idxTI].id << ", 0\n"
								  << "jne notOverflow\n"
								  << "push 0\n call"
								  << "ExitProccess\n"
								  << "notOverflow: \t;assigment to STR begin\n\n";

							paramsCounterForConcating = 0;
							break;
						}
						case IT::IDDATATYPE::UBYTE:
						{
							*file << "\npop eax" << "\t;assigment to UBYTE begin\n"
								  << "cmp eax, 255\n"
								  << "jl isOverflow\n"
								  << "cmp eax, 0\n"
								  << "jg isOverflow\n"
								  << "mov " << IdTable.table[LexTable.table[idForRET].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idForRET].idxTI].id << ", al\n"
								  << "jmp isNotOverflow\n"
								  << "isOverflow:\n"
								  << "\npush 0 \t;UBYTE OVERFLOW\n"
								  << "push 0\n"
								  << "push 0\n"
								  << "push sizeof UBYTEOVERFLOW_text\n"
								  << "push offset UBYTEOVERFLOW_text\n"
								  << "push consoleHandle\n"
								  << "call WriteConsoleA\n"
								  << "push 0\n"
								  << "call ExitProcess \t;UBYTE OVERFLOW\n\n"
								  << "isNotOverflow: \t;assigment to UBYTE done\n";
							break;						
						}
					}
					currentCheckedPos = i;
					LexTable.table[i].lexema;
				}

#pragma endregion

#pragma region return

				if (state.nRuleChain == 6 || state.nRuleChain == 7)
				{
					int i = state.posInLent + 1;

					switch (IdTable.table[LexTable.table[i].idxTI].iddatatype)
					{
						case IT::IDDATATYPE::INT:
						{
							if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::L)
								* file << "\nmov eax, L" << LexTable.table[i].idxTI << " \t;begin of return INT\n";
							else
								*file << "\nmov eax, " << IdTable.table[LexTable.table[i].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[i].idxTI].id << " \tbegin of return INT\n";
							break;
						}
						case IT::IDDATATYPE::UBYTE:
						case IT::IDDATATYPE::LOGIC:
						{
							if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::L)
								* file << "\nmovzx eax, L" << LexTable.table[i].idxTI << " \t;begin of return UBYTE or LOGIC\n";
							else
								*file << "\nmovzx eax, " << IdTable.table[LexTable.table[i].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[i].idxTI].id << " \t;begin of return UBYTE or LOGIC\n";
							break;
						}

						case IT::IDDATATYPE::STR:
						{
							if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::L)
								* file << "\nmov eax, offset L" << LexTable.table[i].idxTI << " \t;begin of return STR\n";
							else
								*file << "\nmov eax, offset " << IdTable.table[LexTable.table[i].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[i].idxTI].id << " \t;begin of return STR\n";
							break;
						}
					}

					//для корректной очистки стека после вызова функции
					*file << "ret " << 4 * IdTable.table[LexTable.table[state.posInLent].idxTI].paramsCount << "\t;end of return\n";
					currentCheckedPos = state.posInLent + 2;
				}

#pragma endregion

#pragma region function_invoke

				if (state.nRuleChain == 8 || state.nRuleChain == 9)
				{
					*file << "\n\t;function calling\n"
						  << "invoke " << IdTable.table[LexTable.table[state.posInLent].idxTI].id;

					//итератор для корректной записи параметров в функцию
					int paramsIterator = state.posInLent + 2;

					for (int i = 0; IdTable.table[LexTable.table[state.posInLent].idxTI].funcParams[i].type != IT::IDDATATYPE::DEF; i++)
					{
						*file << ", " << IdTable.table[LexTable.table[paramsIterator].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[paramsIterator].idxTI].id;
						paramsIterator += 2;
					}

					*file << "\n\n";

					currentCheckedPos = paramsIterator;
				} 

				if (state.nRuleChain == 10 || state.nRuleChain == 11)
				{
					*file << "\n\;tfunction calling\n"
						  << "invoke " << IdTable.table[LexTable.table[state.posInLent].idxTI].id << "\n\n";
					currentCheckedPos = state.posInLent + 3;
				}

#pragma endregion

#pragma region cprint

				if (state.nRuleChain == 12 || state.nRuleChain == 13)
				{
					if (IdTable.table[LexTable.table[state.posInLent + 1].idxTI].idtype == IT::IDTYPE::L)
					{
						*file << "\npush 0 \t;cprint\n"
							  << "push 0\n"
							  << "push sizeof " << "L" << LexTable.table[state.posInLent + 1].idxTI << "\n"
							  << "push offset " << "L" << LexTable.table[state.posInLent + 1].idxTI << "\n"
							  << "call WriteConsoleA\n\n";
					}
					else
					{
						*file << "\npush 0 \t;cprint\n"
							  << "push 0\n"
							  << "push sizeof " << IdTable.table[LexTable.table[state.posInLent + 1].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[state.posInLent + 1].idxTI].id << "\n"
							  << "push offset " << IdTable.table[LexTable.table[state.posInLent + 1].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[state.posInLent + 1].idxTI].id << "\n"
							  << "call WriteConsoleA\n\n";
					}
					currentCheckedPos = state.posInLent + 2;

				}

#pragma endregion

#pragma region loop

				if (state.nRuleChain == 14 || state.nRuleChain == 15)
				{
					//позиция в лексической таблице итератора цикла
					int i = state.posInLent + 2;
					if (IdTable.table[LexTable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::UBYTE)
						* file << "\nmovzx eax, " << "L" << LexTable.table[i + 2].idxTI << "\t;loop begin\n";
					
					if (IdTable.table[LexTable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::INT)
						*file << "\nmov eax, " << "L" << LexTable.table[i + 2].idxTI << "\t;loop begin\n";

					*file << "mov " << IdTable.table[LexTable.table[i].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[i].idxTI].id << ", eax\n"
						  << IdTable.table[LexTable.table[i].idxTI].parrentFunc << ": \n";

					if (IdTable.table[LexTable.table[i + 2].idxTI].value.vint > IdTable.table[LexTable.table[i + 4].idxTI].value.vint)
						loopflag = loopFlag::decrFlag;
					else
						loopflag = loopFlag::incFlag;

					idOfLoopIterator = i;

					currentBlockName = IdTable.table[LexTable.table[i].idxTI].parrentFunc;
					currentCheckedPos = state.posInLent + 8;
				}

#pragma endregion

			}
		}

		*file << FUNCTIONS_end
	}
	else
		throw ERROR_THROW(101);
}

void funcSignature(std::ofstream* file, IT::IdTable& IdTable, int i)
{
	if (IdTable.table[i].idtype == IT::IDTYPE::F)
	{
		*file << IdTable.table[i].id << " proc  ";
		for (int j = 0; IdTable.table[i].funcParams[j].type != IT::IDDATATYPE::DEF; j++)
		{
			switch (IdTable.table[i].funcParams[j].type)
			{
				case IT::IDDATATYPE::INT:	*file << IdTable.table[i].id << "_" << IdTable.table[i].funcParams[j].id << " : " << "sdword";  break;

				case IT::IDDATATYPE::LOGIC: *file << IdTable.table[i].id << "_" << IdTable.table[i].funcParams[j].id << " : " << "sdword";  break;

				case IT::IDDATATYPE::STR:	*file << IdTable.table[i].id << "_" << IdTable.table[i].funcParams[j].id << " : " << "sdword";  break;

				case IT::IDDATATYPE::UBYTE:	*file << IdTable.table[i].id << "_" << IdTable.table[i].funcParams[j].id << " : " << "sdword";  break;
			}
			if (j != TI_MAXPARAM_COUNT && IdTable.table[i].funcParams[j + 1].type != IT::IDDATATYPE::DEF)
				* file << ", ";
		}
		*file << "\n\n";
	}
}
