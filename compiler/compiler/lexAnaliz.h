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

#define keyTokens_size 29

struct KeyTokens
{
	char keyToken[10]{};
	char lex = '/0';
};

enum TypeFlag
{
	def, integer, string, ubyte, logic
};

struct ParrentBlock
{
	char* name;

	ParrentBlock()
	{
		this->name = new char[ID_MAXSIZE+3]{}; //+ 1 для \0 и 2 для нумерации блоков
		strcpy(this->name, "global");
	}

	ParrentBlock(const char* inputName)
	{
		this->name = new char[ID_MAXSIZE + 3]{}; //+ 1 для \0 и 2 для нумерации блоков
		strcpy(this->name, inputName);
	}
};

int searchingForIDinStack(IT::IdTable& idTable, std::stack<ParrentBlock>& stack, const char* token);
void lexAnaliz(const In::IN& source, LT::LexTable& lexTable, IT::IdTable& idTable);
bool tokenAnaliz(const char* token, const int strNumber, LT::LexTable& lexTable, IT::IdTable& idTable, const KeyTokens* keyTokens);