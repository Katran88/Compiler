#pragma once
#include "MFST.h"
#include "IT.h"

void SemantikAnaliz(std::stack<MFST::MFSTState>& storestate, GRB::Greibach& grebach, LT::LexTable& LexTable, IT::IdTable& idTable);
