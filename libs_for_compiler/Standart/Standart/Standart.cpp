#pragma warning(disable : 4996)
#include <iostream>
#include <time.h>

extern "C"
{
	void __stdcall concat1(char* dest, char* par1)
	{
		int k = 0;
		for (int i = 0; par1[i] != '\0'; i++)
			dest[k++] = par1[i];

		dest[k] = '\0';
	}

	void __stdcall concat2(char* dest, char* par1, char* par2)
	{
		int k = 0;
		for (int i = 0; par2[i] != '\0' && k < 100; i++)
			dest[k++] = par2[i];

		for (int i = 0; par1[i] != '\0' && k < 100; i++)
			dest[k++] = par1[i];

		if (k >= 100)
		{
			std::cout << "STR OVERFLOW EXCEPTION\n";
			dest[0] = '\0';
		}

		dest[k] = '\0';
	}

	void __stdcall concat3(char* dest, char* par1, char* par2, char* par3)
	{
		int k = 0;
		for (int i = 0; par3[i] != '\0' && k < 100; i++)
			dest[k++] = par3[i];

		for (int i = 0; par2[i] != '\0' && k < 100; i++)
			dest[k++] = par2[i];

		for (int i = 0; par1[i] != '\0' && k < 100; i++)
			dest[k++] = par1[i];

		if (k >= 100)
		{
			std::cout << "STR OVERFLOW EXCEPTION\n";
			dest[0] = '\0';
		}
		dest[k] = '\0';
	}

	void __stdcall concat4(char* dest, char* par1, char* par2, char* par3, char* par4)
	{
		int k = 0;
		for (int i = 0; par4[i] != '\0' && k < 100; i++)
			dest[k++] = par4[i];

		for (int i = 0; par3[i] != '\0' && k < 100; i++)
			dest[k++] = par3[i];

		for (int i = 0; par2[i] != '\0' && k < 100; i++)
			dest[k++] = par2[i];

		for (int i = 0; par1[i] != '\0' && k < 100; i++)
			dest[k++] = par1[i];

		if (k >= 100)
		{
			std::cout << "STR OVERFLOW EXCEPTION\n";
			dest[0] = '\0';
		}

		dest[k] = '\0';
	}

	void __stdcall cprint(char* str)
	{
		setlocale(LC_ALL, "Russian");
		std::cout << str;
	}

	void __stdcall cprintl(char* str)
	{
		setlocale(LC_ALL, "Russian");
		std::cout << str << std::endl;
	}

	int __stdcall func_strlength(char* str)
	{
		return strlen(str);
	}

	char* __stdcall func_inttostr(char* buf, int value)
	{
		setlocale(LC_ALL, "Russian");

		int i = 0;
		unsigned int k = 10;
		int temp = value;
		while (temp / k != 0)
		{
			temp /= 10;
			i++;
		}

		if (value < 0)
		{
			buf[0] = '-';
			value -= value * 2;
		}

		int z = i;
		do
		{
			buf[z] = value % 10 + '0';
			value /= 10;
			z--;
		} while (z >= 0 && value != 0);

		buf[i + 1] = '\0';

		return buf;
	}

	int __stdcall func_random(int a, int b)
	{
		srand(time(0));
		return rand() % (b - a + 1) + a;
	}

	int __stdcall func_strtoint(char* str)
	{
		int length = strlen(str) - 1;

		if (str[0] == '-')
			length--;
		int rc = 0;
		for (int i = 0; i < strlen(str); i++)
		{
			if (str[i] >= '0' && str[i] <= '9')
				rc += (int(str[i]) - '0') * pow(10, length--);
			else
			{
				if (i == 0 && str[i] == '-')
					continue;
				return 0;
			}
		}

		if (str[0] == '-')
			rc *= -1;

		return rc;
	}
}