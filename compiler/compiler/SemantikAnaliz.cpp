#include "SemantikAnaliz.h"

void SemantikAnaliz(std::stack<MFST::MFSTState>& storestate, IT::IdTable& idTable)
{
	short a;
		for (const auto& state : storestate._Get_container())
		{
			a = state.nRule;
			a = state.nRuleChain;
			
			for (const auto& chainEl : state.st._Get_container())
			{
				a = chainEl;
			}
		}
}
