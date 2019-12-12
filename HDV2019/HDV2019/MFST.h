#pragma once
#include "GRB.h"
#include "LT.h"
#include <stack>

//#define DEBUG

#define MFST_DIAGN_MAXSIZE 2*ERRORMASSAGE_MAX_SIZE
#define MFST_DIAGN_NUMBER 3

static int FST_TRACE_n = -1;
static char rbuf[1000]{}, sbuf[205]{}, lbuf[1000]{};

#define MFST_TRACE_START std::cout<<std::setw( 4)<< std::left<<"Шаг"<< ':'		\
								  <<std::setw(20)<< std::left<<" Правило"		\
								  <<std::setw(30)<< std::left<<" Входная лента"	\
								  <<std::setw(20)<< std::left<<" Стек"			\
								  <<std::endl;

#define MFST_TRACE1		std::cout<<std::setw(4)<<std::left<<++FST_TRACE_n<< ": "	\
									<<std::setw(20)<<std::left<<rule.getCRule(rbuf, currentRuleChain) \
									<<std::setw(30)<<std::left<<getCLenta(lbuf, currentPosInLent)	\
									<<std::setw(20)<<std::left<<getCSt(sbuf)	\
									<<std::endl;
#define MFST_TRACE2		std::cout<<std::setw(4)<<std::left<<FST_TRACE_n<< ": "	\
									<<std::setw(20)<<std::left<< " "	\
									<<std::setw(30)<<std::left<<getCLenta(lbuf, currentPosInLent)	\
									<<std::setw(20)<<std::left<<getCSt(sbuf)	\
									<<std::endl;
#define MFST_TRACE3		std::cout<<std::setw(4)<<std::left<<++FST_TRACE_n<< ": "	\
									<<std::setw(20)<<std::left<< " "	\
									<<std::setw(30)<<std::left<<getCLenta(lbuf, currentPosInLent)	\
									<<std::setw(20)<<std::left<<getCSt(sbuf)	\
									<<std::endl;

#define MFST_TRACE4(c) std::cout<<std::setw(4)<<std::left<<++FST_TRACE_n<< ": "<<std::setw(20)<<std::left<<c<<std::endl;
#define MFST_TRACE5(c) std::cout<<std::setw(4)<<std::left<<  FST_TRACE_n<< ": "<<std::setw(20)<<std::left<<c<<std::endl;
#define MFST_TRACE6(c,k) std::cout<<std::setw(4)<<std::left<<  FST_TRACE_n<< ": "<<std::setw(20)<<std::left<<c<<k<<std::endl;
#define	MFST_TRACE7		 std::cout << std::setw(4) << std::left << state.posInLent<<": "	\
									<<std::setw(20)<<std::left<<rule.getCRule(rbuf, state.nRuleChain)	\
									<<std::endl;


namespace MFST
{
	struct MFSTState	// состояние автомата (для сохранения)
	{
		short posInLent;
		short nRule;
		short nRuleChain;		// номер текущей цепочки, текущего правила
		std::stack<short> st;	// стек автомата

		MFSTState();
		MFSTState(short posInLent, std::stack<short>& a_steck, short currentChain);
		MFSTState(short posInLent, std::stack<short>& a_stack, short currentRule, short currentChain);

	};

	struct MFST // магазинный автомат
	{
		enum RC_STEP //код возврата функции step
		{
			NS_OK,			// найдено правило и цепочка, цепочка записана в стек 
			NS_NORULE,		// не найдено правило грамматики (ошибка в грамматике)
			NS_NORULECHAIN,	// не найдена походящая цепочка правила (ошибка в исходном коде)
			NS_ERROR,		// неизвесный нетерминальный символ грамматики
			TS_OK,			// тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK,			// тек. символ ленты != вершине стека, восстановленно состояние 
			LENTA_END,		// теущая позиция ленты >= lenta_size 
			SURPRISE		// неожиданный код возврата (ошибка в step)
		};

		struct MFST_Diagnosis	// диагностика
		{
			short	posInLent;		// позиция на ленте 
			RC_STEP	rc_step;			// код завершения шага 
			short	ruleNum;			// номер правила 
			short	nrule_chain;		// номер цепочки правила
			MFST_Diagnosis();
			MFST_Diagnosis(short posInLent, RC_STEP rc_step, short ruleNum, short ruleChainNum);
		} diagnosis[MFST_DIAGN_NUMBER]; // последние самые глубокие сообщения

		GRBALPHABET* lenta;					// перекодированная (TS/NS) лента (из LEX)
		short currentPosInLent;				// текущая позиция на ленте
		short currentRule;					// номер текущего правила
		short currentRuleChain;				// номер текущей цепочки, текущего правила
		short lenta_size;					// размер ленты
		GRB::Greibach grebach;				// грамматика Грейбах
		LT::LexTable lexTable;						
		std::stack<short> st;				// стек автомата
		std::stack<MFSTState> storestate;	// стек для сохранения состояний

		MFST();
		MFST(const LT::LexTable& lexTable,GRB::Greibach grebach);

		char* getCSt(char* buf);			//получить содержиое стека
		char* getCLenta(char* buf, short pos, short n = 25);	//лента: n символов, начиная с pos
		char* getDiagnosis(short n, char* buf);		//получить n-ую строку диагностики или '\0'
		
		bool savestate();	//сохранить состояние автомата
		bool resetstate();	//восстановить состояние автомата
		bool push_chain(GRB::Rule::Chain chain);

		RC_STEP step();		//выполнить шаг автомата
		bool start();		//запустить автомат
		bool savedDiagnosis(RC_STEP prc_step);

		void printRules();	//вывести последовательность правил

		struct OutputStruct
		{
			short stepsCount;
			short* nRules;
			short* nChainsOfRules;

			OutputStruct()
			{
				this->stepsCount = 0;
				this->nRules = 0;
				this->nChainsOfRules = 0;
			}
		}outputStruct;

		bool saveoutputTree();
	};
}
			