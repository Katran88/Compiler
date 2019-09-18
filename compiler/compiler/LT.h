#pragma once

#define	LT_MAXSIZE		1024		// максимальное количество строк в таблице лексем	
#define	LT_TI_NULLXDX	-1			// нет элемента таблицы идентификаторов				
#define	LEX_INTEGER		'n'			// лексема для integer
#define	LEX_STRING		's'			// лексема для string
#define	LEX_ID			'i'			// лексема для идентификатора
#define	LEX_LITERAL		'l'			// лексема для литерала
#define	LEX_FUNCTION	'f'			// лексема для function
#define	LEX_DECLARE		'd'			// лексема для declare
#define	LEX_RETURN		'r'			// лексема для return
#define	LEX_PRINT		'p'			// лексема для print
#define	LEX_SEMICOLON	';'			// лексема для ;
#define	LEX_COMMA		','			// лексема для ,
#define	LEX_LEFTBRACE	'{'			// лексема для {
#define	LEX_RIGHTBRACE	'}'			// лексема для }
#define	LEX_LEFTHESIS	'('			// лексема для (
#define	LEX_RIGHTHESIS	')'			// лексема для )
#define	LEX_PLUS		'+'			// лексема для +
#define	LEX_MINUS		'-'			// лексема для -
#define	LEX_STAR		'*'			// лексема для *
#define	LEX_DIRSLASH	'/'			// лексема для /
#define	LEX_EQUAL_SIGN	'='			// лексема для =

#define PARM_LEX_DEFAULT_EXT L".lex.txt" //для файла с итогом лексического анализa
// таблица лексем
namespace LT
{
	struct Entry						// строка таблицы лексем
	{
		char lexema;					// лесема
		int sn;							// номер строки в исходном тексте
		int idxTI;						// индекс в таблице идентификаторов или LT_TI_NULLIDX
		Entry();
		Entry(const char lex, int str_n, int idxTI);
	};

	struct LexTable						// экземпляр таблицы лексем
	{
		int maxsize;					// емкость таблицы лексем < LT_MAXSIZE
		int current_size;				// текущий размер таблицы лексем < maxsize
		Entry* table;					// массив строк таблицы лексем

		LexTable();

		void Add(Entry entry);					// добавление лексем
		Entry GetEntry(int n);					// получить строку таблицы лексем
		void PrintLexTable(const wchar_t* in);		// вывод лексем в файл
		void Delete();							// удалить таблицу лексем (освободить память)
	};
}