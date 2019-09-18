#include <fstream>
#include <string>
#include "In.h"
#include "Error.h"

using namespace std;

namespace In
{
	IN getin(wchar_t *infile)
	{
		IN inner_text;

		ifstream file;
		file.open(infile);

		if(!file)
			throw ERROR_THROW(110);

		inner_text.infile = infile;

		char unsigned tempChar;
		int position = 0;

		bool flagEmptyFile = true;
		
		while ((tempChar = file.get()) && !file.eof())
		{
			flagEmptyFile = false;

			switch (inner_text.code[tempChar])
			{
				case IN::T:
				{
					inner_text.text[inner_text.size++] = tempChar;
					position++;
					break;
				}

				case IN::I:
				{
					inner_text.ignor++;
					position++;
					break;
				}

				case IN::F:
				{
					if (tempChar == IN_CODE_ENDL)
					{	
						inner_text.text[inner_text.size] = '\n';
						inner_text.size++;
						inner_text.lines++;
						position = 0;
						break;
					}
					throw ERROR_THROW_IN(111, inner_text.lines, position + 1);
					break;
				}

				case '-':
				{
					inner_text.text[inner_text.size++] = '-';
					position++;
					break;
				}

			}
		}

		if (flagEmptyFile)
			throw ERROR_THROW(113);

		if (inner_text.size == 0)
			inner_text.lines = 0;

		inner_text.text[inner_text.size] = '\0';
		file.close();
		return inner_text;
	}
}
