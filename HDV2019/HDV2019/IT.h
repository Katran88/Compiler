#pragma once

#include "Error.h"
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>

#define ID_MAXSIZE			25				//максимальное количество сиволов в идентификаторе
#define TI_MAXSIZE			1024			// максимальное количество эл-ов в таблице идентификаторов
#define TI_MAXPARAM_COUNT	5				//максимальное количество параметров
#define TI_INT_DEFAULT		0				// значение по умолчанию дл€ типа integer
#define TI_UBYTE_DEFAULT	0				// значение по умолчанию дл€ типа ubyte
#define TI_LOGIC_DEFAULT	0				// значение по умолчанию дл€ типа logic
#define TI_STR_DEFAULT		0				// значение по умолчанию дл€ типа string 
#define TI_NULLIDX			-1				// нет элемента таблицы идентификаторов

#define TI_STR_MAXSIZE		100				//максимальна€ длина строки
#define TI_INT_MAX_VALUE	pow(2,16)-1		//максимальное значение дл€ int
#define TI_INT_MIN_VALUE	-pow(2,16)		//минимальное значение дл€ int
#define TI_UBYTE_MAX_VALUE	pow(2,8)-1		//максимальное значение дл€ ubyte
#define TI_UBYTE_MIN_VALUE	0				//минимальное значение дл€ ubyte

#define PARM_ID_DEFAULT_EXT L".id.txt" //дл€ файла с итогом лексического анализa(идентификаторы и литералы)

namespace IT	// таблица идентификатов
{
	enum IDDATATYPE {DEF, INT, STR, UBYTE, LOGIC };		// типы данных идентификатов
	enum IDTYPE {D, V, F, P, L, S};			// типы идентификаторов: переменна€, функци€, параметр, литерал, зарезервированный

	struct Entry						// эл-т таблицы идентификаторов
	{
		char parrentFunc[ID_MAXSIZE];
		char id[ID_MAXSIZE];			// идентификатор (автоматически усекаетс€ до ID_MAXSIZE
		IDDATATYPE	iddatatype;			// тип данных
		IDTYPE	idtype;					// тип идентикатора

		int paramsCount;
		struct FuncParams
		{
			IDDATATYPE type = DEF;
			char id[ID_MAXSIZE]{};

			void AddParam(const char* id, IDDATATYPE type, const int strNum, int& paramsCount)
			{
				if (paramsCount < TI_MAXPARAM_COUNT)
				{
					strcpy_s(this[paramsCount].id, id);
					this[paramsCount].type = type;
					paramsCount++;
				}
				else
					throw ERROR_THROW_IN(403, strNum, -1);
			}
		}funcParams[TI_MAXPARAM_COUNT];

		union VALUE
		{
			int vint;					// значение integer/ubyte
			bool vlogic;				// значение logic
			struct
			{
				unsigned char len;				// количесво символов в string
				char str[TI_STR_MAXSIZE]; // символы string
			} vstr;	// значение string
		}value;	// значение идентификатора

		Entry();
		Entry(const char* parrentFunc, const char* id, IDDATATYPE iddatatype, IDTYPE idtype);
	};

	struct IdTable // экземпл€р таблицы идентификаторов
	{
		int maxsize;				// емкость таблицы идентификаторов < TI_MAXSIZE
		int current_size;			// текущий размер таблицы идентификаторов < maxsize
		Entry* table;				// массив строк таблицы идентификаторов

		static bool isLibraryIncluded; //флаг дл€ подключени€ библиотеки в asm file 

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