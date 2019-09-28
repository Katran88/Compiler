#include "stdafx.h"
#include <ctime>
#include <chrono>

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

		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		divisionIntoTokens(in, lexTable, idTable);
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

		std::cout << time_span.count() << " seconds.";

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