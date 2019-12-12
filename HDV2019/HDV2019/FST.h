#include <iostream>
namespace FST
{
	struct RELATION
	{
		char transitionSymbol; // ������ ��������
		short nnode; // ����� ������� �������
		RELATION(char transitionSymbol = 0x00, short newState = NULL);
	};

	struct NODE
	{
		short relationCounter; // ��-�� ����
		RELATION* relations; // ����
		NODE();
		NODE(short relationCount, RELATION rel, ...);
	};

	struct FST
	{
		const char* string; /*�������*/
		short position; /*������� � �������*/
		short statesCounter; /*���-�� ���������*/
		NODE* nodes; /*���� ��������*/
		short* rstates; /*��������� ���������*/
		FST();
		FST(const char* s, short statesCounter, NODE states, ...);
	};

	bool step(FST& fst, short* rstates); // ������ ������ �������
	bool execute(FST& fsts /*�������� �������*/);  //������������ �������
}


#pragma region automats

#pragma region identificator

		#define A_IDENTIFICATOR(string) string, 1,	\
			FST::NODE(53,	FST::RELATION('a', 0),	\
							FST::RELATION('b', 0),	\
							FST::RELATION('c', 0),	\
							FST::RELATION('d', 0),	\
							FST::RELATION('e', 0),	\
							FST::RELATION('f', 0),	\
							FST::RELATION('g', 0),	\
							FST::RELATION('h', 0),	\
							FST::RELATION('i', 0),	\
							FST::RELATION('j', 0),	\
							FST::RELATION('k', 0),	\
							FST::RELATION('l', 0),	\
							FST::RELATION('m', 0),	\
							FST::RELATION('n', 0),	\
							FST::RELATION('o', 0),	\
							FST::RELATION('p', 0),	\
							FST::RELATION('q', 0),	\
							FST::RELATION('r', 0),	\
							FST::RELATION('s', 0),	\
							FST::RELATION('t', 0),	\
							FST::RELATION('u', 0),	\
							FST::RELATION('v', 0),	\
							FST::RELATION('w', 0),	\
							FST::RELATION('x', 0),	\
							FST::RELATION('y', 0),	\
							FST::RELATION('z', 0),	\
							FST::RELATION('A', 0),	\
							FST::RELATION('B', 0),	\
							FST::RELATION('C', 0),	\
							FST::RELATION('D', 0),	\
							FST::RELATION('E', 0),	\
							FST::RELATION('F', 0),	\
							FST::RELATION('G', 0),	\
							FST::RELATION('H', 0),	\
							FST::RELATION('I', 0),	\
							FST::RELATION('J', 0),	\
							FST::RELATION('K', 0),	\
							FST::RELATION('L', 0),	\
							FST::RELATION('M', 0),	\
							FST::RELATION('N', 0),	\
							FST::RELATION('O', 0),	\
							FST::RELATION('P', 0),	\
							FST::RELATION('Q', 0),	\
							FST::RELATION('R', 0),	\
							FST::RELATION('S', 0),	\
							FST::RELATION('T', 0),	\
							FST::RELATION('U', 0),	\
							FST::RELATION('V', 0),	\
							FST::RELATION('W', 0),	\
							FST::RELATION('X', 0),	\
							FST::RELATION('Y', 0),	\
							FST::RELATION('Z', 0),	\
							FST::RELATION('_', 0)),	\
			FST::NODE()			

#pragma endregion

#pragma region STRING_LITERAL

#define STRING_LITERAL(string) string, 3,	\
	FST::NODE(1, FST::RELATION('\"', 1)),	\
	FST::NODE(147,	FST::RELATION(' ', 1),	\
					FST::RELATION('a', 1),	\
					FST::RELATION('b', 1),	\
					FST::RELATION('c', 1),	\
					FST::RELATION('d', 1),	\
					FST::RELATION('e', 1),	\
					FST::RELATION('f', 1),	\
					FST::RELATION('g', 1),	\
					FST::RELATION('h', 1),	\
					FST::RELATION('i', 1),	\
					FST::RELATION('j', 1),	\
					FST::RELATION('k', 1),	\
					FST::RELATION('l', 1),	\
					FST::RELATION('m', 1),	\
					FST::RELATION('n', 1),	\
					FST::RELATION('o', 1),	\
					FST::RELATION('p', 1),	\
					FST::RELATION('q', 1),	\
					FST::RELATION('r', 1),	\
					FST::RELATION('s', 1),	\
					FST::RELATION('t', 1),	\
					FST::RELATION('u', 1),	\
					FST::RELATION('v', 1),	\
					FST::RELATION('w', 1),	\
					FST::RELATION('x', 1),	\
					FST::RELATION('y', 1),	\
					FST::RELATION('z', 1),	\
					FST::RELATION('A', 1),	\
					FST::RELATION('B', 1),	\
					FST::RELATION('C', 1),	\
					FST::RELATION('D', 1),	\
					FST::RELATION('E', 1),	\
					FST::RELATION('F', 1),	\
					FST::RELATION('G', 1),	\
					FST::RELATION('H', 1),	\
					FST::RELATION('I', 1),	\
					FST::RELATION('J', 1),	\
					FST::RELATION('K', 1),	\
					FST::RELATION('L', 1),	\
					FST::RELATION('M', 1),	\
					FST::RELATION('N', 1),	\
					FST::RELATION('O', 1),	\
					FST::RELATION('P', 1),	\
					FST::RELATION('Q', 1),	\
					FST::RELATION('R', 1),	\
					FST::RELATION('S', 1),	\
					FST::RELATION('T', 1),	\
					FST::RELATION('U', 1),	\
					FST::RELATION('V', 1),	\
					FST::RELATION('W', 1),	\
					FST::RELATION('X', 1),	\
					FST::RELATION('Y', 1),	\
					FST::RELATION('Z', 1),	\
					FST::RELATION('0', 1),	\
					FST::RELATION('1', 1),	\
					FST::RELATION('2', 1),	\
					FST::RELATION('3', 1),	\
					FST::RELATION('4', 1),	\
					FST::RELATION('5', 1),	\
					FST::RELATION('6', 1),	\
					FST::RELATION('7', 1),	\
					FST::RELATION('8', 1),	\
					FST::RELATION('9', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('�', 1),	\
					FST::RELATION('=', 1),	\
					FST::RELATION('+', 1),	\
					FST::RELATION('-', 1),	\
					FST::RELATION('*', 1),	\
					FST::RELATION('/', 1),	\
					FST::RELATION('(', 1),	\
					FST::RELATION(')', 1),	\
					FST::RELATION('{', 1),	\
					FST::RELATION('}', 1),	\
					FST::RELATION(';', 1),	\
					FST::RELATION('!', 1),	\
					FST::RELATION(',', 1),	\
					FST::RELATION('\'', 1),	\
					FST::RELATION('<', 1),	\
					FST::RELATION('>', 1),	\
					FST::RELATION('?', 1),	\
					FST::RELATION('_', 1),	\
					FST::RELATION('\"', 2)),\
	FST::NODE()

#pragma endregion

#pragma region INTEGER_LITERAL

#define INTEGER_LITERAL(string) string, 2,	\
	FST::NODE(11,	FST::RELATION('0', 2),	\
					FST::RELATION('-', 1),	\
					FST::RELATION('1', 1),	\
					FST::RELATION('2', 1),	\
					FST::RELATION('3', 1),	\
					FST::RELATION('4', 1),	\
					FST::RELATION('5', 1),	\
					FST::RELATION('6', 1),	\
					FST::RELATION('7', 1),	\
					FST::RELATION('8', 1),	\
					FST::RELATION('9', 1)),	\
	FST::NODE(10,	FST::RELATION('0', 1),	\
					FST::RELATION('1', 1),	\
					FST::RELATION('2', 1),	\
					FST::RELATION('3', 1),	\
					FST::RELATION('4', 1),	\
					FST::RELATION('5', 1),	\
					FST::RELATION('6', 1),	\
					FST::RELATION('7', 1),	\
					FST::RELATION('8', 1),	\
					FST::RELATION('9', 1)),	\
	FST::NODE()

#pragma endregion

#pragma region BINARY_LITERAL

#define BINARY_LITERAL(string) string,	2,			\
	FST::NODE(1,	FST::RELATION('b', 1)),			\
	FST::NODE(2,	FST::RELATION('0', 1),			\
					FST::RELATION('1', 1)),			\
	FST::NODE()

#pragma endregion

#pragma region EIGHT_LITERAL

#define EIGHT_LITERAL(string) string,	2,			\
	FST::NODE(1,	FST::RELATION('e', 1)),			\
	FST::NODE(8,	FST::RELATION('0', 1),			\
					FST::RELATION('1', 1),			\
					FST::RELATION('2', 1),			\
					FST::RELATION('3', 1),			\
					FST::RELATION('4', 1),			\
					FST::RELATION('5', 1),			\
					FST::RELATION('6', 1),			\
					FST::RELATION('7', 1)),			\
	FST::NODE()

#pragma endregion

#pragma region HEX_LITERAL

#define HEX_LITERAL(string) string,	2,				\
	FST::NODE(1,	FST::RELATION('h', 1)),			\
	FST::NODE(16,	FST::RELATION('0', 1),			\
					FST::RELATION('1', 1),			\
					FST::RELATION('2', 1),			\
					FST::RELATION('3', 1),			\
					FST::RELATION('4', 1),			\
					FST::RELATION('5', 1),			\
					FST::RELATION('6', 1),			\
					FST::RELATION('7', 1),			\
					FST::RELATION('8', 1),			\
					FST::RELATION('9', 1),			\
					FST::RELATION('A', 1),			\
					FST::RELATION('B', 1),			\
					FST::RELATION('C', 1),			\
					FST::RELATION('D', 1),			\
					FST::RELATION('E', 1),			\
					FST::RELATION('F', 1)),			\
	FST::NODE()

#pragma endregion

#pragma endregion