#include "SemantikAnaliz.h"
#include "PolishNotation.h"
#define ASSIGMENT_NTERM 'E'
#define FUNC_CALL_NTERM 'W'
#define EXPRESSION_NTERM 'N'
void SemantikAnaliz(std::stack<MFST::MFSTState>& storestate, GRB::Greibach& grebach, LT::LexTable& LexTable, IT::IdTable& idTable)
{
	char currentChain[20]{};
	GRB::Rule rule;
	IT::IDDATATYPE tempType;
	int alreadyCheckedPos = -1; //for correct checking of assigment
	bool flagForFuncParams = false; // for correct checking of assigment with calling of function
		for (const auto& state : storestate._Get_container())
		{
			rule = grebach.getRule(state.nRule);
			rule.getCRule(currentChain, state.nRuleChain);
			

			//checking for 'E' and equals of checking string
			if (ASSIGMENT_NTERM == -rule.nTerm)
			{
				//for assigment
				if (state.posInLent > alreadyCheckedPos)
				{
					if (LexTable.table[state.posInLent - 1].lexema == LEX_EQUAL_SIGN)
					{
						//����� ������ ���� ����������� ������� �����-���� ��������
						if(idTable.table[LexTable.table[state.posInLent - 2].idxTI].idtype != IT::IDTYPE::V)
							throw ERROR_THROW_IN(411, LexTable.table[state.posInLent - 2].sn + 1, -1);

						tempType = idTable.table[LexTable.table[state.posInLent - 2].idxTI].iddatatype;

						if (tempType == IT::IDDATATYPE::STR)
						{
							for (int i = state.posInLent; LexTable.table[i].lexema != LEX_SEMICOLON; alreadyCheckedPos = i++)
							{
								if (LexTable.table[i].idxTI != -1)
								{
									if (idTable.table[LexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR)
									{
										if (idTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::F)
											flagForFuncParams = true;
									}
									else
									{
										if(!flagForFuncParams)
											throw ERROR_THROW_IN(400, LexTable.table[i].sn + 1, -1);
									}
								}
								else
								{
									if (LexTable.table[i].lexema != LEX_PLUS &&
										LexTable.table[i].lexema != LEX_LEFTHESIS &&
										LexTable.table[i].lexema != LEX_RIGHTHESIS &&
										LexTable.table[i].lexema != LEX_COMMA &&
										!flagForFuncParams)
										throw ERROR_THROW_IN(408, LexTable.table[i].sn + 1, -1);

									if (LexTable.table[i].lexema == LEX_LEFTHESIS &&
										LexTable.table[i-1].idxTI != -1  &&
										idTable.table[LexTable.table[i - 1].idxTI].idtype != IT::IDTYPE::F)
										throw ERROR_THROW_IN(407, LexTable.table[i].sn + 1, -1);
									if (LexTable.table[i].lexema == LEX_RIGHTHESIS && flagForFuncParams)
										flagForFuncParams = false;
								}
							}
						}

						if (tempType == IT::IDDATATYPE::INT || tempType == IT::IDDATATYPE::UBYTE)
						{
							for (int i = state.posInLent; LexTable.table[i].lexema != LEX_SEMICOLON; alreadyCheckedPos = i++)
							{
								if (LexTable.table[i].idxTI != -1)
								{
									if (idTable.table[LexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::UBYTE ||
										idTable.table[LexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT)
									{
										if (idTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::F)
											flagForFuncParams = true;
									}
									else
									{
										if (!flagForFuncParams)
											throw ERROR_THROW_IN(400, LexTable.table[i].sn + 1, -1);
									}
								}
								else
								{
									if (LexTable.table[i].lexema == LEX_LEFTHESIS &&
										LexTable.table[i - 1].idxTI != -1 &&
										idTable.table[LexTable.table[i - 1].idxTI].idtype != IT::IDTYPE::F)
										throw ERROR_THROW_IN(407, LexTable.table[i].sn + 1, -1);
									if (LexTable.table[i].lexema == LEX_RIGHTHESIS && flagForFuncParams)
										flagForFuncParams = false;
								}
							}
						}

						if (tempType == IT::IDDATATYPE::LOGIC)
						{
							for (int i = state.posInLent; LexTable.table[i].lexema != LEX_SEMICOLON; alreadyCheckedPos = i++)
							{
								if (LexTable.table[i].idxTI != -1)
								{
									if (idTable.table[LexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::LOGIC)
									{
										if (idTable.table[LexTable.table[i].idxTI].idtype == IT::IDTYPE::F)
											flagForFuncParams = true;
									}
									else
									{
										if (!flagForFuncParams)
											throw ERROR_THROW_IN(400, LexTable.table[i].sn + 1, -1);
									}
								}
								else
								{
									if (LexTable.table[i].lexema != LEX_AND_SIGN &&
										LexTable.table[i].lexema != LEX_OR_SIGN &&
										LexTable.table[i].lexema != LEX_NOT_SIGN &&
										LexTable.table[i].lexema != LEX_LEFTHESIS &&
										LexTable.table[i].lexema != LEX_RIGHTHESIS &&
										LexTable.table[i].lexema != LEX_COMMA &&
										!flagForFuncParams)
										throw ERROR_THROW_IN(408, LexTable.table[i].sn + 1, -1);

									if (LexTable.table[i].lexema == LEX_LEFTHESIS &&
										LexTable.table[i - 1].idxTI != -1 &&
										idTable.table[LexTable.table[i - 1].idxTI].idtype != IT::IDTYPE::F)
										throw ERROR_THROW_IN(407, LexTable.table[i].sn + 1, -1);
									if (LexTable.table[i].lexema == LEX_RIGHTHESIS && flagForFuncParams)
										flagForFuncParams = false;
								}
							}
						}

						if (!PN::polishNotation(state.posInLent, LexTable, idTable))
							throw ERROR_THROW_IN(406, LexTable.table[state.posInLent].sn + 1, -1);
						continue;
					}
				}
			}

			if (EXPRESSION_NTERM == -rule.nTerm)
			{
				//for return
				if (LexTable.table[state.posInLent].lexema == LEX_RETURN)
				{
					tempType = idTable.table[LexTable.table[state.posInLent].idxTI].iddatatype;
					if (tempType == IT::IDDATATYPE::STR)
					{
						if (idTable.table[LexTable.table[state.posInLent + 1].idxTI].iddatatype != IT::IDDATATYPE::STR)
								throw ERROR_THROW_IN(402, LexTable.table[state.posInLent].sn+1, -1);
						continue;
					}

					if (tempType == IT::IDDATATYPE::UBYTE || tempType == IT::IDDATATYPE::INT)
					{
						if (idTable.table[LexTable.table[state.posInLent + 1].idxTI].iddatatype != IT::IDDATATYPE::INT &&
							idTable.table[LexTable.table[state.posInLent + 1].idxTI].iddatatype != IT::IDDATATYPE::UBYTE)
							throw ERROR_THROW_IN(402, LexTable.table[state.posInLent].sn+1, -1);
						continue;
					}

					if (tempType == IT::IDDATATYPE::LOGIC)
					{
						if (idTable.table[LexTable.table[state.posInLent + 1].idxTI].iddatatype != IT::IDDATATYPE::LOGIC)
							throw ERROR_THROW_IN(402, LexTable.table[state.posInLent].sn+1, -1);
						continue;
					}
				}

				//for loop
				/*if (LexTable.table[state.posInLent].lexema == LEX_LOOP)
				{
					if(idTable.table[LexTable.table[state.posInLent+4].idxTI].value.vint > idTable.table[LexTable.table[state.posInLent + 6].idxTI].value.vint)
						throw ERROR_THROW_IN(405, LexTable.table[state.posInLent].sn + 1, -1)
				}*/
			}

			if (FUNC_CALL_NTERM == -rule.nTerm && state.posInLent > alreadyCheckedPos)
			{
				for (int i = state.posInLent, j = 0; LexTable.table[i].lexema != LEX_RIGHTHESIS; alreadyCheckedPos = i)
				{
					if(LexTable.table[i++].idxTI != -1)
						if(idTable.table[LexTable.table[state.posInLent - 2].idxTI].funcParams[j++].type != 
						   idTable.table[LexTable.table[i-1].idxTI].iddatatype)
							throw ERROR_THROW_IN(404, LexTable.table[i-1].sn+1, -1)
				}
			}
			
		}
}



