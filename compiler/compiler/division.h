#pragma once

#include <iostream>
#include <fstream>
#include <stack>
#include "Error.h"
#include "FST.h"
#include "In.h"
#include "LT.h"
#include "IT.h"


#define keyTokens_size 18

struct KeyTokens
{
	char keyToken[keyTokens_size]{};
	char lex = '/0';
};

enum TypeFlag
{
	def, integer, string
};

enum ParrentBlockFlag
{
	global, function
};

struct ParrentBlock
{
	char* name;
	ParrentBlockFlag parrentBlockFlag;

	ParrentBlock()
	{
		this->name = new char[ID_MAXSIZE+2]{};
		this->parrentBlockFlag = ParrentBlockFlag::global;
	}
};

void divisionIntoTokens(const In::IN& source, LT::LexTable& lexTable, IT::IdTable& idTable);
bool tokenAnaliz(const char* token, const int strNumber, LT::LexTable& lexTable, IT::IdTable& idTable, const KeyTokens* keyTokens);