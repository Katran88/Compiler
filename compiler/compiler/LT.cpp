#include "LT.h"
#include "Error.h"
#include <fstream>

LT::Entry::Entry()
{
	this->lexema = '\0';
	this->sn = LT_TI_NULLXDX;
	this->idxTI = LT_TI_NULLXDX;
}

LT::Entry::Entry(const char lex, int str_n, int idxTI)
{
	this->lexema = lex;
	this->sn = str_n;
	this->idxTI = idxTI;
}

LT::LexTable::LexTable()
{
	this->maxsize = LT_MAXSIZE;
	this->current_size = 0;
	this->table = new Entry[LT_MAXSIZE];
}

void LT::LexTable::Add(Entry entry)
{
	if(this->current_size < this->maxsize)
		this->table[this->current_size++] = entry;
	else
		throw ERROR_THROW(121);
}

LT::Entry LT::LexTable::GetEntry(int n)
{
	if (n < this->maxsize && n >= 0)
		return table[n];
}

void LT::LexTable::PrintLexTable(const wchar_t* in)
{
	int i = 0;
	wchar_t lex[300];
	for (; in[i] != '\0'; i++)
		lex[i] = in[i];
	lex[i] = '\0';
	wcscat_s(lex, PARM_LEX_DEFAULT_EXT);

	std::ofstream* lexStream = new std::ofstream;
	lexStream->open(lex);

	if (lexStream->is_open())
	{
		int currentString = 0;
		(*lexStream) << '\n' << currentString + 1 << '\t';
		for (int i = 0; i < this->current_size;)
		{
			if (currentString == this->table[i].sn)
				(*lexStream) << this->table[i++].lexema;
			else
			{
				(*lexStream) << '\n' << currentString++ + 2 << '\t';

				if (currentString == this->table[i].sn)
					(*lexStream) << this->table[i++].lexema;
			}

			
		}
	}
	else
		throw ERROR_THROW(125);
	lexStream->close();
	delete lexStream;
}

void LT::LexTable::Delete()
{
	delete[] this->table;
	this->table = nullptr;
}



