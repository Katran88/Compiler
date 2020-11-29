#include <ctime>
extern "C"
{
	char* __stdcall func_getDate(char* buf)
	{
		time_t rawtime;
		struct tm timeinfo;			//структура хранящая текущее время

		time(&rawtime);					//текущая дата в секундах
		localtime_s(&timeinfo, &rawtime);	//текущее локальное время, представленное в структуре
		strftime(buf, 100, "%d.%m.%Y", &timeinfo);
		return buf;
	}

	char* __stdcall func_getTime(char* buf)
	{
		time_t rawtime;
		struct tm timeinfo;			//структура хранящая текущее время

		time(&rawtime);					//текущая дата в секундах
		localtime_s(&timeinfo, &rawtime);	//текущее локальное время, представленное в структуре
		strftime(buf, 100, "%H:%M:%S", &timeinfo);
		return buf;
	}
}