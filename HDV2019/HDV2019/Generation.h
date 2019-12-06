#pragma once
#include "LT.h"
#include "IT.h"

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
					   << "includelib ..\\Debug\\DateTimelib.lib\n"													\
					   << "getDate PROTO	; Возвращает текущую, локальную дату в виде строки\n"									\
					   << "getTime PROTO	; Возвращает текущее, локальную время в виде строки\n"								\
					   << ";------------------------------\n\n";													\

#define STACK(value) ".stack " << value << "\n\n";

#define LITERALS_CONSTANTS_begin ";-----------Literals and constants-----------\n"\
							  << ".const\n\n";

#define DEFAULT_CONSTANTS "consoleTitle byte 'HDV2019', 0\n\n";

#define LITERALS_CONSTANTS_end	 ";--------------------------------------------\n"; 
void generation(LT::LexTable& LexTable, IT::IdTable& IdTable);