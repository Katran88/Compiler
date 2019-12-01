#pragma once

#include <iostream>
#include <fstream>
#include <stack>
#include "Error.h"
#include "FST.h"
#include "In.h"
#include "LT.h"
#include "IT.h"

#pragma warning(disable: 4996)

#define keyTokens_size 34
#define libName "DateTime"
struct KeyTokens
{
	char keyToken[10]{};
	char lex = '/0';
};

struct ParrentBlock
{
	IT::IDDATATYPE returnType;
	char* name;

	ParrentBlock()
	{
		this->returnType = IT::IDDATATYPE::DEF;
		this->name = new char[ID_MAXSIZE+3]{}; //+ 1 для \0 и +2 для нумерации блоков
		strcpy(this->name, "global");
	}

	ParrentBlock(const char* inputName, IT::IDDATATYPE returnType)
	{
		this->returnType = returnType;
		this->name = new char[ID_MAXSIZE + 3]{}; //+ 1 для \0 и +2 для нумерации блоков
		strcpy(this->name, inputName);
	}
};

int fromBaseTo10(const char* str, int base);

int searchingForIDinStack(IT::IdTable& idTable, std::stack<ParrentBlock>& stack, const char* token);
void lexAnaliz(const In::IN& source, LT::LexTable& lexTable, IT::IdTable& idTable);
bool tokenAnaliz(const char* token, const int strNumber, LT::LexTable& lexTable, IT::IdTable& idTable, const KeyTokens* keyTokens);