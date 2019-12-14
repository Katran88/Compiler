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
	int currentchechedPosForParams = -1;
	bool flagForFuncParams = false; // for correct checking of assigment with calling of function
		for (const auto& state : storestate._Get_container())
		{
			rule = grebach.getRule(state.nRule);
			rule.getCRule(currentChain, state.nRuleChain);
			

			//checking for 'E' and equals of checking string
			if (ASSIGMENT_NTERM == -rule.nTerm)
			{
				if (state.posInLent > alreadyCheckedPos)
				{
					//for assigment
					if (LexTable.table[state.posInLent - 1].lexema == LEX_EQUAL_SIGN)
					{
						//чтобы нельзя было присваивать функции какое-либо значение
						if(idTable.table[LexTable.table[state.posInLent - 2].idxTI].idtype != IT::IDTYPE::V &&
							idTable.table[LexTable.table[state.posInLent - 2].idxTI].idtype != IT::IDTYPE::P)
							throw ERROR_THROW_IN(411, LexTable.table[state.posInLent - 2].sn + 1, -1);

						tempType = idTable.table[LexTable.table[state.posInLent - 2].idxTI].iddatatype;

						if (tempType == IT::IDDATATYPE::STR)
						{
							//за раз можно осуществить конкатенацию только 4х строк
							int strCounter = 1;

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
									if (LexTable.table[i].lexema == LEX_PLUS)
										strCounter++;

									if (strCounter >= 4)
										throw ERROR_THROW_IN(412, LexTable.table[i].sn + 1, -1);

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
						continue;
					}
				

					//for cprint
					if (LexTable.table[state.posInLent-1].lexema == LEX_PRINT ||
						LexTable.table[state.posInLent-1].lexema == LEX_PRINTL)
					{
						//за раз можно осуществить конкатенацию только 4х строк
						int strCounter = 1;
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
									if (!flagForFuncParams)
										throw ERROR_THROW_IN(400, LexTable.table[i].sn + 1, -1);
								}
							}
							else
							{
								if (LexTable.table[i].lexema == LEX_PLUS)
									strCounter++;

								if (strCounter > 4)
									throw ERROR_THROW_IN(412, LexTable.table[i].sn + 1, -1);

								if (LexTable.table[i].lexema != LEX_PLUS &&
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
						continue;
					}
				}
			}

			//for some expressions
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
				if (LexTable.table[state.posInLent].lexema == LEX_LOOP)
				{
					if((idTable.table[LexTable.table[state.posInLent + 4].idxTI].iddatatype != IT::IDDATATYPE::INT &&
					    idTable.table[LexTable.table[state.posInLent + 4].idxTI].iddatatype != IT::IDDATATYPE::UBYTE) ||
					   (idTable.table[LexTable.table[state.posInLent + 6].idxTI].iddatatype != IT::IDDATATYPE::INT &&
					    idTable.table[LexTable.table[state.posInLent + 6].idxTI].iddatatype != IT::IDDATATYPE::UBYTE) ||
					   (idTable.table[LexTable.table[state.posInLent + 4].idxTI].idtype == IT::IDTYPE::F ||
						idTable.table[LexTable.table[state.posInLent + 6].idxTI].idtype == IT::IDTYPE::F))
						throw ERROR_THROW_IN(405, LexTable.table[state.posInLent].sn + 1, -1)
				}
			}

			//cheching for correct pushed params
			if (FUNC_CALL_NTERM == -rule.nTerm && state.posInLent > currentchechedPosForParams)
			{
				for (int i = state.posInLent, j = 0; LexTable.table[i].lexema != LEX_RIGHTHESIS; currentchechedPosForParams = i)
				{
					if (LexTable.table[i++].idxTI != -1)
					{
						switch (idTable.table[LexTable.table[state.posInLent - 2].idxTI].funcParams[j++].type)
						{
							case IT::IDDATATYPE::INT:
							case IT::IDDATATYPE::UBYTE:
							{
								if (idTable.table[LexTable.table[i - 1].idxTI].iddatatype != IT::IDDATATYPE::INT &&
									idTable.table[LexTable.table[i - 1].idxTI].iddatatype != IT::IDDATATYPE::UBYTE)
									throw ERROR_THROW_IN(404, LexTable.table[i - 1].sn, -1);
								break;
							}
							case IT::IDDATATYPE::LOGIC:
							{
								if (idTable.table[LexTable.table[i - 1].idxTI].iddatatype != IT::IDDATATYPE::LOGIC)
									throw ERROR_THROW_IN(404, LexTable.table[i - 1].sn, -1);
								break;
							}
							case IT::IDDATATYPE::STR:
							{
								if (idTable.table[LexTable.table[i - 1].idxTI].iddatatype != IT::IDDATATYPE::STR)
									throw ERROR_THROW_IN(404, LexTable.table[i - 1].sn, -1);
								break;
							}
							default: break;
						}
					}
				}
			}

			//checking for correct condition(if 1 operand it need to be LOGIC type)
			if (state.nRule == 3 && (state.nRuleChain >= 2 && state.nRuleChain <= 5))
			{
				if(idTable.table[LexTable.table[state.posInLent].idxTI].iddatatype != IT::IDDATATYPE::LOGIC)
					throw ERROR_THROW_IN(414, LexTable.table[state.posInLent - 2].sn + 1, -1);
			}
			
		}

		//use polish notation
		int i = 0;
		alreadyCheckedPos = -1;
		for (const auto& state : storestate._Get_container())
		{
			if (alreadyCheckedPos > state.posInLent)
				continue;

			if (state.nRule == 6)
			{
				//for assigment
				if (LexTable.table[state.posInLent - 1].lexema == LEX_EQUAL_SIGN)
				{
					if (!PN::polishNotation(state.posInLent, LexTable, idTable))
						throw ERROR_THROW_IN(406, LexTable.table[state.posInLent].sn + 1, -1);
					continue;
				}

				//for cprint
				if (LexTable.table[state.posInLent - 1].lexema == LEX_PRINT ||
					LexTable.table[state.posInLent - 1].lexema == LEX_PRINTL)
				{
					if (!PN::polishNotation(state.posInLent, LexTable, idTable))
						throw ERROR_THROW_IN(406, LexTable.table[state.posInLent].sn + 1, -1);
					continue;
				}

				if (state.posInLent > alreadyCheckedPos)
				{
					i = state.posInLent;
					while (LexTable.table[i++].lexema != LEX_SEMICOLON)
						alreadyCheckedPos = i;
				}
			}
		}
}



