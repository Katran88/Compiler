#pragma once

#define	LT_MAXSIZE			1024		// максимальное количество строк в таблице лексем	
#define	LT_TI_NULLXDX		-1			// нет элемента таблицы идентификаторов				
#define	LEX_TYPE			't'			// лексема для тип данных
#define	LEX_ID				'i'			// лексема для идентификатора
#define	LEX_MAIN			'm'			// лексема для главной функции
#define LEX_IF				'c'			// лексема для блока if
#define	LEX_GLOBAL			'g'			// лексема для определения глобальной переменной
#define	LEX_LITERAL			'l'			// лексема для литерала
#define	LEX_FUNCTION		'f'			// лексема для func
#define	LEX_RETURN			'r'			// лексема для return
#define	LEX_PRINT			'p'			// лексема для cprint
#define	LEX_INCLUDE			'e'			// лексема для _connect
#define	LEX_LIBRARY			'y'			// лексема для libName
#define	LEX_LOOP			'z'			// лексема для loop
#define	LEX_TILDA			'~'			// лексема для ~ (помежуток значений итератора цикла)
#define	LEX_SEMICOLON		';'			// лексема для ;
#define	LEX_COMMA			','			// лексема для ,
#define	LEX_LEFTBRACE		'{'			// лексема для {
#define	LEX_RIGHTBRACE		'}'			// лексема для }
#define	LEX_LEFTHESIS		'('			// лексема для (
#define	LEX_RIGHTHESIS		')'			// лексема для )
#define	LEX_PLUS			'+'			// лексема для +
#define	LEX_MINUS			'-'			// лексема для -
#define	LEX_STAR			'*'			// лексема для *
#define	LEX_DIRSLASH		'/'			// лексема для /
#define	LEX_EQUAL_SIGN		'='			// лексема для =
#define	LEX_LESS_SIGN		'<'			// лексема для <
#define	LEX_MORE_SIGN		'>'			// лексема для >
#define	LEX_SAME_SIGN		's'			// лексема для ==
#define	LEX_DIFFERENT_SIGN	'd'			// лексема для !=
#define	LEX_MORE_SAME_SIGN	'b'			// лексема для >=
#define	LEX_LESS_SAME_SIGN	'w'			// лексема для <=
#define	LEX_OR_SIGN			'|'			// лексема для or
#define	LEX_AND_SIGN		'&'			// лексема для and
#define	LEX_NOT_SIGN		'!'			// лексема для !

#define libName "DateTime"

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