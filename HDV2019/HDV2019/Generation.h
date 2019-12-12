#pragma once
#include "LT.h"
#include "IT.h"
#include "MFST.h"

#include <stack>

//памятка:
// имена констант и литералов это 'L' + номер в idTable
// имена переменных это родительскийБлок + '_' + имя дентификатора
// переменные для возвращаемого значения каждой вызываемой функции inttostr, getTime, getDate имя функции + "_" + id в lexTable

#define ifFlag "ifFlag" //флаг для входа в блок(если 1, то входим, если 0, то нет) записана в DEFAULT_VARS

#define cprint_var "cprint_var"
#define cprintl_var "cprintl_var"

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

#define STANDART_LIB	  ";-----------Standart functions-----------\n"					\
					   << "includelib ..\\Standart.lib\n"								\
					   << "concat1 PROTO : DWORD, : DWORD\n"							\
					   << "concat2 PROTO : DWORD, : DWORD, : DWORD\n"					\
					   << "concat3 PROTO : DWORD, : DWORD, : DWORD, : DWORD\n"			\
					   << "concat4 PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD\n" \
					   << "cprint PROTO : DWORD\n"										\
					   << "cprintl PROTO : DWORD\n"										\
					   << "strlength PROTO : DWORD\n"									\
					   << "inttostr PROTO : DWORD , : SDWORD\n"							\
					   << ";----------------------------------------\n\n";				\

#define LIB_INCLUDE	      ";-----------DateTime-----------\n"											\
					   << "includelib ..\\DateTimelib.lib\n"											\
					   << "getDate PROTO : DWORD	; Возвращает текущую, локальную дату в виде строки\n"		\
					   << "getTime PROTO : DWORD	; Возвращает текущее, локальную время в виде строки\n"		\
					   << ";------------------------------\n\n";										\

#define STACK(value) ".stack " << value << "\n\n";

#define LITERALS_CONSTANTS_begin ";-----------Literals and constants-----------\n"\
							  << ".const\n\n";

#define DEFAULT_CONSTANTS		 "consoleTitle byte 'HDV2019', 0\n"								 \
							  << "INTOVERFLOW_text byte 'INT OVERFLOW EXCEPTION', 10, 0\n"		 \
							  << "UBYTEOVERFLOW_text byte 'UBYTE OVERFLOW EXCEPTION', 10, 0\n"	 \
							  << "DIVISION_BY_ZERO_text byte 'DIVISION BY ZERO', 10, 0\n\n"		 \
							  << "STROVERFLOW_TEXT byte 'STR OVERFLOW', 0\n\n";					 \

#define LITERALS_CONSTANTS_end	 ";--------------------------------------------\n"; 

#define VARIABLES_begin "\n\n;----------------Variables-------------------\n.data\n\n";
#define DEFAULT_VARS	ifFlag << " byte 0 \t; флаг для входа в блок if\n"								  \
					 << cprint_var << " byte "<<  TI_STR_MAXSIZE <<" dup(0)\t; variable for cprint\n"    \
					 << cprintl_var << " byte "<<  TI_STR_MAXSIZE <<" dup(0)\t; variable for cprintl\n";  \

#define VARIABLES_end	";---------------------------------------------\n";

#define CODE_BLOCK "\n\n.code\n\n";

#define FUNCTIONS_begin "\n;----------------Functions-------------------\n\n";
#define FUNCTIONS_end	";----------------------------------------------\n";

enum loopFlag { undef, incFlag, decrFlag };
enum flagForCprint {none, cprint, cprintl};
struct innerBlock
{
	char* blockName;
	loopFlag loopflag;
	int idOfLoopIterator; // для доступа к итератору цикла (id в лекс табл)
	innerBlock(char* blockName, loopFlag loopflag, int idOfLoopIterator)
	{
		this->loopflag = loopflag;
		this->blockName = blockName;
		this->idOfLoopIterator = idOfLoopIterator;
	}
};


void funcSignature(std::ofstream* file, IT::IdTable& IdTable, int i);
void generation(LT::LexTable& LexTable, IT::IdTable& IdTable, std::stack<MFST::MFSTState>& storestate);