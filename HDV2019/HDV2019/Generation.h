#pragma once
#include "LT.h"
#include "IT.h"

#define STANDART_BEGIN    ".586							; ������� ������ (��������� Pentium)\n"											\
					   << ".model flat, stdcall			; ������ ������, ���������� � �������\n"										\
					   << "includelib kernel32.lib			; ������������: ����������� � kernel32\n\n"									\
					   << "ExitProcess PROTO: DWORD		; �������� ������� ��� ���������� �������� Windows\n\n"							\
					   << ";-----------console-----------\n"																			\
					   << "GetStdHandle PROTO: DWORD	; �������� handle ������ �� �������\n"											\
					   << "WriteConsoleA PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD		;����� �� �������(����������� � - ��)\n"\
					   << "SetConsoleTitleA PROTO: DWORD		; �������� �-�� ��������������� ��������� ����������� ����\n"			\
					   << ";-----------------------------\n\n";																			\

#define LIB_INCLUDE	      ";-----------DateTime-----------\n"																			\
					   << "includelib ..\\Debug\\DateTimelib.lib\n"													\
					   << "getDate PROTO	; ���������� �������, ��������� ���� � ���� ������\n"									\
					   << "getTime PROTO	; ���������� �������, ��������� ����� � ���� ������\n"								\
					   << ";------------------------------\n\n";													\

#define STACK(value) ".stack " << value << "\n\n";

#define LITERALS_CONSTANTS_begin ";-----------Literals and constants-----------\n"\
							  << ".const\n\n";

#define DEFAULT_CONSTANTS "consoleTitle byte 'HDV2019', 0\n\n";

#define LITERALS_CONSTANTS_end	 ";--------------------------------------------\n"; 
void generation(LT::LexTable& LexTable, IT::IdTable& IdTable);