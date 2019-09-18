#pragma once

#define ID_MAXSIZE		10				//максимальное количество сиволов в идентификаторе
#define TI_MAXSIZE		1024			// максимальное количество эл-ов в таблице идентификаторов 
#define TI_INT_DEFAULT	0x00000000		// значение по умолчанию дл€ типа integer 
#define TI_STR_DEFAULT	0x00			// значение по умолчанию дл€ типа string 
#define TI_NULLIDX		-1				// нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE	255

#define PARM_ID_DEFAULT_EXT L".id.txt" //дл€ файла с итогом лексического анализa(идентификаторы и литералы)

namespace IT	// таблица идентификатов
{
	enum IDDATATYPE {DEF = 0, INT = 1, STR = 2 };		// типы данных идентификатов: integer, string
	enum IDTYPE {D = 0, V = 1, F = 2, P = 3, L = 4};			// типы идентификаторов: переменна€, функци€, параметр, лексема


	struct Entry						// эл-т таблицы идентификаторов
	{
		char parrentFunc[ID_MAXSIZE];
		char id[ID_MAXSIZE];			// идентификатор (автоматически усекаетс€ до ID_MAXSIZE
		IDDATATYPE	iddatatype;			// тип данных
		IDTYPE	idtype;					// тип идентикатора
		union VALUE
		{
			int vint;					// значение integer
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
	
		IdTable();
		void Add(Entry entry);
		Entry GetEntry(int n);
		int IsId(const char id[ID_MAXSIZE]);
		int IsId(const char id[ID_MAXSIZE], const char parentFunc[ID_MAXSIZE]);
		int IsLit(const char lit[ID_MAXSIZE]);
		void PrintIdTable(const wchar_t* in);
		void Delete();
	};
}