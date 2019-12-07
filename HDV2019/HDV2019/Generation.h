#pragma once
#include "LT.h"
#include "IT.h"
#include "MFST.h"

#include <stack>

//�������:
// ����� �������� � ���������� ��� 'L' ��� 'V' (��������������) + ����� � idTable
// ����� ���������� ������� ��� ������ �� �����

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
					   << "includelib ..\\DateTimelib.lib\n"													\
					   << "getDate PROTO	; ���������� �������, ��������� ���� � ���� ������\n"									\
					   << "getTime PROTO	; ���������� �������, ��������� ����� � ���� ������\n"								\
					   << ";------------------------------\n\n";													\

#define STACK(value) ".stack " << value << "\n\n";

#define LITERALS_CONSTANTS_begin ";-----------Literals and constants-----------\n"\
							  << ".const\n\n";
#define DEFAULT_CONSTANTS		 "consoleTitle byte 'HDV2019', 0\n\n";
#define LITERALS_CONSTANTS_end	 ";--------------------------------------------\n"; 

#define DATA_BLOCK "\n\n.data\n\n";

#define VARIABLES_begin "\n;----------------Variables-------------------\n\n";
#define DEFAULT_VARS "consoleHandle dd 0h\t; ��������� �������\n\n";
#define VARIABLES_end	";---------------------------------------------\n";

#define CODE_BLOCK "\n\n.code\n\n";

#define FUNCTIONS_begin "\n;----------------Functions-------------------\n\n";
#define FUNCTIONS_end	";----------------------------------------------\n";


void funcSignature(std::ofstream* file, IT::IdTable& IdTable, int i);
void generation(LT::LexTable& LexTable, IT::IdTable& IdTable, std::stack<MFST::MFSTState>& storestate);