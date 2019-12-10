#pragma once
#include "LT.h"
#include "IT.h"
#include "MFST.h"

#include <stack>

//памятка:
// имена констант и литералов это 'L' + номер в idTable
// имена переменных это родительскийБлок + '_' + имяИдентификатора

#define STANDART_BEGIN    ".586							; система команд (процессор Pentium)\n"											\
					   << ".model flat, stdcall			; модель памяти, соглашение о вызовах\n"										\
					   << "includelib kernel32.lib			; компановщику: компоновать с kernel32\n"									\
					   << "includelib libucrt.lib			; для UCRT - universal C Run-Time library\n\n"								\
					   << "ExitProcess PROTO: DWORD		; прототип функции для завершения процесса Windows\n\n"							\
					   << ";-----------console-----------\n"																			\
					   << "GetStdHandle PROTO : DWORD	; получить handle вывода на консоль\n"											\
					   << "WriteConsoleA PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD		;вывод на консоль(стандартная ф - ия)\n"\
					   << "SetConsoleTitleA PROTO: DWORD		; прототип ф-ии устанавливающей заголовок консольного окна\n"			\
					   << ";-----------------------------\n\n";																			\

#define STANDART_LIB	  ";-----------Standart functions-----------\n"    \
					   << "includelib ..\\Standart.lib\n"				   \
					   << "concat PROTO: DWORD,: DWORD\n"				   \
					   << ";----------------------------------------\n\n"; \

#define LIB_INCLUDE	      ";-----------DateTime-----------\n"																			\
					   << "includelib ..\\DateTimelib.lib\n"													\
					   << "getDate PROTO	; Возвращает текущую, локальную дату в виде строки\n"									\
					   << "getTime PROTO	; Возвращает текущее, локальную время в виде строки\n"								\
					   << ";------------------------------\n\n";													\

#define STACK(value) ".stack " << value << "\n\n";

#define LITERALS_CONSTANTS_begin ";-----------Literals and constants-----------\n"\
							  << ".const\n\n";

#define DEFAULT_CONSTANTS		 "consoleTitle byte 'HDV2019', 0\n"								 \
							  << "INTOVERFLOW_text byte 'INT OVERFLOW EXCEPTION', 10, 0\n"		 \
							  << "UBYTEOVERFLOW_text byte 'UBYTE OVERFLOW EXCEPTION', 10, 0\n"	 \
							  << "DIVISION_BY_ZERO_text byte 'DIVISION BY ZERO', 10, 0\n\n";	 \

#define LITERALS_CONSTANTS_end	 ";--------------------------------------------\n"; 

#define VARIABLES_begin "\n\n;----------------Variables-------------------\n.data\n\n";
#define DEFAULT_VARS "consoleHandle dd 0h\t; состояние консоли\n\n"
#define VARIABLES_end	";---------------------------------------------\n";

#define CODE_BLOCK "\n\n.code\n\n";

#define FUNCTIONS_begin "\n;----------------Functions-------------------\n\n";
#define FUNCTIONS_end	";----------------------------------------------\n";

enum loopFlag { undef, incFlag, decrFlag};


void funcSignature(std::ofstream* file, IT::IdTable& IdTable, int i);
void generation(LT::LexTable& LexTable, IT::IdTable& IdTable, std::stack<MFST::MFSTState>& storestate);