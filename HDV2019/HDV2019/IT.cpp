#include "IT.h"
#include "lexAnaliz.h"
bool IT::IdTable::isLibraryIncluded = false;

IT::Entry::Entry()
{
	this->paramsCount = 0;
	this->parrentFunc[0] = '\0';
	this->id[0] = '\0';
	this->iddatatype = DEF;
	this->idtype = D;
}

IT::Entry::Entry(const char* parrentFunc, const char* id, IDDATATYPE iddatatype, IDTYPE idtype)
{
	this->paramsCount = 0;

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

	this->Add({ "global", "strlength", IT::IDDATATYPE::INT, IT::IDTYPE::F });
	this->table[this->current_size - 1].funcParams->AddParam("\0", IT::IDDATATYPE::STR, 0, this->table[this->current_size - 1].paramsCount);

	this->Add({ "global", "inttostr", IT::IDDATATYPE::STR, IT::IDTYPE::F });
	this->table[this->current_size - 1].funcParams->AddParam("\0", IT::IDDATATYPE::INT, 0, this->table[this->current_size - 1].paramsCount);

	this->Add({ "global", "strtoint", IT::IDDATATYPE::INT, IT::IDTYPE::F });
	this->table[this->current_size - 1].funcParams->AddParam("\0", IT::IDDATATYPE::STR, 0, this->table[this->current_size - 1].paramsCount);

	this->Add({ "global", "random", IT::IDDATATYPE::INT, IT::IDTYPE::F });
	this->table[this->current_size - 1].funcParams->AddParam("\0", IT::IDDATATYPE::INT, 0, this->table[this->current_size - 1].paramsCount);
	this->table[this->current_size - 1].funcParams->AddParam("\0", IT::IDDATATYPE::INT, 0, this->table[this->current_size - 1].paramsCount);
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
			if (this->table[i].idtype != IT::IDTYPE::S)
				return i;
	}
	return TI_NULLIDX;
}

int IT::IdTable::IsId(const char* id, const char* parentFunc)
{
	for (int i = 0; i < this->current_size; i++)
	{
		if ((strcmp(this->table[i].id, id) == 0) && (strcmp(this->table[i].parrentFunc, parentFunc) == 0))
			if(this->table[i].idtype != IT::IDTYPE::S)
				return i;
	}
	return TI_NULLIDX;
}

int IT::IdTable::IsLit(const char* lit)
{
	//для строк
	char* temp = new char[TI_STR_MAXSIZE];
	int j = 0;
	for (int i = 1; lit[i] != '\0'; i++, j++)
		temp[j] = lit[i];

	if(j>0) temp[j-1] = '\0';
	//

	for (int i = 0; i < this->current_size; i++)
	{
		if (this->table[i].idtype == IT::IDTYPE::L)
		{
			if (this->table[i].value.vint == atoi(lit) && lit[0] != '\"')
			{
				delete[] temp;
				return i;
			}
			
			if (lit[0] == '\"' && strcmp(this->table[i].value.vstr.str, temp) == 0)
			{
				delete[] temp;
				return i;
			}
		}
	}

	//для чисел в отличной от 10ой сс
	int tempLit = 0;
	switch (lit[0])
	{
		case 'b': tempLit = fromBaseTo10(lit, 2);  break;
		case 'e': tempLit = fromBaseTo10(lit, 8);  break;
		case 'h': tempLit = fromBaseTo10(lit, 16); break;
		default:
		{
			delete[] temp;
			return TI_NULLIDX;
		}
	}

	for (int i = 0; i < this->current_size; i++)
		if (this->table[i].idtype == IT::IDTYPE::L)
			if (this->table[i].value.vint == tempLit)
			{
				delete[] temp;
				return i;
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
		char buf[50]{};

		*(idStream) << "=========================================================================================" << std::endl;
		*(idStream) << "| Функции                                                                               |" << std::endl;
		*(idStream) << "=========================================================================================" << std::endl;
		*(idStream) << '|' << std::setw(20) << "Идентификатор: " << '|' << std::setw(30) << "Тип возвращаемого значения: " << '|' << std::setw(35) << "Параметры    " << '|' << std::endl;
		*(idStream) << "=========================================================================================" << std::endl;

		for (int i = 0; i < this->current_size; i++)
		{
			if (this->table[i].idtype == IT::IDTYPE::F)
			{
				strcpy_s(buf, "");
				for (int j = 0; j + 1 <= TI_MAXPARAM_COUNT && this->table[i].funcParams[j].type != IT::IDDATATYPE::DEF; j++)
				{
					strcat_s(buf, this->table[i].funcParams[j].id);
					strcat_s(buf, ":");
					switch (this->table[i].funcParams[j].type)
					{
						case IT::IDDATATYPE::INT:	strcat_s(buf, "INT");	break;
						case IT::IDDATATYPE::STR:	strcat_s(buf, "STR");	break;
						case IT::IDDATATYPE::UBYTE: strcat_s(buf, "UBYTE"); break;
						case IT::IDDATATYPE::LOGIC: strcat_s(buf, "LOGIC"); break;
					}

					if (this->table[i].funcParams[j + 1].type != IT::IDDATATYPE::DEF)
						strcat_s(buf, ", ");
				}
				strcat_s(buf, "  ");

				if (flagForFirst)
					*(idStream) << "----------------------------------------------------------------------------------------" << std::endl;

				switch (this->table[i].iddatatype)
				{
					case IT::IDDATATYPE::INT:
					{
						*(idStream) << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(30) << "INT " << '|' << std::setw(35) << buf << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::STR:
					{
						*(idStream) << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(30) << "STR " << '|' << std::setw(35) << buf << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::UBYTE:
					{
						*(idStream) << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(30) << "UBYTE " << '|' << std::setw(35) << buf << '|' << std::endl;
						break;
					}
					case IT::IDDATATYPE::LOGIC:
					{
						*(idStream) << '|' << std::setw(20) << this->table[i].id << '|' << std::setw(30) << "LOGIC " << '|' << std::setw(35) << buf << '|' << std::endl;
						break;
					}
				}

				flagForFirst = true;
			}


		}

		*(idStream) << "=========================================================================================" << std::endl;
		
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