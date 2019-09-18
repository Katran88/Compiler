#include "stdafx.h"
#include <vector>

void wmain(int argc, wchar_t* argv[])
{
	setlocale(LC_ALL, "Russian");

	Log::LOG log;

	try
	{
		Parm::PARM param = Parm::getparm(argc, argv);
		log = Log::getlog(param.log);
		Log::WriteLine(log, "Тест: ", "без ошибок\n", "");
		Log::WriteLog(log);
		Log::WriteParm(log, param);
		In::IN in = In::getin(param.in);
		Log::WriteIn(log, in);
		LT::LexTable lexTable;
		IT::IdTable	idTable;

		divisionIntoTokens(in, lexTable, idTable);
		lexTable.PrintLexTable(param.in);
		idTable.PrintIdTable(param.in);


		lexTable.Delete();
		idTable.Delete();
		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
		Log::Close(log);
	}
}