#pragma once
#include <iostream>
#include "LT.h"
#include "IT.h"


namespace PN
{
	struct lexEntry : public LT::Entry
	{
		lexEntry* next;

		lexEntry()
		{
			this->next = nullptr;
		}

		lexEntry(LT::Entry inputEntry, lexEntry* next)
		{
			this->lexema = inputEntry.lexema;
			this->idxTI = inputEntry.idxTI;
			this->sn = inputEntry.sn;
			this->next = next;
		}

		operator LT::Entry() const
		{
			return LT::Entry(this->lexema, this->sn, this->idxTI);
		}
	};

	class Stack
	{
		private:
		lexEntry* head;
		int current_size;
		const int max_count = 200;

		public:
		Stack()
		{
			this->current_size = 0;
			this->head = nullptr;
		}
		~Stack()
		{
			while(this->current_size != 0)
				this->pop();
		}

		bool push(const LT::Entry inputEntry);
		bool push(const PN::lexEntry inputEntry);
		lexEntry* pop();
		char checkLastEl();
		int getCount() { return this->current_size; }
	};


	bool polishNotation(int lexBeginIndex, LT::LexTable& LexTable, IT::IdTable& IdTable);
}


