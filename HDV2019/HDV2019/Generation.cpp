#include "Generation.h"
void assigment(LT::LexTable& LexTable, IT::IdTable& IdTable, std::ofstream* file, const MFST::MFSTState& state, int& currentCheckedPos, flagForCprint flagForCprint);

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
						*file << 'L' << i << "\tbyte\t\"";

						for( int j = 0; IdTable.table[i].value.vstr.str[j] != '\0'; j++)
							*file << IdTable.table[i].value.vstr.str[j];

						*file << "\", " << TI_STR_MAXSIZE - IdTable.table[i].value.vstr.len << " dup(0)" << "\t ; string literal" << '\n';
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
			if (IdTable.table[i].idtype == IT::IDTYPE::V)
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
		for (int i = 0; i < LexTable.current_size; i++)
		{
			if(LexTable.table[i].lexema == LEX_ID && strcmp(IdTable.table[LexTable.table[i].idxTI].id, "inttostr") == 0)
				*file << IdTable.table[LexTable.table[i].idxTI].id << "_" << i << "\tbyte\t" << 17 << " dup(0)" << "\t ; return var" << '\n';

			if (LexTable.table[i].lexema == LEX_ID && strcmp(IdTable.table[LexTable.table[i].idxTI].id, "getDate") == 0 ||
				LexTable.table[i].lexema == LEX_ID && strcmp(IdTable.table[LexTable.table[i].idxTI].id, "getTime") == 0)
				* file << IdTable.table[LexTable.table[i].idxTI].id << "_" << i << "\tbyte\t" << TI_MAXSIZE << " dup(0)" << "\t ; return var" << '\n';
		}
		*file << VARIABLES_end
		
		*file << CODE_BLOCK;

		*file << FUNCTIONS_begin;

		int currentCheckedPos = 0;
		int currentfuncID = 0; // id в табл ИДЕНТИФИКАТОРОВ для данных в момент разбираемой функции
		std::stack<innerBlock> blocks; // текущий блок(цикла или условного оператора)

		//генереция кода связана с номерами правила и цепочки в грейбах(изменять аккуратно!)
		for(const MFST::MFSTState& state : storestate._Get_container())
		{	//Предотвращение повторного разбора правил
			if (currentCheckedPos > state.posInLent || (state.nRule == 1 && (state.nRuleChain >= 0 && state.nRuleChain <= 1)))
				continue;
			
			//'шапка' функции
			if (state.nRule == 0)
			{
				//конец функции
				if (state.posInLent - 1 > 0 &&
					IdTable.table[LexTable.table[state.posInLent - 1].idxTI].id[0] == '}' &&
					(strcmp(IdTable.table[LexTable.table[state.posInLent - 1].idxTI].parrentFunc, IdTable.table[LexTable.table[currentCheckedPos + 1].idxTI].parrentFunc) == 0))
				{
					*file << "\n" << IdTable.table[currentfuncID].id << " endp\n\n";
					currentfuncID = 0;
				}

				//main
				if (state.nRuleChain == 2)
				{
					*file << FUNCTIONS_end;

					*file << "main proc \t;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<MAIN\n\n";
					*file << ";----------Initialization of global variables----------\n";
					for (const MFST::MFSTState& st : storestate._Get_container())
						if (st.nRule == 0 && st.nRuleChain == 0)
							assigment(LexTable, IdTable, file, st, currentCheckedPos, flagForCprint::none);
					*file << ";------------------------------------------------------\n";
				}

				if (state.nRuleChain == 3 || state.nRuleChain == 4)
				{
					funcSignature(file, IdTable, LexTable.table[state.posInLent + 2].idxTI);
					currentfuncID = LexTable.table[state.posInLent + 2].idxTI;
					int i = 0;

					for (i = state.posInLent; LexTable.table[i].lexema != LEX_LEFTBRACE;)
						i++;
					currentCheckedPos = i;
				}
				continue;
			}
			//тело функции
			if (state.nRule == 1)
			{

#pragma region innerblocks
				// конец внутренних блоков(циклов и условных операторов)
				if (blocks.size() != 0)
				{
					int nearblocksCounter = 0;

					for (int z = state.posInLent - 1; z > 0; z--)
					{
						if (LexTable.table[z].idxTI != -1 &&
							IdTable.table[LexTable.table[z].idxTI].id[0] == '}')
							nearblocksCounter++;
						else
							break;
					}

					while (nearblocksCounter > 0)
					{
						if (strcmp(blocks.top().blockName, IdTable.table[LexTable.table[state.posInLent - nearblocksCounter].idxTI].parrentFunc) == 0)
						{
							//для цикла
							if (blocks.size() != 0 && blocks.top().blockName[0] == 'l')
							{
								switch (blocks.top().loopflag)
								{
								case loopFlag::incFlag:
								{
									*file << "inc " << IdTable.table[LexTable.table[blocks.top().idOfLoopIterator].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[blocks.top().idOfLoopIterator].idxTI].id << "\n";
									*file << "mov eax, " << IdTable.table[LexTable.table[blocks.top().idOfLoopIterator].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[blocks.top().idOfLoopIterator].idxTI].id << "\n";

									if (IdTable.table[LexTable.table[blocks.top().idOfLoopIterator + 4].idxTI].iddatatype == IT::IDDATATYPE::INT)
										* file << "mov ebx, L" << LexTable.table[blocks.top().idOfLoopIterator + 4].idxTI << "\n";
									else
										*file << "movzx ebx, L" << LexTable.table[blocks.top().idOfLoopIterator + 4].idxTI << "\n";
									*file << "cmp eax, ebx\n";
									*file << "jle " << IdTable.table[LexTable.table[blocks.top().idOfLoopIterator].idxTI].parrentFunc << "\n";
									break;
								}

								case loopFlag::decrFlag:
								{
									*file << "dec " << IdTable.table[LexTable.table[blocks.top().idOfLoopIterator].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[blocks.top().idOfLoopIterator].idxTI].id << "\n";
									*file << "mov eax, " << IdTable.table[LexTable.table[blocks.top().idOfLoopIterator].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[blocks.top().idOfLoopIterator].idxTI].id << "\n";

									if (IdTable.table[LexTable.table[blocks.top().idOfLoopIterator + 4].idxTI].iddatatype == IT::IDDATATYPE::INT)
										* file << "mov ebx, L" << LexTable.table[blocks.top().idOfLoopIterator + 4].idxTI << "\n";
									else
										*file << "movzx ebx, L" << LexTable.table[blocks.top().idOfLoopIterator + 4].idxTI << "\n";

									*file << "cmp eax, ebx\n";
									*file << "jge " << IdTable.table[LexTable.table[blocks.top().idOfLoopIterator].idxTI].parrentFunc << "\n";
									break;
								}
								default: break;
								}

								blocks.pop();
								*file << "\n;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<loop end\n";
							}
							//для условного блока
							if (blocks.size() != 0 && blocks.top().blockName[0] == 'i')
							{
								*file << blocks.top().blockName << ":\t;<<<<<<<<<<<<<<<<<<<<<<<<<<< if block end\n\n";
								blocks.pop();
							}
							nearblocksCounter--;
						}
					}
				}
#pragma endregion

#pragma	region assigment
				//присваивание какому-то идентификатору
				if (state.nRuleChain >= 2 && state.nRuleChain <= 5)
				{
					assigment(LexTable, IdTable, file, state, currentCheckedPos, flagForCprint::none);
				}

#pragma endregion

#pragma region return

				if (state.nRuleChain == 6 || state.nRuleChain == 7)
				{
					*file << "\n;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<return begin\n\n";
					int i = state.posInLent + 1;

					switch (IdTable.table[LexTable.table[i].idxTI].iddatatype)
					{
						case IT::IDDATATYPE::INT:
						{
							if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::L)
								* file << "\nmov eax, L" << LexTable.table[i].idxTI << " \t;return INT\n";
							else
								*file << "\nmov eax, " << IdTable.table[LexTable.table[i].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[i].idxTI].id << " \t;return INT\n";
							break;
						}
						case IT::IDDATATYPE::UBYTE:
						case IT::IDDATATYPE::LOGIC:
						{
							if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::L)
								* file << "\nmovzx eax, L" << LexTable.table[i].idxTI << " \t;return UBYTE or LOGIC\n";
							else
								*file << "\nmovzx eax, " << IdTable.table[LexTable.table[i].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[i].idxTI].id << " \t;return UBYTE or LOGIC\n";
							break;
						}

						case IT::IDDATATYPE::STR:
						{
							if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::L)
								* file << "\nmov eax, offset L" << LexTable.table[i].idxTI << " \t;return STR\n";
							else
								*file << "\nmov eax, offset " << IdTable.table[LexTable.table[i].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[i].idxTI].id << " \t;return STR\n";
							break;
						}
					}

					//для корректной очистки стека после вызова функции
					*file << "ret " << 4 * IdTable.table[LexTable.table[state.posInLent].idxTI].paramsCount << "\n";
					*file << "\n;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<return end\n\n";

					currentCheckedPos = state.posInLent + 2;
				}

#pragma endregion

#pragma region function_invoke

				if (state.nRuleChain == 8 || state.nRuleChain == 9)
				{
					*file << "\n;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<function calling\n"
						  << "invoke " << IdTable.table[LexTable.table[state.posInLent].idxTI].id;

					if (strcmp(IdTable.table[LexTable.table[state.posInLent].idxTI].id, "inttostr") == 0)
						* file << ", offset " << IdTable.table[LexTable.table[state.posInLent].idxTI].id << "_" << state.posInLent;

					if (strcmp(IdTable.table[LexTable.table[state.posInLent].idxTI].id, "getDate") == 0)
						* file << ", offset " << IdTable.table[LexTable.table[state.posInLent].idxTI].id << "_" << state.posInLent;

					if (strcmp(IdTable.table[LexTable.table[state.posInLent].idxTI].id, "getTime") == 0)
						* file << ", offset " << IdTable.table[LexTable.table[state.posInLent].idxTI].id << "_" << state.posInLent;

					//итератор для корректной записи параметров в функцию
					int paramsIterator = state.posInLent + 2;

					for (int i = 0; IdTable.table[LexTable.table[state.posInLent].idxTI].funcParams[i].type != IT::IDDATATYPE::DEF; i++)
					{
						*file << ", ";
						if (IdTable.table[LexTable.table[paramsIterator].idxTI].idtype == IT::IDDATATYPE::STR)
							*file << "offset ";
						*file << IdTable.table[LexTable.table[paramsIterator].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[paramsIterator].idxTI].id;
						paramsIterator += 2;
					}

					*file << "\n\n";

					currentCheckedPos = paramsIterator;
				} 

				if (state.nRuleChain == 10 || state.nRuleChain == 11)
				{
					*file << "\n;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<function calling\n"
						  << "invoke " << IdTable.table[LexTable.table[state.posInLent].idxTI].id << "\n\n";
					currentCheckedPos = state.posInLent + 3;
				}

#pragma endregion

#pragma region cprint

				if (state.nRuleChain == 12 || state.nRuleChain == 13)
				{
					*file << "\n;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprint begin";
						assigment(LexTable, IdTable, file, state, currentCheckedPos, flagForCprint::cprint);
						*file << "push offset " << cprint_var << "\n"
							  << "call cprint\n";
					*file << ";<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprint end\n\n";
				}

				if (state.nRuleChain == 14 || state.nRuleChain == 15)
				{
					*file << "\n;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl begin";
					assigment(LexTable, IdTable, file, state, currentCheckedPos, flagForCprint::cprintl);
					*file << "push offset " << cprintl_var << "\n"
						  << "call cprintl\n";
					*file << ";<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl end\n\n";
				}

#pragma endregion

#pragma region loop

				if (state.nRuleChain == 16 || state.nRuleChain == 17)
				{
					*file << "\n;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<loop begin\n";

					//позиция в лексической таблице итератора цикла
					int i = state.posInLent + 2;
					if (IdTable.table[LexTable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::UBYTE)
						* file << "\nmovzx eax, " << "L" << LexTable.table[i + 2].idxTI << "\n";
					
					if (IdTable.table[LexTable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::INT)
						*file << "\nmov eax, " << "L" << LexTable.table[i + 2].idxTI << "\n";

					*file << "mov " << IdTable.table[LexTable.table[i].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[i].idxTI].id << ", eax\n"
						  << IdTable.table[LexTable.table[i].idxTI].parrentFunc << ": \n";

					if (IdTable.table[LexTable.table[i + 2].idxTI].value.vint > IdTable.table[LexTable.table[i + 4].idxTI].value.vint)
						blocks.push({ IdTable.table[LexTable.table[i].idxTI].parrentFunc, loopFlag::decrFlag, i });
					else
						blocks.push({ IdTable.table[LexTable.table[i].idxTI].parrentFunc, loopFlag::incFlag, i });
					currentCheckedPos = state.posInLent + 8;
				}

#pragma endregion

#pragma region if

				if (state.nRuleChain == 18 || state.nRuleChain == 19)
				{
					*file << "\n\n;<<<<<<<<<<<<<<<<<<<<<<<<<<<if block begin\n\n";
					blocks.push({ IdTable.table[LexTable.table[state.posInLent].idxTI].id, loopFlag::undef, -1 });
					char regiteR[] = "eax";
					int i = state.posInLent + 2;
					for (int j = 1; LexTable.table[i].lexema != LEX_LEFTBRACE; j++)
					{
						if (j % 2 != 0)
							regiteR[1] = 'a';
						else
							regiteR[1] = 'b';

						switch (IdTable.table[LexTable.table[i].idxTI].iddatatype)
						{
							case IT::IDDATATYPE::INT:
							{
								switch (IdTable.table[LexTable.table[i].idxTI].idtype)
								{
									case IT::IDTYPE::L:
									{
										*file << "mov " << regiteR <<", L" << LexTable.table[i].idxTI << "\n";
										break;
									}
									case IT::IDTYPE::V:
									{
										*file << "mov " << regiteR << ", " << IdTable.table[LexTable.table[i].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[i].idxTI].id << "\n";
										break;
									}
								}
								break;
							}

							case IT::IDDATATYPE::UBYTE:
							case IT::IDDATATYPE::LOGIC:
							{
								switch (IdTable.table[LexTable.table[i].idxTI].idtype)
								{
								case IT::IDTYPE::L:
								{
									*file << "movzx " << regiteR << ", L" << LexTable.table[i].idxTI << "\n";
									break;
								}
								case IT::IDTYPE::V:
								{
									*file << "movzx " << regiteR << ", " << IdTable.table[LexTable.table[i].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[i].idxTI].id << "\n";
									break;
								}
								}
								break;
							}

							case IT::IDDATATYPE::STR:
							{
								switch (IdTable.table[LexTable.table[i].idxTI].idtype)
								{
									case IT::IDTYPE::L:
									{
										*file << "mov " << regiteR << ", lengthof L" << LexTable.table[i].idxTI << "\n";
										break;
									}
									case IT::IDTYPE::V:
									{
										*file << "mov " << regiteR << ", lengthof " << IdTable.table[LexTable.table[i].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[i].idxTI].id << "\n";
										break;
									}
								}
								break;
							}
						}

						i += 2;

						if (j % 2 == 0)
						{
							*file << "cmp eax, ebx \n";
							//устанавливаем флаги процессора
							switch (LexTable.table[i-3].lexema)
							{
								case LEX_LESS_SIGN: // <
								{
									*file << "jge s" << i - 3 << "\t; <\n";
									break;
								}
								case LEX_MORE_SIGN: // >
								{
									*file << "jle s" << i - 3 << "\t; >\n";
									break;
								}
								case LEX_SAME_SIGN: // ==
								{
									*file << "jne s" << i - 3 << "\t; ==\n";
									break;
								}
								case LEX_DIFFERENT_SIGN: // !=
								{
									*file << "je s" << i - 3 << "\t; !=\n";
									break;
								}
								case LEX_MORE_SAME_SIGN: // >=
								{
									*file << "jl s" << i - 3 << "\t; >=\n";
									break;
								}
								case LEX_LESS_SAME_SIGN: // <=
								{
									*file << "jg s" << i - 3 << "\t; <=\n";
									break;
								}

							default:
								break;
							}
							//соблюдение всех условий
							switch (LexTable.table[i-5].lexema)
							{
								case LEX_LEFTHESIS:
								case LEX_OR_SIGN:
								{
									*file << "or " << ifFlag << ", 1\n";
									break;
								}

								case LEX_AND_SIGN:
								{
									*file << "and " << ifFlag << ", 1\n";
									break;
								}

								default: break;
							}

							*file << "\ns" << i - 3 << ":\t; condition is not done\n";
						}
					}

					*file << "\n cmp " << ifFlag << ", 0\t; check for result flag\n";
					*file << "je " << IdTable.table[LexTable.table[state.posInLent].idxTI].id << "\n\n";
					
					currentCheckedPos = i;
				}
#pragma endregion

			}
		}

		*file << "\npush 0\ncall ExitProcess\n\n"
			  << "main endp\n"
			  << "end main";
		file->close();
	}
	else
		throw ERROR_THROW(101);

		delete file;
		file = NULL;
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

				case IT::IDDATATYPE::LOGIC: *file << IdTable.table[i].id << "_" << IdTable.table[i].funcParams[j].id << " : " << "byte";  break;

				case IT::IDDATATYPE::STR:	*file << IdTable.table[i].id << "_" << IdTable.table[i].funcParams[j].id << " : " << "dword";  break;

				case IT::IDDATATYPE::UBYTE:	*file << IdTable.table[i].id << "_" << IdTable.table[i].funcParams[j].id << " : " << "byte";  break;
			}
			if (j != TI_MAXPARAM_COUNT && IdTable.table[i].funcParams[j + 1].type != IT::IDDATATYPE::DEF)
				* file << ", ";
		}
		*file << "\n\n";
	}
}


void assigment(LT::LexTable& LexTable, IT::IdTable& IdTable, std::ofstream* file, const MFST::MFSTState& state, int& currentCheckedPos, flagForCprint flagForCprint)
{
	*file << "\n;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin\n\n";

	//для правильного просмотра типа переменной, которой будет все присваиваться
	int offsetFromBegin = 0;
	if(state.nRuleChain == 0 && state.nRule != 0)
		offsetFromBegin = 4;

	if (state.nRuleChain == 2 || state.nRuleChain == 3)
		offsetFromBegin = 1;

	if (state.nRuleChain == 4 || state.nRuleChain == 5)
		offsetFromBegin = 0;

	//для глобальных переменных
	if (state.nRule == 0 && state.nRuleChain == 0)
		offsetFromBegin = 2;

	int idForRET = state.posInLent + offsetFromBegin; //номер идентификатора в таблице лексем кому присваивать все это
	int i; // пропускаем '=' и становимся на 1й эл-т
	if (flagForCprint == flagForCprint::none)
		i = idForRET + 2;
	else
		i = idForRET + 1;

	int paramsCounterForConcating = 1; // счетчик стринговых переменных для вызова функции для их конкатонации
	static int pointsIterator = 0; // счетчик для меток в ассемблере
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
						* file << "\nmovzx eax, " << "L" << LexTable.table[i].idxTI
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
					{
						if (strcmp(IdTable.table[LexTable.table[i].idxTI].id, "inttostr") == 0)
							* file << "\npush offset " << IdTable.table[LexTable.table[i].idxTI].id << "_" << i;

						if (strcmp(IdTable.table[LexTable.table[i].idxTI].id, "getTime") == 0)
							* file << "\npush offset " << IdTable.table[LexTable.table[i].idxTI].id << "_" << i;

						if (strcmp(IdTable.table[LexTable.table[i].idxTI].id, "getDate") == 0)
							* file << "\npush offset " << IdTable.table[LexTable.table[i].idxTI].id << "_" << i;

						*file << "\ncall " << IdTable.table[LexTable.table[i].idxTI].id
							<< "\npush eax\n";
					}
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
						<< "jne NOzero"<< pointsIterator <<"\n"
						<< "\n\t;DIVISION_BY_ZERO\n"
						<< "push offset DIVISION_BY_ZERO_text\n"
						<< "call cprintl\n"
						<< "push 0\n"
						<< "call ExitProcess\n"
						<< "NOzero" << pointsIterator << ":\n"
						<< "idiv ebx\n"
						<< "push eax\n";
					pointsIterator++;
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
	switch (flagForCprint)
	{
		case flagForCprint::none:
		{
			switch (IdTable.table[LexTable.table[idForRET].idxTI].iddatatype)
			{
				case IT::IDDATATYPE::INT:
				{
					*file << "\npop eax" << "\t;assigment to INT begin\n"
						<< "cmp eax, 2147483647\n"
						<< "jg isOverflow" << pointsIterator << "\n"
						<< "cmp eax, -2147483648\n"
						<< "jl isOverflow" << pointsIterator << "\n"
						<< "mov " << IdTable.table[LexTable.table[idForRET].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idForRET].idxTI].id << ", eax\n"
						<< "jmp isNotOverflow" << pointsIterator << "\n"
						<< "isOverflow" << pointsIterator << ":\n"
						<< "\n\t;INT OVERFLOW\n"
						<< "push offset INTOVERFLOW_text\n"
						<< "call cprintl\n"
						<< "push 0\n"
						<< "call ExitProcess \t;INT OVERFLOW\n\n"
						<< "isNotOverflow" << pointsIterator << ": \t;assigment to INT done\n\n";
					break;
				}
				case IT::IDDATATYPE::LOGIC:
				{
					*file << "\npop eax" << "\t;assigment to LOGIC begin\n"
						<< "cmp eax, 0\n"
						<< "je zeroEqual" << pointsIterator << "\n"
						<< "mov " << IdTable.table[LexTable.table[idForRET].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idForRET].idxTI].id << ", 1\n"
						<< "jmp ZeroNOTequal" << pointsIterator << "\n"
						<< "zeroEqual" << pointsIterator << ":\n"
						<< "mov " << IdTable.table[LexTable.table[idForRET].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idForRET].idxTI].id << ", 0\n"
						<< "zeroNOTequal" << pointsIterator << ": \t;assigment to LOGIC done\n\n";
					break;
				}
				case IT::IDDATATYPE::STR:
				{
					*file << "\npush offset " << IdTable.table[LexTable.table[idForRET].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idForRET].idxTI].id << "\t;assigment to STR begin\n"
						<< "call concat" << paramsCounterForConcating << "\n"
						<< "cmp " << IdTable.table[LexTable.table[idForRET].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idForRET].idxTI].id << ", 0\n"
						<< "jne notOverflow" << pointsIterator << "\n"
						<< "push offset STROVERFLOW_text\n"
						<< "call cprintl\n"
						<< "push 0\n"
						<< "call ExitProcess\n"
						<< "notOverflow" << pointsIterator << ": \t;assigment to STR begin\n\n";
					paramsCounterForConcating = 0;
					break;
				}
				case IT::IDDATATYPE::UBYTE:
				{
					*file << "\npop eax" << "\t;assigment to UBYTE begin\n"
						<< "cmp eax, 255\n"
						<< "jg isOverflow" << pointsIterator << "\n"
						<< "cmp eax, 0\n"
						<< "jl isOverflow" << pointsIterator << "\n"
						<< "mov " << IdTable.table[LexTable.table[idForRET].idxTI].parrentFunc << "_" << IdTable.table[LexTable.table[idForRET].idxTI].id << ", al\n"
						<< "jmp isNotOverflow" << pointsIterator << "\n"
						<< "isOverflow" << pointsIterator << ":\n"
						<< "\n\t;UBYTE OVERFLOW\n"
						<< "push offset UBYTEOVERFLOW_text\n"
						<< "call cprintl\n"
						<< "push 0\n"
						<< "call ExitProcess \t;UBYTE OVERFLOW\n\n"
						<< "isNotOverflow" << pointsIterator << ": \t;assigment to UBYTE done\n";
					break;
				}
			}
			break;
		}

		case flagForCprint::cprint:
		{
		  *file << "\npush offset " << cprint_var << "\t;assigment to STR begin\n"
				<< "call concat" << paramsCounterForConcating << "\n"
				<< "cmp " << cprint_var << ", 0\n"
				<< "jne notOverflow" << pointsIterator << "\n"
			    << "push offset STROVERFLOW_TEXT\n"
			    << "call cprintl\n"
				<< "push 0\n"
				<< "call ExitProcess\n"
				<< "notOverflow" << pointsIterator << ": \t;assigment to STR begin\n\n";
			break;
		}

		case flagForCprint::cprintl:
		{
		  *file << "\npush offset " << cprintl_var << "\t;assigment to STR begin\n"
				<< "call concat" << paramsCounterForConcating << "\n"
				<< "cmp " << cprintl_var << ", 0\n"
				<< "jne notOverflow" << pointsIterator << "\n"
			    << "push offset STROVERFLOW_TEXT\n"
			    << "call cprintl\n"
				<< "push 0\n"
				<< "call ExitProcess\n"
				<< "notOverflow" << pointsIterator << ": \t;assigment to STR begin\n\n";
			break;
		}
		default: break;
	}
	pointsIterator++;
	currentCheckedPos = i;
	LexTable.table[i].lexema;

	*file << "\n;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end\n\n";
}
