#include <ctime>
extern "C"
{
	char* __stdcall func_getDate(char* buf)
	{
		time_t rawtime;
		struct tm timeinfo;			//��������� �������� ������� �����

		time(&rawtime);					//������� ���� � ��������
		localtime_s(&timeinfo, &rawtime);	//������� ��������� �����, �������������� � ���������
		strftime(buf, 100, "%d.%m.%Y", &timeinfo);
		return buf;
	}

	char* __stdcall func_getTime(char* buf)
	{
		time_t rawtime;
		struct tm timeinfo;			//��������� �������� ������� �����

		time(&rawtime);					//������� ���� � ��������
		localtime_s(&timeinfo, &rawtime);	//������� ��������� �����, �������������� � ���������
		strftime(buf, 100, "%H:%M:%S", &timeinfo);
		return buf;
	}
}