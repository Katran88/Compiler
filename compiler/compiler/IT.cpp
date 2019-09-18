#include "IT.h"
#include "Error.h"
#include <string.h>
#include <stdlib.h>
#include <fstream>

IT::Entry::Entry()
{
	this->parrentFunc[0] = '\0';
	this->id[0] = '\0';
	this->iddatatype = DEF;
	this->idtype = D;
}

IT::Entry::Entry(const char* parrentFunc, const char* id, IDDATATYPE iddatatype, IDTYPE idtype)
{
	int i = 0;
	if(parrentFunc)
	for (i = 0; parrentFunc[i] != '\0'; i++)
		this->parrentFunc[i] = parrentFunc[i];
	this->parrentFunc[i] = '\0';
	i = 0;	
	if(id)
	for (i = 0; id[i] != '\0'; i++)
		this->id[i] = id[i];

	this->id[i] = '\0';
	this->iddatatype = iddatatype;
	this->idtype = idtype;
	
}

IT::IdTable::IdTable()
{
	this->maxsize = TI_MAXSIZE;
	this->current_size = 0;
	this->table = new Entry[TI_MAXSIZE];
}

void IT::IdTable::Add(Entry entry)
{
	if (strlen(entry.id) > ID_MAXSIZE)
		throw ERROR_THROW(128);

	if (this->current_size < this->maxsize)
	{
		this->table[current_size] = entry;

		if (entry.iddatatype == IT::IDDATATYPE::INT)
			this->table[current_size].value.vint = TI_INT_DEFAULT;

		if (entry.iddatatype == IT::IDDATATYPE::STR)
		{
			this->table[current_size].value.vstr.str[0] = TI_STR_DEFAULT;
			this->table[current_size].value.vstr.len = 0;
		}

		this->current_size++;
	}
	else
		throw ERROR_THROW(122);
}

IT::Entry IT::IdTable::GetEntry(int n)
{
	if(n < this->maxsize && n >= 0)
		return this->table[n];
}

int IT::IdTable::IsId(const char id[ID_MAXSIZE])
{
	for (int i = 0; i < this->current_size; i++)
	{
		if (strcmp(this->table[i].id, id) == 0)
			return i;
	}
	return TI_NULLIDX;
}

int IT::IdTable::IsId(const char id[ID_MAXSIZE], const char parentFunc[ID_MAXSIZE])
{
	for (int i = 0; i < this->current_size; i++)
	{
		if ((strcmp(this->table[i].id, id) == 0) &&
			(strcmp(this->table[i].parrentFunc, parentFunc) == 0))
			return i;
	}
	return TI_NULLIDX;
}

int IT::IdTable::IsLit(const char lit[ID_MAXSIZE])
{
	for (int i = 0; i < this->current_size; i++)
	{
		if (this->table[i].idtype == IT::IDTYPE::L)
		{
			if (this->table[i].value.vint == atoi(lit))
				return i;

			if (strcmp(this->table[i].value.vstr.str, lit) == 0)
				return i;
		}
	}
	return TI_NULLIDX;
}

void IT::IdTable::PrintIdTable(const wchar_t* in)
{
	int i = 0;
	wchar_t id[300];
	for (; in[i] != '\0'; i++)
		id[i] = in[i];
	id[i] = '\0';
	wcscat_s(id, PARM_ID_DEFAULT_EXT);

	std::ofstream* idStream = new std::ofstream;
	idStream->open(id);

	if (idStream->is_open())
	{
		for (int i = 0; i < this->current_size; i++)
		{
			if (this->table[i].idtype == IT::IDTYPE::L)
			{
				*(idStream) << std::endl << "----------------" << std::endl;
				*(idStream) << "Тип идентификатора: L" << std::endl;

				switch (this->table[i].iddatatype)
				{
					case 0: *(idStream) << "Тип данных идентификатора: DEF" << std::endl; break;
					case 1: 
					{
						*(idStream) << "Тип данных идентификатора: INT" << std::endl;
						*(idStream) << "Значение: " << this->table[i].value.vint << std::endl;
						break;
					}
					case 2:
					{
						*(idStream) << "Тип данных идентификатора: STR" << std::endl;
						*(idStream) << "Значение: \"" << this->table[i].value.vstr.str << "\"" << std::endl;
						*(idStream) << "Длина строки: " << (int)this->table[i].value.vstr.len << std::endl;
						break;
					}
				}

				*(idStream) << "----------------" << std::endl;
			}
			else
			{
				*(idStream) << std::endl << "----------------" << std::endl;
				switch (this->table[i].idtype)
				{
					case 0: *(idStream) << "Тип идентификатора: D" << std::endl; break;
					case 1: *(idStream) << "Тип идентификатора: V" << std::endl; break;
					case 2: *(idStream) << "Тип идентификатора: F" << std::endl; break;
					case 3: *(idStream) << "Тип идентификатора: P" << std::endl; break;
				}

				if(this->table[i].idtype != IT::IDTYPE::F)
					*(idStream) << "Принадлежит ф-ции: " << this->table[i].parrentFunc << std::endl;

				*(idStream) << "Имя идентификатора: " << this->table[i].id << std::endl;

				switch (this->table[i].iddatatype)
				{
				case 0: *(idStream) << "Тип данных идентификатора: DEF" << std::endl; break;
				case 1:
				{
					*(idStream) << "Тип данных идентификатора: INT" << std::endl;
					*(idStream) << "Значение: " << this->table[i].value.vint << std::endl;
					break;
				}
				case 2:
				{
					*(idStream) << "Тип данных идентификатора: STR" << std::endl;
					*(idStream) << "Значение: \"" << this->table[i].value.vstr.str << "\"" << std::endl;
					*(idStream) << "Длина строки: " << (int)this->table[i].value.vstr.len << std::endl;
					break;
				}
				}

				*(idStream) << "----------------" << std::endl;
			}
		}
	}
	else
		throw ERROR_THROW(125);
	idStream->close();
	delete idStream;
}

void IT::IdTable::Delete()
{
	delete[] this->table;
	this->table = nullptr;
}
