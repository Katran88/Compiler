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

		*file << DATA_BLOCK;

		*file << VARIABLES_begin;
		*file << DEFAULT_VARS;
		for (int i = 0; i < IdTable.current_size; i++)
		{
			if (IdTable.table[i].idtype == IT::IDTYPE::V)
				switch (IdTable.table[i].iddatatype)
				{
					case IT::IDDATATYPE::INT:  *file << 'V' << i << "\tsdword\t" << IdTable.table[i].value.vint << "\t ; integer var" << '\n'; break;

					case IT::IDDATATYPE::LOGIC: *file << 'V' << i << "\tbyte\t" << IdTable.table[i].value.vlogic << "\t ; logic var" << '\n'; break;

					case IT::IDDATATYPE::STR: *file << 'V' << i << "\tbyte\t" << IdTable.table[i].value.vstr.len << "dup(0)" << "\t ; string var" << '\n'; break;

					case IT::IDDATATYPE::UBYTE: *file << 'V' << i << "\tbyte\t" << IdTable.table[i].value.vint << "\t ; ubyte var" << '\n'; break;
				}
		}
		*file << VARIABLES_end

		*file << CODE_BLOCK;

		*file << FUNCTIONS_begin;

		//генереция кода связана с номером правила и цепочки в грейбах(изменять аккуратно!)
		bool flagForFuncBlockFrame = false;
		for(auto& state : storestate._Get_container())
		{
			if (state.nRule == 0 && (state.nRuleChain == 3 || state.nRuleChain == 4))
			{
				funcSignature(file, IdTable, LexTable.table[state.posInLent + 2].idxTI);
				flagForFuncBlockFrame = true;
				continue;
			}
			//тело функции
			if (state.nRule == 1)
			{
				if (state.nRuleChain >= 2 && state.nRuleChain <= 5)
				{

				}
			}

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
