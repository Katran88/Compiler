#include "Generation.h"

void generation(LT::LexTable& LexTable, IT::IdTable& IdTable, std::stack<MFST::MFSTState>& storestate)
{
	std::ofstream* file = new std::ofstream("..\\HDV2019_asm\\HDV2019.asm");
	if (file->is_open())
	{
		*file << STANDART_BEGIN;

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
		*file << DEFAULT_VARS;
		for (int i = 0; i < IdTable.current_size; i++)
		{
			if (IdTable.table[i].idtype == IT::IDTYPE::V)
				switch (IdTable.table[i].iddatatype)
				{
					case IT::IDDATATYPE::INT:  *file << 'V' << i << "\tsdword\t" << IdTable.table[i].value.vint << "\t ; integer var" << '\n'; break;

					case IT::IDDATATYPE::LOGIC: *file << 'V' << i << "\tbyte\t" << IdTable.table[i].value.vlogic << "\t ; logic var" << '\n'; break;

					case IT::IDDATATYPE::STR: *file << 'V' << i << "\tbyte\t" << TI_STR_MAXSIZE << " dup(0)" << "\t ; string var" << '\n'; break;

					case IT::IDDATATYPE::UBYTE: *file << 'V' << i << "\tbyte\t" << IdTable.table[i].value.vint << "\t ; ubyte var" << '\n'; break;
				}
		}
		*file << VARIABLES_end
		
		*file << CODE_BLOCK;

		*file << FUNCTIONS_begin;

		bool flagForFuncBlockFrame = false;
		int currentCheckedPos = 0;
		//генереция кода связана с номерами правила и цепочки в грейбах(изменять аккуратно!)
		for(auto& state : storestate._Get_container())
		{	//Предотвращение повторного разбора правил
			if (currentCheckedPos > state.posInLent)
				continue;
			
			//'шапка' функции
			if (state.nRule == 0 && (state.nRuleChain == 3 || state.nRuleChain == 4))
			{
				funcSignature(file, IdTable, LexTable.table[state.posInLent + 2].idxTI);
				flagForFuncBlockFrame = true;

				if (state.nRuleChain == 3)
					currentCheckedPos = state.posInLent + 6;
				if (state.nRuleChain == 4)
					currentCheckedPos = state.posInLent + 5;
			}
			//тело функции
			if (state.nRule == 1)
			{
				//присваивание какому-то идентификатору
#pragma	region assigment

				if (state.nRuleChain >= 2 && state.nRuleChain <= 5)
				{
					//для правильного просмотра типа переменной, которой будет все присваиваться
					int offsetFromBegin = 0;
					if (state.nRuleChain == 2 || state.nRuleChain == 3)
						offsetFromBegin = 1;
					if (state.nRuleChain == 4 || state.nRuleChain == 5)
						offsetFromBegin = 1;

					int idForRET = state.posInLent + offsetFromBegin; //номер идентификатора в таблице лексем кому присваивать все это
					int i = idForRET + 2; // пропускаем '=' и становимся на 1й эл-т
					int paramsCounterForConcating = 0; // счетчик стринговых переменных для вызова функции для их конкатонации
					
					for (; LexTable.table[i].lexema != LEX_SEMICOLON; i++)
					{
						if (LexTable.table[i].idxTI != -1)
						{
							switch (IdTable.table[LexTable.table[i].idxTI].iddatatype)
							{
								case IT::IDDATATYPE::UBYTE:
								case IT::IDDATATYPE::LOGIC:
								{
									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::V)
										*file << "\n movzx eax, " << 'V' << LexTable.table[i].idxTI
											  << "\n push eax\n";

									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::L)
										* file << "\n movzx eax, " << 'L' << LexTable.table[i].idxTI
											   << "\n push eax\n";

									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::F)
										* file << "\n call " << IdTable.table[LexTable.table[i].idxTI].id << "\n"
											   << "\n push eax\n";
									break;
								}

								case IT::IDDATATYPE::INT:
								{
									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::V)
										* file << "\n mov eax, " << 'V' << LexTable.table[i].idxTI
											   << "\n push eax\n";

									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::L)
										* file << "\n mov eax, " << 'L' << LexTable.table[i].idxTI
											   << "\n push eax\n";

									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::F)
										* file << "\n call " << IdTable.table[LexTable.table[i].idxTI].id << "\n"
											   << "\n push eax\n";
									break;
								}

								case IT::IDDATATYPE::STR:
								{
									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::V)
										* file << "\n push offset " << 'V' << LexTable.table[i].idxTI << "\n";

									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::L)
										* file << "\n push offset " << 'L' << LexTable.table[i].idxTI << "\n";

									if (IdTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::F)
										* file << "\n call " << IdTable.table[LexTable.table[i].idxTI].id << "\n"
											   << "\n push eax\n";
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
											*file << "\npop ebx" << "; \t+"
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
									*file << "\npop ebx" << "; \t-"
										  << "pop eax\n"
										  << "sub eax, ebx\n"
										  << "push eax\n";
									break;
								}
								case LEX_STAR:
								{//INT, UBYTE
									*file << "\npop ebx" << "; \t*"
										  << "pop eax\n"
										  << "imul eax, ebx\n"
										  << "push eax\n";
									break;
								}
								case LEX_DIRSLASH:
								{//INT, UBYTE
									*file << "\npop ebx" << "; \t/"
										  << "pop eax\n"
										  << "cdq\n"
										  << "idiv ebx\n"
										  << "push eax\n";
									break;
								}
								case LEX_OR_SIGN:
								{//INT, UBYTE, LOGIC
									*file << "\npop ebx" << "; \t|"
										  << "pop eax\n"
										  << "or eax, ebx\n"
										  << "push eax\n";
									break;
								}
								case LEX_AND_SIGN:
								{//INT, UBYTE, LOGIC
									*file << "\npop ebx" << "; \t&"
										  << "pop eax\n"
										  << "or eax, ebx\n"
										  << "push eax\n";
									break;
								}
								case LEX_NOT_SIGN:
								{
									*file << "\npop ebx" << "; \t!"
										  << "not eax\n"
										  << "push eax\n";
									break;
								}
								default: break;
							}
						}
					}

					switch (IdTable.table[idForRET].iddatatype)
					{
						case IT::IDDATATYPE::INT:
						case IT::IDDATATYPE::LOGIC:
						case IT::IDDATATYPE::STR:
						case IT::IDDATATYPE::UBYTE:
					}
					currentCheckedPos = i;
				}

#pragma endregion
			}

			//конец функции
			if (flagForFuncBlockFrame)
			{
				*file << "\nret\n" << IdTable.table[LexTable.table[state.posInLent + 2].idxTI].id << " endp\n\n";
				flagForFuncBlockFrame = false;
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
			case IT::IDDATATYPE::INT:	*file << IdTable.table[i].funcParams[j].id << " : " << "sdword";  break;

			case IT::IDDATATYPE::LOGIC: *file << IdTable.table[i].funcParams[j].id << " : " << "byte";  break;

			case IT::IDDATATYPE::STR:	*file << IdTable.table[i].funcParams[j].id << " : " << "dword";  break;

			case IT::IDDATATYPE::UBYTE:	*file << IdTable.table[i].funcParams[j].id << " : " << "byte";  break;
			}
			if (j != TI_MAXPARAM_COUNT && IdTable.table[i].funcParams[j + 1].type != IT::IDDATATYPE::DEF)
				* file << ", ";
		}
		*file << "\n\n";
	}
}
