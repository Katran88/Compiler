#pragma once

#include "Error.h"
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>

#define ID_MAXSIZE			25				//������������ ���������� ������� � ��������������
#define TI_MAXSIZE			1024			// ������������ ���������� ��-�� � ������� ���������������
#define TI_MAXPARAM_COUNT	5				//������������ ���������� ����������
#define TI_INT_DEFAULT		0				// �������� �� ��������� ��� ���� integer
#define TI_UBYTE_DEFAULT	0				// �������� �� ��������� ��� ���� ubyte
#define TI_LOGIC_DEFAULT	0				// �������� �� ��������� ��� ���� logic
#define TI_STR_DEFAULT		0				// �������� �� ��������� ��� ���� string 
#define TI_NULLIDX			-1				// ��� �������� ������� ���������������
#define TI_STR_MAXSIZE		255

#define PARM_ID_DEFAULT_EXT L".id.txt" //��� ����� � ������ ������������ ������a(�������������� � ��������)

namespace IT	// ������� �������������
{
	enum IDDATATYPE {DEF, INT, STR, UBYTE, LOGIC };		// ���� ������ �������������
	enum IDTYPE {D, V, F, P, L, S};			// ���� ���������������: ����������, �������, ��������, �������, �����������������

	struct Entry						// ��-� ������� ���������������
	{
		char parrentFunc[ID_MAXSIZE];
		char id[ID_MAXSIZE];			// ������������� (������������� ��������� �� ID_MAXSIZE
		IDDATATYPE	iddatatype;			// ��� ������
		IDTYPE	idtype;					// ��� ������������

		struct FuncParams
		{
			static int currentCount;
			IDDATATYPE type = DEF;
			char id[ID_MAXSIZE]{};

			void AddParam(const char* id, IDDATATYPE type, const int strNum)
			{
				if (currentCount < TI_MAXPARAM_COUNT)
				{
					strcpy_s(this->id, id);
					this->type = type;
					currentCount++;
				}
				else
					throw ERROR_THROW_IN(403, strNum, -1);
			}
		}funcParams[TI_MAXPARAM_COUNT];

		union VALUE
		{
			int vint;					// �������� integer/ubyte
			bool vlogic;				// �������� logic
			struct
			{
				unsigned char len;				// ��������� �������� � string
				char str[TI_STR_MAXSIZE]; // ������� string
			} vstr;	// �������� string
		}value;	// �������� ��������������

		Entry();
		Entry(const char* parrentFunc, const char* id, IDDATATYPE iddatatype, IDTYPE idtype);
	};

	struct IdTable // ��������� ������� ���������������
	{
		int maxsize;				// ������� ������� ��������������� < TI_MAXSIZE
		int current_size;			// ������� ������ ������� ��������������� < maxsize
		Entry* table;				// ������ ����� ������� ���������������
	
		IdTable();
		void Add(Entry entry);
		Entry GetEntry(int n);
		int IsId(const char* id);
		int IsId(const char* id, const char* parentFunc);
		int IsLit(const char* lit);
		void PrintIdTable(const wchar_t* in);
		void Delete();
	};
}