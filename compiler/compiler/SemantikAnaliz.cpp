#include "SemantikAnaliz.h"
#define ASSIGMENT_NTERM 'E'
void SemantikAnaliz(std::stack<MFST::MFSTState>& storestate, GRB::Greibach& grebach, LT::LexTable& LexTable, IT::IdTable& idTable)
{
	char currentChain[200]{};
	GRB::Rule rule;
	IT::IDDATATYPE tempType;
	int currentString = -1; //for correct checking of assigment
		for (const auto& state : storestate._Get_container())
		{
			rule = grebach.getRule(state.nRule);
			rule.getCRule(currentChain, state.nRuleChain);
			
			
			//checking for 'E' and equals of checking string
			if (ASSIGMENT_NTERM == -rule.nTerm) //return and assigment
			{

				//for assigment
				if ((currentString == -1 || currentString != LexTable.table[state.posInLent].sn))
				{
					currentString = LexTable.table[state.posInLent].sn;
					if (LexTable.table[state.posInLent - 1].lexema == LEX_EQUAL_SIGN)
					{
						tempType = idTable.table[LexTable.table[state.posInLent - 2].idxTI].iddatatype;

						if (tempType == IT::IDDATATYPE::STR)
						{
							for (int i = state.posInLent; LexTable.table[i].lexema != LEX_SEMICOLON; i++)
								if (idTable.table[LexTable.table[i].idxTI].iddatatype != IT::IDDATATYPE::DEF &&
									idTable.table[LexTable.table[i].idxTI].iddatatype != IT::IDDATATYPE::STR)
									throw ERROR_THROW_IN(400, LexTable.table[i].sn, -1);
							continue;
						}

						if (tempType == IT::IDDATATYPE::INT || tempType == IT::IDDATATYPE::UBYTE)
						{
							for (int i = state.posInLent; LexTable.table[i].lexema != LEX_SEMICOLON; i++)
								if (idTable.table[LexTable.table[i].idxTI].iddatatype != IT::IDDATATYPE::DEF &&
									idTable.table[LexTable.table[i].idxTI].iddatatype != IT::IDDATATYPE::INT &&
									idTable.table[LexTable.table[i].idxTI].iddatatype != IT::IDDATATYPE::UBYTE)
									throw ERROR_THROW_IN(400, LexTable.table[i].sn, -1);
							continue;
						}

						if (tempType == IT::IDDATATYPE::LOGIC)
						{
							for (int i = state.posInLent; LexTable.table[i].lexema != LEX_SEMICOLON; i++)
								if (idTable.table[LexTable.table[i].idxTI].iddatatype != IT::IDDATATYPE::DEF &&
									idTable.table[LexTable.table[i].idxTI].iddatatype != IT::IDDATATYPE::LOGIC)
									throw ERROR_THROW_IN(400, LexTable.table[i].sn, -1);
							continue;
						}
					}
				}

				//for return
				if (LexTable.table[state.posInLent - 1].lexema == LEX_RETURN)
				{
					tempType = idTable.table[LexTable.table[state.posInLent - 1].idxTI].iddatatype;
					for (int i = state.posInLent; LexTable.table[i].lexema != LEX_SEMICOLON; i++)
						if (idTable.table[LexTable.table[i].idxTI].iddatatype != tempType)
							throw ERROR_THROW_IN(402, LexTable.table[i].sn, -1);
					continue;
				}
			}

			
			
		}
}

