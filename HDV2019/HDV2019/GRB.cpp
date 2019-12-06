#include "GRB.h"
#include <string.h>
#define GRB_ERROR_SERIES 130

#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)

namespace GRB
{
	Rule::Chain::Chain(short symbolCount, GRBALPHABET s, ...)
	{
		this->chainOfTerm_nTerm = new GRBALPHABET[this->size = symbolCount];

		int* ptr = (int*)& s;
		for (int i = 0; i < symbolCount; i++)
			this->chainOfTerm_nTerm[i] = (short)ptr[i];
	}

	Rule::Rule(GRBALPHABET nTerm, int iderror, short rules_count, Chain c, ...)
	{
		this->nTerm = nTerm;
		this->iderror = iderror;
		this->chains = new Chain[this->rulesCount = rules_count];

		Chain* ptr = &c;
		for (int i = 0; i < rules_count; i++)
				this->chains[i] = ptr[i];
	}

	Greibach::Greibach(GRBALPHABET startN, GRBALPHABET stbottomT, short rulesCount, Rule r, ...)
	{
		this->startN = startN;
		this->stbottomT = stbottomT;
		this->rules = new Rule[this->rulesCount = rulesCount];
		Rule* ptr = &r;
		for (int i = 0; i < rulesCount; i++)
			rules[i] = ptr[i];
	}

	short Greibach::getRule(GRBALPHABET nTerm, Rule& rule)
	{
		short rc = -1, k = 0;

		while (k < this->rulesCount && rules[k].nTerm != nTerm)
			k++;

		if (k < this->rulesCount)
			rule = rules[rc = k];

		return rc;
	}

	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < this->rulesCount)
			rc = rules[n];
		return rc;
	}

	char* Rule::getCRule(char* b, short nchain)
	{
		char buf[20];
		b[0] = Chain::alphabet_to_char(this->nTerm);
		b[1] = '-';
		b[2] = '>';
		b[3] = '\0';
		this->chains[nchain].getCChain(buf);
		strcat(b, buf);

		return b;
	}

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;
		
		while (j < this->rulesCount && this->chains[j].chainOfTerm_nTerm[0] != t)
			j++;

		rc = (j < this->rulesCount ? j : -1);
		if (rc >= 0)
			pchain = chains[rc];
		return rc;
	}

	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < this->size; i++)
			b[i] = Chain::alphabet_to_char(this->chainOfTerm_nTerm[i]);
		b[this->size] = '\0';
		return b;
	}


#pragma region GRB_rules
	Greibach greibach(NS('S'), TS('$'), 10,
		Rule(NS('S'), GRB_ERROR_SERIES + 0, 5,	//Неверная структура программы
			//объявление глобальной переменной
			Rule::Chain(7, TS('g'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('S')),
			//подключение библиотеки
			Rule::Chain(3, TS('e'), TS('y'), NS('S')),
			// main
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}')),
			// объявление функции с параметрами и без
			Rule::Chain(10, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(9, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), NS('S'))
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 2, 20,	//ошибка в выражении
			//определение переменной
			Rule::Chain(4, TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(3, TS('t'), TS('i'), TS(';')),
			///////
			// объявление и инициализация
			Rule::Chain(6, TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('t'), TS('i'), TS('='), NS('E'), TS(';')),
			///////
			// инициализация
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			///////
			// return
			Rule::Chain(4, TS('r'), NS('B'), TS(';'), NS('N')),
			Rule::Chain(3, TS('r'), NS('B'), TS(';')),
			///////
			//вызов функции без присвоения возвращаемого значения
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('N')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),

			Rule::Chain(4, TS('i'), TS('('), TS(')'), NS('N')),
			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			///////
			// вывод в консоль
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('p'), NS('E'), TS(';')),
			///////
			// цикл
			Rule::Chain(12, TS('z'), TS('('), TS('i'), TS(','), TS('l'), TS('~'), TS('l'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(11, TS('z'), TS('('), TS('i'), TS(','), TS('l'), TS('~'), TS('l'), TS(')'), TS('{'), NS('N'), TS('}')),
			///////
			// условный оператор
			Rule::Chain(8, TS('c'), TS('('), NS('C'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(7, TS('c'), TS('('), NS('C'), TS(')'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(9, TS('c'), TS('!'), TS('('), NS('C'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(8, TS('c'), TS('!'), TS('('), NS('C'), TS(')'), TS('{'), NS('N'), TS('}'))
			///////
		),
		Rule(NS('C'), GRB_ERROR_SERIES + 6, 4,	//условие входа в блок
			//одно условие
			Rule::Chain(3, TS('i'), NS('O'), NS('B')),
			Rule::Chain(3, TS('i'), NS('O'), NS('B')),
			//несколько с операторами И ИЛИ
			Rule::Chain(4, TS('i'), NS('O'), NS('B'), NS('L')),
			Rule::Chain(4, TS('i'), NS('O'), NS('B'), NS('L'))
		),
		Rule(NS('L'), GRB_ERROR_SERIES + 8, 2,	//оператор в условии входа в блок
			Rule::Chain(2, TS('|'), NS('C')),
			Rule::Chain(2, TS('&'), NS('C'))
		),
		Rule(NS('O'), GRB_ERROR_SERIES + 7, 6,	//оператор в условии входа в блок
			Rule::Chain(1, TS('<')),
			Rule::Chain(1, TS('>')),
			Rule::Chain(1, TS('s')),
			Rule::Chain(1, TS('d')),
			Rule::Chain(1, TS('b')),
			Rule::Chain(1, TS('w'))
		),
		Rule(NS('B'), GRB_ERROR_SERIES + 1, 2,	//операнд в выражении
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l'))
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 1, 12,	//операнд в выражении
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(2, TS('!'), NS('B')),

			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),

			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(3, TS('!'), NS('B'), NS('M')),

			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')),
			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), TS(')'), NS('M'))
		),
		Rule(NS('M'), GRB_ERROR_SERIES + 5, 6,	//оператор в выражении
			Rule::Chain(2, TS('+'), NS('E')),
			Rule::Chain(2, TS('-'), NS('E')),
			Rule::Chain(2, TS('*'), NS('E')),
			Rule::Chain(2, TS('/'), NS('E')),

			Rule::Chain(2, TS('&'), NS('E')),
			Rule::Chain(2, TS('|'), NS('E'))
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 3, 2,	// параметры функции при определении
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 4, 4, //параметры вызываемой ф-ции
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		)
	);
#pragma endregion

	Greibach getGreibach() { return greibach; }

}