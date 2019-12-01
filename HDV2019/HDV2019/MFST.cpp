#include "MFST.h"
#include "GRB.h"
#include <iostream>
#include <iomanip>

#pragma region Constructors_MFSTState
MFST::MFSTState::MFSTState()
{
	this->posInLent = 0;
	this->nRuleChain = -1;
	this->nRule = -1;
}

MFST::MFSTState::MFSTState(short posInLent, std::stack<short>& a_stack, short currentChain)
{
	this->posInLent = posInLent;
	this->st = a_stack;
	this->nRuleChain = currentChain;
}

MFST::MFSTState::MFSTState(short posInLent, std::stack<short>& a_stack, short currentRule, short currentChain)
{
	this->posInLent = posInLent;
	this->st = a_stack;
	this->nRule = currentRule;
	this->nRuleChain = currentChain;
}
#pragma endregion

#pragma region Constructors_MFST_Diagnosis
MFST::MFST::MFST_Diagnosis::MFST_Diagnosis()
{
	this->posInLent = -1;
	this->rc_step = RC_STEP::SURPRISE;
	this->ruleNum = -1;
	this->nrule_chain = -1;
}

MFST::MFST::MFST_Diagnosis::MFST_Diagnosis(short posInLent, RC_STEP rc_step, short ruleNum, short ruleChainNum)
{
	this->posInLent = posInLent;
	this->rc_step = rc_step;
	this->ruleNum = ruleNum;
	this->nrule_chain = ruleChainNum;
}

#pragma endregion

#pragma region Constructors_MFST
MFST::MFST::MFST()
{
	this->lenta = 0;
	this->lenta_size;
	this->currentPosInLent = 0;
}

MFST::MFST::MFST(const LT::LexTable& lexTable, GRB::Greibach grebach)
{
	this->grebach = grebach;
	this->lexTable = lexTable;
	this->lenta = new short[this->lenta_size = lexTable.current_size];

	for (int i = 0; i < this->lenta_size; i++)
		this->lenta[i] = GRB::Rule::Chain::T(lexTable.table[i].lexema);

	this->currentPosInLent = 0;
	this->st.push(grebach.stbottomT);
	this->st.push(grebach.startN);
	this->currentRuleChain = -1;
}
#pragma endregion

MFST::MFST::RC_STEP MFST::MFST::step()
{
	RC_STEP rc = SURPRISE;

	if (currentPosInLent < lenta_size)
	{
		if(GRB::Rule::Chain::isN(st.top()))
		{
			GRB::Rule rule;
			if ((currentRule = grebach.getRule(st.top(), rule)) >= 0)
			{
				GRB::Rule::Chain chain;
				if ((currentRuleChain = rule.getNextChain(lenta[currentPosInLent], chain, currentRuleChain + 1)) >= 0)
				{
					MFST_TRACE1
					savestate();
					st.pop();
					push_chain(chain);
					rc = NS_OK;
					MFST_TRACE2
				}
				else
				{
					MFST_TRACE4("TNS_NS_NORULECHAIN/NS_NORULE")
					savedDiagnosis(NS_NORULECHAIN); rc = resetstate() ? NS_NORULECHAIN : NS_NORULE;
				}
			}
			else
				rc = NS_ERROR;
		}
		else if (st.top() == lenta[currentPosInLent])
		{
			currentPosInLent++;
			st.pop();
			currentRuleChain = -1;
			rc = TS_OK;
			MFST_TRACE3
		}
		else
		{
			MFST_TRACE4("TNS_NS_NORULECHAIN/NS_NORULE")
			rc = resetstate() ? TS_NOK : NS_NORULECHAIN;
		}
	}
	else
	{
		rc = LENTA_END;
		MFST_TRACE4("LENTA_END")
	}

	return rc;
}

bool MFST::MFST::push_chain(GRB::Rule::Chain chain)
{
	for (int i = chain.size - 1; i >= 0; i--)
		st.push(chain.chainOfTerm_nTerm[i]);
	return true;
}

bool MFST::MFST::savestate()
{
	storestate.push(MFSTState(currentPosInLent, st, currentRule, currentRuleChain));
	MFST_TRACE6("SAVESTATE:", storestate.size())
	return true;
}

bool MFST::MFST::resetstate()
{
	bool rc = false;
	MFSTState state;
	
	if ((rc = storestate.size()) > 0)
	{
		state = storestate.top();
		currentPosInLent = state.posInLent;
		st = state.st;
		currentRule = state.nRule;
		currentRuleChain = state.nRuleChain;
		storestate.pop();

		MFST_TRACE5("RESTATE")
		MFST_TRACE2
	}

	return rc;
}

bool MFST::MFST::savedDiagnosis(RC_STEP prc_step)
{
	bool rc = false;
	short k = 0;

	while (k < MFST_DIAGN_NUMBER && currentPosInLent <= diagnosis[k].posInLent)
		k++;

	if (rc = (k < MFST_DIAGN_NUMBER))
	{
		diagnosis[k] = MFST_Diagnosis(currentPosInLent, prc_step, currentRule, currentRuleChain);

		for (int i = k + 1; i < MFST_DIAGN_NUMBER; i++)
			diagnosis[i].posInLent = -1;
	}

	return rc;
}

bool MFST::MFST::start()
{
	bool rc = false;
	RC_STEP rc_step = SURPRISE;
	char buf[MFST_DIAGN_MAXSIZE]{};
	rc_step = step();
	while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
		rc_step = step();

	switch (rc_step)
	{
		case LENTA_END:
		{
			MFST_TRACE4("------>LENTA_END")
			std::cout<< "------------------------------------------------------------------------------------------   ------"<< std::endl;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: всего строк %d, синтаксический анализ выполнен без ошибок", 0, lenta_size);
			std::cout << std::setw(4) << std::left << 0 << "всего строк " << lenta_size << ", синтаксический анализ выполнен без ошибок" << std::endl;
			rc = true;
			break;
		}

		case NS_NORULE:
		{
			MFST_TRACE4("------>NS_NORULE")
			std::cout << "------------------------------------------------------------------------------------------   ------" << std::endl;
			std::cout << getDiagnosis(0, buf) << std::endl;
			std::cout << getDiagnosis(1, buf) << std::endl;
			std::cout << getDiagnosis(2, buf) << std::endl;
			break;
		}

		case NS_NORULECHAIN: MFST_TRACE4("------>NS_NORULECHAIN") break;
		case NS_ERROR: MFST_TRACE4("------>NS_ERROR") break;
		case SURPRISE: MFST_TRACE4("------>NS_SURPRISE") break;


	}

	return rc;
}

char* MFST::MFST::getCSt(char* buf)
{
	short p;
	for (int i = (signed)st.size() - 1; i >= 0; --i)
	{
		p = st._Get_container()[i];
		buf[st.size() - 1 - i] = GRB::Rule::Chain::alphabet_to_char(p);
	}

	buf[st.size()] = '\0';

	return buf;
}

char* MFST::MFST::getCLenta(char* buf, short pos, short n)
{
	short i = 0, k = (pos + n < lenta_size) ? pos + n : lenta_size;

	for (int i = pos; i < k; i++)
		buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);

	buf[i - pos] = '\0';
	return buf;
}

char* MFST::MFST::getDiagnosis(short n, char* buf)
{
	char* rc = new char[200]{};
	int errid = 0;
	int lpos = -1;
	if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].posInLent) >= 0)
	{
		errid = grebach.getRule(diagnosis[n].ruleNum).iderror;
		Error::ERROR err = Error::geterror(errid);
		sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,	%s", err.id, lexTable.table[lpos].sn, err.message);
		rc = buf;
	}
	return rc;
}

void MFST::MFST::printRules()
{
	MFSTState state;
	GRB::Rule rule;
	for (unsigned short i = 0; i < storestate.size(); i++)
	{
		state = storestate._Get_container()[i];
		rule = grebach.getRule(state.nRule);
		MFST_TRACE7
	}
}

bool MFST::MFST::saveoutputTree()
{
	MFSTState state;
	GRB::Rule rule;
	outputStruct.nRules = new short[outputStruct.stepsCount = storestate.size()];
	outputStruct.nChainsOfRules = new short[outputStruct.stepsCount];

	for (unsigned short i = 0; i < storestate.size(); i++)
	{
		state = storestate._Get_container()[i];
		outputStruct.nRules[i] = state.nRule;
		outputStruct.nChainsOfRules[i] = state.nRuleChain;
	}
	return true;
}