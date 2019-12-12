#pragma once
#include "GRB.h"
#include "LT.h"
#include <stack>

//#define DEBUG

#define MFST_DIAGN_MAXSIZE 2*ERRORMASSAGE_MAX_SIZE
#define MFST_DIAGN_NUMBER 3

static int FST_TRACE_n = -1;
static char rbuf[1000]{}, sbuf[205]{}, lbuf[1000]{};

#define MFST_TRACE_START std::cout<<std::setw( 4)<< std::left<<"���"<< ':'		\
								  <<std::setw(20)<< std::left<<" �������"		\
								  <<std::setw(30)<< std::left<<" ������� �����"	\
								  <<std::setw(20)<< std::left<<" ����"			\
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
	struct MFSTState	// ��������� �������� (��� ����������)
	{
		short posInLent;
		short nRule;
		short nRuleChain;		// ����� ������� �������, �������� �������
		std::stack<short> st;	// ���� ��������

		MFSTState();
		MFSTState(short posInLent, std::stack<short>& a_steck, short currentChain);
		MFSTState(short posInLent, std::stack<short>& a_stack, short currentRule, short currentChain);

	};

	struct MFST // ���������� �������
	{
		enum RC_STEP //��� �������� ������� step
		{
			NS_OK,			// ������� ������� � �������, ������� �������� � ���� 
			NS_NORULE,		// �� ������� ������� ���������� (������ � ����������)
			NS_NORULECHAIN,	// �� ������� ��������� ������� ������� (������ � �������� ����)
			NS_ERROR,		// ���������� �������������� ������ ����������
			TS_OK,			// ���. ������ ����� == ������� �����, ������������ �����, pop �����
			TS_NOK,			// ���. ������ ����� != ������� �����, �������������� ��������� 
			LENTA_END,		// ������ ������� ����� >= lenta_size 
			SURPRISE		// ����������� ��� �������� (������ � step)
		};

		struct MFST_Diagnosis	// �����������
		{
			short	posInLent;		// ������� �� ����� 
			RC_STEP	rc_step;			// ��� ���������� ���� 
			short	ruleNum;			// ����� ������� 
			short	nrule_chain;		// ����� ������� �������
			MFST_Diagnosis();
			MFST_Diagnosis(short posInLent, RC_STEP rc_step, short ruleNum, short ruleChainNum);
		} diagnosis[MFST_DIAGN_NUMBER]; // ��������� ����� �������� ���������

		GRBALPHABET* lenta;					// ���������������� (TS/NS) ����� (�� LEX)
		short currentPosInLent;				// ������� ������� �� �����
		short currentRule;					// ����� �������� �������
		short currentRuleChain;				// ����� ������� �������, �������� �������
		short lenta_size;					// ������ �����
		GRB::Greibach grebach;				// ���������� �������
		LT::LexTable lexTable;						
		std::stack<short> st;				// ���� ��������
		std::stack<MFSTState> storestate;	// ���� ��� ���������� ���������

		MFST();
		MFST(const LT::LexTable& lexTable,GRB::Greibach grebach);

		char* getCSt(char* buf);			//�������� ��������� �����
		char* getCLenta(char* buf, short pos, short n = 25);	//�����: n ��������, ������� � pos
		char* getDiagnosis(short n, char* buf);		//�������� n-�� ������ ����������� ��� '\0'
		
		bool savestate();	//��������� ��������� ��������
		bool resetstate();	//������������ ��������� ��������
		bool push_chain(GRB::Rule::Chain chain);

		RC_STEP step();		//��������� ��� ��������
		bool start();		//��������� �������
		bool savedDiagnosis(RC_STEP prc_step);

		void printRules();	//������� ������������������ ������

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
			