#include "PolishNotation.h"

#define BEGIN_SIMBOL '|'

namespace PN
{
	bool Stack::push(const LT::Entry inputEntry)
	{
		if (this->current_size < this->max_count)
		{
			lexEntry* temp = new lexEntry(inputEntry, this->head);
			this->head = temp;
			this->current_size++;
		}
		else
			return false;
	}

	bool Stack::push(const PN::lexEntry inputEntry)
	{
		if (this->current_size < this->max_count)
		{
			lexEntry* temp = new lexEntry(inputEntry, this->head);
			this->head = temp;
			this->current_size++;
		}
		else
			return false;
	}

	lexEntry* Stack::pop()
	{
		if (this->current_size != 0)
		{

			lexEntry temp(*this->head);
			delete this->head;
			this->head = temp.next;
			this->current_size--;

			return &lexEntry(temp);
		}
		else
			return 0;
	}

	char Stack::checkLastEl()
	{
		return this->head->lexema;
	}
}

bool PN::polishNotation(int lexBeginIndex, LT::LexTable& LexTable, IT::IdTable& IdTable)
{
	PN::Stack* literals = new PN::Stack;
	PN::Stack* operators = new PN::Stack;


	operators->push({ BEGIN_SIMBOL, -1, -1 });

	bool flag = true;
	bool isComplete = false;
	LT::Entry temp;
	for (int i = lexBeginIndex; flag;)
	{
		temp = LexTable.GetEntry(i);

		if (temp.lexema == LEX_ID || temp.lexema == LEX_LITERAL)
		{
			literals->push(temp);
			i++;
			continue;
		}

		switch (operators->checkLastEl())
		{
			case BEGIN_SIMBOL:
			{
				if (temp.lexema == LEX_PLUS ||
					temp.lexema == LEX_MINUS ||
					temp.lexema == LEX_STAR ||
					temp.lexema == LEX_DIRSLASH ||
					temp.lexema == LEX_LEFTHESIS)
				{
					operators->push(temp);
					i++;
					break;
				}

				if (temp.lexema == LEX_RIGHTHESIS)	 { flag = false;}
				if (temp.lexema == LEX_SEMICOLON) { isComplete = true; flag = false; }
				break;
			}

			case LEX_PLUS:
			case LEX_MINUS:
			{
				if (temp.lexema == LEX_PLUS ||
					temp.lexema == LEX_MINUS ||
					temp.lexema == LEX_LEFTHESIS ||
					temp.lexema == LEX_SEMICOLON)
				{
					literals->push(*operators->pop());
					break;
				}
				
				if (temp.lexema == LEX_STAR ||
					temp.lexema == LEX_DIRSLASH ||
					temp.lexema == LEX_LEFTHESIS)
				{
					operators->push(temp);
					i++;
					break;
				}
			}

			case LEX_STAR:
			case LEX_DIRSLASH:
			{
				if (temp.lexema == LEX_PLUS ||
					temp.lexema == LEX_MINUS ||
					temp.lexema == LEX_STAR ||
					temp.lexema == LEX_DIRSLASH ||
					temp.lexema == LEX_RIGHTHESIS ||
					temp.lexema == LEX_SEMICOLON)
				{
					literals->push(*operators->pop());
					break;
				}

				if (temp.lexema == LEX_LEFTHESIS)
				{
					operators->push(temp);
					i++;
					break;
				}
			}

			case LEX_LEFTHESIS:
			{
				if (temp.lexema == LEX_SEMICOLON) flag = false;

				if (temp.lexema == LEX_PLUS ||
					temp.lexema == LEX_MINUS ||
					temp.lexema == LEX_STAR ||
					temp.lexema == LEX_DIRSLASH ||
					temp.lexema == LEX_LEFTHESIS)
				{
					operators->push(temp);
					i++;
					break;
				}

				if (temp.lexema == LEX_RIGHTHESIS)
				{
					operators->pop();
					i++;
					break;
				}
			}
			default:
				flag = false;
		}

		
	}

	if (isComplete)
	{
		PN::lexEntry* temp;
		int count = literals->getCount();
		LT::Entry* tempArr = new LT::Entry[count];
		
		for (int i = count - 1; i >= 0; i--)
		{
			temp = literals->pop();
			if (temp)
				tempArr[i] = *temp;
		}

		for (int i = lexBeginIndex, j = 0; LexTable.table[i].lexema != LEX_SEMICOLON; i++)
		{
			if (i < count)
				LexTable.table[i] = tempArr[j++];
			else
				LexTable.table[i] = {' ', tempArr[j-1].sn, -1};
		}

		delete[] tempArr;
		delete literals;
		delete operators;
		return true;
	}
	else
	{
		delete literals;
		delete operators;
		return false;
	}
}