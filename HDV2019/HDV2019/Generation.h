#pragma once
#include "LT.h"
#include "IT.h"
#include "MFST.h"

#include <stack>

//памятка:
// имена констант и переменных это 'L' или 'V' (соответственно) + номер в idTable
// имена параметров функций это просто их имена

#define STANDART_BEGIN    ".586							; система команд (процессор Pentium)\n"											\
					   << ".model flat, stdcall			; модель памяти, соглашение о вызовах\n"										\
					   << "includelib kernel32.lib			; компановщику: компоновать с kernel32\n\n"									\
					   << "ExitProcess PROTO: DWORD		; прототип функции для завершения процесса Windows\n\n"							\
					   << ";-----------console-----------\n"																			\
					   << "GetStdHandle PROTO: DWORD	; получить handle вывода на консоль\n"											\
					   << "WriteConsoleA PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD		;вывод на консоль(стандартная ф - ия)\n"\
					   << "SetConsoleTitleA PROTO: DWORD		; прототип ф-ии устанавливающей заголовок консольного окна\n"			\
					   << ";-----------------------------\n\n";																			\

#define LIB_INCLUDE	      ";-----------DateTime-----------\n"																			\
					   << "includelib ..\\DateTimelib.lib\n"													\
					   << "getDate PROTO	; Возвращает текущую, локальную дату в виде строки\n"									\
					   << "getTime PROTO	; Возвращает текущее, локальную время в виде строки\n"								\
					   << ";------------------------------\n\n";													\

#define STACK(value) ".stack " << value << "\n\n";

#define LITERALS_CONSTANTS_begin ";-----------Literals and constants-----------\n"\
							  << ".const\n\n";
#define DEFAULT_CONSTANTS		 "consoleTitle byte 'HDV2019', 0\n\n";
#define LITERALS_CONSTANTS_end	 ";--------------------------------------------\n"; 

#define DATA_BLOCK "\n\n.data\n\n";

#define VARIABLES_begin "\n;----------------Variables-------------------\n\n";
#define DEFAULT_VARS "consoleHandle dd 0h\t; состояние консоли\n\n";
#define VARIABLES_end	";---------------------------------------------\n";

#define CODE_BLOCK "\n\n.code\n\n";

#define FUNCTIONS_begin "\n;----------------Functions-------------------\n\n";
#define FUNCTIONS_end	";----------------------------------------------\n";


void funcSignature(std::ofstream* file, IT::IdTable& IdTable, int i);
void generation(LT::LexTable& LexTable, IT::IdTable& IdTable, std::stack<MFST::MFSTState>& storestate);