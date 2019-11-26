#include "IT.h"
#include "Error.h"
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>

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

		if (entry.iddatatype == IT::IDDATATYPE::UBYTE)
			this->table[current_size].value.vint = TI_UBYTE_DEFAULT;

		if (entry.iddatatype == IT::IDDATATYPE::LOGIC)
			this->table[current_size].value.vlogic = TI_LOGIC_DEFAULT;

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

int IT::IdTable::IsId(const char* id)
{
	for (int i = 0; i < this->current_size; i++)
	{
		if (strcmp(this->table[i].id, id) == 0)
			return i;
	}
	return TI_NULLIDX;
}

int IT::IdTable::IsId(const char* id, const char* parentFunc)
{
	for (int i = 0; i < this->current_size; i++)
	{
		if ((strcmp(this->table[i].id, id) == 0) &&
			(strcmp(this->table[i].parrentFunc, parentFunc) == 0))
			return i;
	}
	return TI_NULLIDX;
}

int IT::IdTable::IsLit(const char* lit)
{
	char* temp = new char[TI_STR_MAXSIZE];
	int j = 0;
	for (int i = 0; lit[i] != '\0'; i++)
	{
		if (lit[i] != '\"')
		{
			temp[j] = lit[i];
			j++;
		}
	}
	temp[j] = '\0';

	for (int i = 0; i < this->current_size; i++)
	{
		if (this->table[i].idtype == IT::IDTYPE::L)
		{
			if (this->table[i].value.vint == atoi(lit))
			{
				delete[] temp;
				return i;
			}
			
			if (strcmp(this->table[i].value.vstr.str, temp) == 0)
			{
				delete[] temp;
				return i;
			}
		}
	}

	delete[] temp;
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
		bool flagForFirst = false;

		#pragma region Literals

		*(idStream) << "====================================================================================" << std::endl;
		*(idStream) << "| Литералы                                                                         |" << std::endl;
		*(idStream) << "====================================================================================" << std::endl;
		*(idStream) << '|' << std::setw(15) << "Тип данных: " << "|" << std::setw(50) << "Значение: " << '|' << std::setw(15) << "Длина строки: " << '|' << std::endl;
		*(idStream) << "====================================================================================" << std::endl;

		for (int i = 0; i < this->current_size; i++)
		{
			if (this->table[i].idtype == IT::IDTYPE::L)
			{
				if (flagForFirst)
					* (idStream) << "------------------------------------------------------------------------------------" << std::endl;

				switch (this->table[i].iddatatype)
				{
					case IT::IDDATATYPE::INT:
					{
						*(idStream) << '|' << std::setw(15) << "INT  " << '|' << std::setw(50) << this->table[i].value.vint << '|' << std::setw(15) << "-" << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::STR:
					{
						*(idStream) << '|' << std::setw(15) << "STR  " << '|' << std::setw(50) << this->table[i].value.vstr.str << '|' << std::setw(15) << (int)this->table[i].value.vstr.len << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::UBYTE:
					{
						*(idStream) << '|' << std::setw(15) << "UBYTE  " << '|' << std::setw(50) << this->table[i].value.vint << '|' << std::setw(15) << "-" << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::LOGIC:
					{
						*(idStream) << '|' << std::setw(15) << "LOGIC  " << '|' << std::setw(50) << this->table[i].value.vlogic << '|' << std::setw(15) << "-" << '|' << std::endl;
						break;
					}
				}

				flagForFirst = true;
			}
		}

		*(idStream) << "====================================================================================" << std::endl;


		#pragma endregion
		*(idStream) << "\n\n\n";
		#pragma region Functions

		flagForFirst = false;

		*(idStream) << "================================================" << std::endl;
		*(idStream) << "| Функции                                      |" << std::endl;
		*(idStream) << "================================================" << std::endl;
		*(idStream) << '|' << std::setw(20) << "Идентификатор: " << '|' << std::setw(25) << "Тип данных возврата: " << '|'<< std::endl;
		*(idStream) << "================================================" << std::endl;

		for (int i = 0; i < this->current_size; i++)
		{
			if (this->table[i].idtype == IT::IDTYPE::F)
			{
				if (flagForFirst)
					*(idStream) << "------------------------------------------------" << std::endl;

				switch (this->table[i].iddatatype)
				{
					case IT::IDDATATYPE::INT:
					{
						*(idStream) << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(25) << "INT " << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::STR:
					{
						*(idStream) << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(25) << "STR " << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::UBYTE:
					{
						*(idStream) << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(25) << "UBYTE " << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::LOGIC:
					{
						*(idStream) << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(25) << "LOGIC " << '|' << std::endl;
						break;
					}
				}

				flagForFirst = true;
			}


		}

		*(idStream) << "================================================" << std::endl;
		
		#pragma endregion
		*(idStream) << "\n\n\n";
		#pragma region Variables

		flagForFirst = false;

		*(idStream) << "==================================================================================================================================================================" << std::endl;
		*(idStream) << "| Переменные                                                                                                                                                     |" << std::endl;
		*(idStream) << "==================================================================================================================================================================" << std::endl;
		*(idStream) << '|' << std::setw(30) << "Имя родительского блока: " << '|' << std::setw(20) << "Идентификатор: " << '|' << std::setw(15) << "Тип данных: " << '|' << std::setw(25) << "Тип идентификатора: " << '|' << std::setw(50) << "Значение: " << '|' << std::setw(15) << "Длина строки: " << '|' << std::endl;
		*(idStream) << "==================================================================================================================================================================" << std::endl;

		for (int i = 0; i < this->current_size; i++)
		{
			if (this->table[i].idtype == IT::IDTYPE::V)
			{
				if (flagForFirst)
					* (idStream) << "------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

				switch (this->table[i].iddatatype)
				{
					case IT::IDDATATYPE::INT:
					{
						*(idStream) << '|' << std::setw(30) << this->table[i].parrentFunc << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(15) << "INT " << '|' << std::setw(25) << "V  " << '|' << std::setw(50) << this->table[i].value.vint << '|' << std::setw(15) << "- " << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::STR:
					{
						*(idStream) << '|' << std::setw(30) << this->table[i].parrentFunc << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(15) << "STR " << '|' << std::setw(25) << "V  " << '|' << std::setw(50) << this->table[i].value.vstr.str << '|' << std::setw(15) << (int)this->table[i].value.vstr.len << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::UBYTE:
					{
						*(idStream) << '|' << std::setw(30) << this->table[i].parrentFunc << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(15) << "UBYTE " << '|' << std::setw(25) << "V  " << '|' << std::setw(50) << this->table[i].value.vint << '|' << std::setw(15) << "- " << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::LOGIC:
					{
						*(idStream) << '|' << std::setw(30) << this->table[i].parrentFunc << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(15) << "LOGIC " << '|' << std::setw(25) << "V  " << '|' << std::setw(50) << this->table[i].value.vlogic << '|' << std::setw(15) << "- " << '|' << std::endl;
						break;
					}
				}

				flagForFirst = true;
			}

			if (this->table[i].idtype == IT::IDTYPE::P)
			{
				if (flagForFirst)
					* (idStream) << "------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

				switch (this->table[i].iddatatype)
				{
					case IT::IDDATATYPE::INT:
					{
						*(idStream) << '|' << std::setw(30) << this->table[i].parrentFunc << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(15) << "INT " << '|' << std::setw(25) << "V  " << '|' << std::setw(50) << this->table[i].value.vint << '|' << std::setw(15) << "- " << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::STR:
					{
						*(idStream) << '|' << std::setw(30) << this->table[i].parrentFunc << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(15) << "STR " << '|' << std::setw(25) << "V  " << '|' << std::setw(50) << this->table[i].value.vstr.str << '|' << std::setw(15) << (int)this->table[i].value.vstr.len << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::UBYTE:
					{
						*(idStream) << '|' << std::setw(30) << this->table[i].parrentFunc << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(15) << "UBYTE " << '|' << std::setw(25) << "V  " << '|' << std::setw(50) << this->table[i].value.vint << '|' << std::setw(15) << "- " << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::LOGIC:
					{
						*(idStream) << '|' << std::setw(30) << this->table[i].parrentFunc << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(15) << "LOGIC " << '|' << std::setw(25) << "V  " << '|' << std::setw(50) << this->table[i].value.vlogic << '|' << std::setw(15) << "- " << '|' << std::endl;
						break;
					}
				}

				flagForFirst = true;
			}


		}

		*(idStream) << "==================================================================================================================================================================" << std::endl;

		#pragma endregion
		*(idStream) << "\n\n\n";
		
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
