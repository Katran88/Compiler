#include "Generation.h"

void generation(LT::LexTable& LexTable, IT::IdTable& IdTable)
{
	std::ofstream* file = new std::ofstream("..\\HDV2019_asm\\HDV2019.asm");
	if (file->is_open())
	{
		*file << STANDART_BEGIN

		if (IT::IdTable::isLibraryIncluded)
			*file << LIB_INCLUDE

		*file << STACK(4096)

		*file << LITERALS_CONSTANTS_begin
		*file << DEFAULT_CONSTANTS
			for (int i = 0; i < IdTable.current_size; i++)
			{
				if(IdTable.table[i].idtype == IT::IDTYPE::L)
					switch(IdTable.table[i].iddatatype)
					{
						case IT::IDDATATYPE::INT:  *file	<<	'L'	<< i << "\tdword\t"  << IdTable.table[i].value.vint << "\t ; integer literal" << '\n'; break;
					
						case IT::IDDATATYPE::LOGIC: *file	<<	'L'	<< i << "\tbyte\t"   << IdTable.table[i].value.vlogic << "\t ; logic literal" << '\n'; break;

						case IT::IDDATATYPE::STR:
						{
							*file << 'L' << i << "\tbyte\t\'";

							for( int j = 0; IdTable.table[i].value.vstr.str[j] != '\0'; j++)
								*file << IdTable.table[i].value.vstr.str[j];

							*file << "\', " << TI_STR_MAXSIZE - IdTable.table[i].value.vstr.len << " dup(0)" << "\t ; string literal" << '\n';
							break;
						}
						case IT::IDDATATYPE::UBYTE: *file	<<	'L'	<< i << "\tubyte\t" << IdTable.table[i].value.vint << "\t ; ubyte literal" << '\n'; break;
					}
			}
		*file << LITERALS_CONSTANTS_end
	}
	else
		throw ERROR_THROW(101);
}
