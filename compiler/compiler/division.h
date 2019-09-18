#pragma once

#include <iostream>
#include <fstream>
#include "Error.h"
#include "FST.h"
#include "In.h"
#include "LT.h"
#include "IT.h"

#define keyTokens_size 20

struct KeyTokens
{
	char keyToken[keyTokens_size]{};
	char lex = '/0';
};

void divisionIntoTokens(const In::IN& source, LT::LexTable& lexTable, IT::IdTable& idTable);
bool tokenAnaliz(const char* token, const int strNumber, LT::LexTable& lexTable, IT::IdTable& idTable, const KeyTokens* keyTokens);