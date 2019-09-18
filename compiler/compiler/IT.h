#pragma once

#define ID_MAXSIZE		10				//������������ ���������� ������� � ��������������
#define TI_MAXSIZE		1024			// ������������ ���������� ��-�� � ������� ��������������� 
#define TI_INT_DEFAULT	0x00000000		// �������� �� ��������� ��� ���� integer 
#define TI_STR_DEFAULT	0x00			// �������� �� ��������� ��� ���� string 
#define TI_NULLIDX		-1				// ��� �������� ������� ���������������
#define TI_STR_MAXSIZE	255

#define PARM_ID_DEFAULT_EXT L".id.txt" //��� ����� � ������ ������������ ������a(�������������� � ��������)

namespace IT	// ������� �������������
{
	enum IDDATATYPE {DEF = 0, INT = 1, STR = 2 };		// ���� ������ �������������: integer, string
	enum IDTYPE {D = 0, V = 1, F = 2, P = 3, L = 4};			// ���� ���������������: ����������, �������, ��������, �������


	struct Entry						// ��-� ������� ���������������
	{
		char parrentFunc[ID_MAXSIZE];
		char id[ID_MAXSIZE];			// ������������� (������������� ��������� �� ID_MAXSIZE
		IDDATATYPE	iddatatype;			// ��� ������
		IDTYPE	idtype;					// ��� ������������
		union VALUE
		{
			int vint;					// �������� integer
			struct
			{
				unsigned char len;				// ��������� �������� � string
				char str[TI_STR_MAXSIZE]; // ������� string
			} vstr;	// �������� string
		}value;	// �������� ��������������

		Entry();
		Entry(const char* parrentFunc, const char* id, IDDATATYPE iddatatype, IDTYPE idtype);
	};

	struct IdTable // ��������� ������� ���������������
	{
		int maxsize;				// ������� ������� ��������������� < TI_MAXSIZE
		int current_size;			// ������� ������ ������� ��������������� < maxsize
		Entry* table;				// ������ ����� ������� ���������������
	
		IdTable();
		void Add(Entry entry);
		Entry GetEntry(int n);
		int IsId(const char id[ID_MAXSIZE]);
		int IsId(const char id[ID_MAXSIZE], const char parentFunc[ID_MAXSIZE]);
		int IsLit(const char lit[ID_MAXSIZE]);
		void PrintIdTable(const wchar_t* in);
		void Delete();
	};
}