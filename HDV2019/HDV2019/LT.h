#pragma once

#define	LT_MAXSIZE			1024		// ������������ ���������� ����� � ������� ������	
#define	LT_TI_NULLXDX		-1			// ��� �������� ������� ���������������				
#define	LEX_TYPE			't'			// ������� ��� ��� ������
#define	LEX_ID				'i'			// ������� ��� ��������������
#define	LEX_MAIN			'm'			// ������� ��� ������� �������
#define LEX_IF				'c'			// ������� ��� ����� if
#define	LEX_GLOBAL			'g'			// ������� ��� ����������� ���������� ����������
#define	LEX_LITERAL			'l'			// ������� ��� ��������
#define	LEX_FUNCTION		'f'			// ������� ��� func
#define	LEX_RETURN			'r'			// ������� ��� return
#define	LEX_PRINT			'p'			// ������� ��� cprint
#define	LEX_INCLUDE			'e'			// ������� ��� _connect
#define	LEX_LIBRARY			'y'			// ������� ��� libName
#define	LEX_LOOP			'z'			// ������� ��� loop
#define	LEX_TILDA			'~'			// ������� ��� ~ (��������� �������� ��������� �����)
#define	LEX_SEMICOLON		';'			// ������� ��� ;
#define	LEX_COMMA			','			// ������� ��� ,
#define	LEX_LEFTBRACE		'{'			// ������� ��� {
#define	LEX_RIGHTBRACE		'}'			// ������� ��� }
#define	LEX_LEFTHESIS		'('			// ������� ��� (
#define	LEX_RIGHTHESIS		')'			// ������� ��� )
#define	LEX_PLUS			'+'			// ������� ��� +
#define	LEX_MINUS			'-'			// ������� ��� -
#define	LEX_STAR			'*'			// ������� ��� *
#define	LEX_DIRSLASH		'/'			// ������� ��� /
#define	LEX_EQUAL_SIGN		'='			// ������� ��� =
#define	LEX_LESS_SIGN		'<'			// ������� ��� <
#define	LEX_MORE_SIGN		'>'			// ������� ��� >
#define	LEX_SAME_SIGN		's'			// ������� ��� ==
#define	LEX_DIFFERENT_SIGN	'd'			// ������� ��� !=
#define	LEX_MORE_SAME_SIGN	'b'			// ������� ��� >=
#define	LEX_LESS_SAME_SIGN	'w'			// ������� ��� <=
#define	LEX_OR_SIGN			'|'			// ������� ��� or
#define	LEX_AND_SIGN		'&'			// ������� ��� and
#define	LEX_NOT_SIGN		'!'			// ������� ��� !

#define libName "DateTime"

#define PARM_LEX_DEFAULT_EXT L".lex.txt" //��� ����� � ������ ������������ ������a
// ������� ������
namespace LT
{
	struct Entry						// ������ ������� ������
	{
		char lexema;					// ������
		int sn;							// ����� ������ � �������� ������
		int idxTI;						// ������ � ������� ��������������� ��� LT_TI_NULLIDX
		Entry();
		Entry(const char lex, int str_n, int idxTI);
	};

	struct LexTable						// ��������� ������� ������
	{
		int maxsize;					// ������� ������� ������ < LT_MAXSIZE
		int current_size;				// ������� ������ ������� ������ < maxsize
		Entry* table;					// ������ ����� ������� ������

		LexTable();

		void Add(Entry entry);					// ���������� ������
		Entry GetEntry(int n);					// �������� ������ ������� ������
		void PrintLexTable(const wchar_t* in);		// ����� ������ � ����
		void Delete();							// ������� ������� ������ (���������� ������)
	};
}