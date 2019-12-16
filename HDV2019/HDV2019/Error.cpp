#include "Error.h"

namespace Error
{
	// серии ошибок: 0-99 - системные ошибки
	//				 100-109 - ошибки с файлами ввода/вывода
	//				 110-119 - ошибки открытия и чтения файлов
	//				 120-129 - лексические ошибки
	//				 130-139 - синтаксические ошибки
	//				 400-415 - семантические ошибки
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"),
		ERROR_ENTRY(1, "Системный сбой"),

		ERROR_ENTRY_NODEF(2),		ERROR_ENTRY_NODEF(3),		ERROR_ENTRY_NODEF(4),
		ERROR_ENTRY_NODEF(5),		ERROR_ENTRY_NODEF(6),		ERROR_ENTRY_NODEF(7),
		ERROR_ENTRY_NODEF(8),		ERROR_ENTRY_NODEF(9),		ERROR_ENTRY_NODEF10(10),
		ERROR_ENTRY_NODEF10(20),	ERROR_ENTRY_NODEF10(30),	ERROR_ENTRY_NODEF10(40),
		ERROR_ENTRY_NODEF10(50),	ERROR_ENTRY_NODEF10(60),	ERROR_ENTRY_NODEF10(70),
		ERROR_ENTRY_NODEF10(80),	ERROR_ENTRY_NODEF10(90),

		ERROR_ENTRY(100, "Параметр -in должен быть задан"),
		ERROR_ENTRY(101, "Не удалось открыть файл для генерации кода"),
		ERROR_ENTRY_NODEF(102),		ERROR_ENTRY_NODEF(103),

		ERROR_ENTRY(104, "Превышена длина входного параметра"),

		ERROR_ENTRY_NODEF(105),		ERROR_ENTRY_NODEF(106),		ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108),		ERROR_ENTRY_NODEF(109),

		ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111, "Недопустимый символ в исходном коде (-in)"),
		ERROR_ENTRY(112, "Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY(113, "Файл, переданный как -in:, пуст"),
		ERROR_ENTRY_NODEF(114),		ERROR_ENTRY_NODEF(115),
		ERROR_ENTRY_NODEF(116),		ERROR_ENTRY_NODEF(117),
		ERROR_ENTRY_NODEF(118),		ERROR_ENTRY_NODEF(119),

		ERROR_ENTRY(120, "Таблица лексем переполнена."),
		ERROR_ENTRY(121, "Неораспознанная лексема"),
		ERROR_ENTRY(122, "Таблица идентификаторов переполнена."),
		ERROR_ENTRY(123, "Перезапись идентификатора"),
		ERROR_ENTRY(124, "Лексическая ошибка"),
		ERROR_ENTRY(125, "Не удалось создать файл с лексемами(или идентификаторами)"),
		ERROR_ENTRY(126, "Слишком длинный литерал"),
		ERROR_ENTRY(127, "Неверный формат строкового литерала"),
		ERROR_ENTRY(128, "Слишком большое имя переменной"),
		ERROR_ENTRY(129, "Неизвестная переменная"),
		ERROR_ENTRY(130, "Неверная структура программы"),
		ERROR_ENTRY(131, "Ошибочный операнд в выражении"),
		ERROR_ENTRY(132, "Ошибка в выражении"),
		ERROR_ENTRY(133, "Ошибка в параметрах функции"),
		ERROR_ENTRY(134, "Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY(135, "Ошибочный оператор в выражении"),
		ERROR_ENTRY(136, "Ошибка в условии входа в блок"),
		ERROR_ENTRY(137, "Ошибочный оператор в условии входа в блок"),
		ERROR_ENTRY(138, "Неверный логический оператор"),
		ERROR_ENTRY(139, "Использована рарезервированная последовательность символов"),
		ERROR_ENTRY_NODEF10(140),	ERROR_ENTRY_NODEF10(150),	
		ERROR_ENTRY_NODEF10(160),	ERROR_ENTRY_NODEF10(170),	
		ERROR_ENTRY_NODEF10(180),	ERROR_ENTRY_NODEF10(190),	
		ERROR_ENTRY_NODEF100(200),
		ERROR_ENTRY_NODEF100(300),
		ERROR_ENTRY(400, "Невозможно преобразовать тип правостороннего операнда к типу левостороннего"),
		ERROR_ENTRY(401, "Отсутствует точка входа в программу(main)"),
		ERROR_ENTRY(402, "Неверный тип возвращаемого значения"),
		ERROR_ENTRY(403, "Превышено максимальное количество параметров функции"),
		ERROR_ENTRY(404, "Несооответствие типов формальных и фактических параметров при вызове функции"),
		ERROR_ENTRY(405, "Промежуток значений итератора цикла задан неверно"),
		ERROR_ENTRY(406, "Ошибка в структуре выражения"),
		ERROR_ENTRY(407, "Вызов неизвестной функции"),
		ERROR_ENTRY(408, "Ипользован запрещенный оператор с данными операндами"),
		ERROR_ENTRY(409, "Выход за диапазон допустимых целочисленных значений"),
		ERROR_ENTRY(410, "Выход за диапазон допустимого размера для STR"),
		ERROR_ENTRY(411, "Присваивать какое-либо значение можно только переменной"),
		ERROR_ENTRY(412, "За одно присваивание можно осуществить конкатенацию максимум 4х строк"),
		ERROR_ENTRY(413, "cprint принимает только тип данных str"),
		ERROR_ENTRY(414, "Неверно задано условие для входа в блок if"),
		ERROR_ENTRY(415, "В программе может быть только одна точка входа(main)"),
		ERROR_ENTRY_NODEF(416),
		ERROR_ENTRY_NODEF(417),		ERROR_ENTRY_NODEF(418),
		ERROR_ENTRY_NODEF(419),
		ERROR_ENTRY_NODEF10(420),	ERROR_ENTRY_NODEF10(430),
		ERROR_ENTRY_NODEF10(440),	ERROR_ENTRY_NODEF10(450),
		ERROR_ENTRY_NODEF10(460),	ERROR_ENTRY_NODEF10(470),
		ERROR_ENTRY_NODEF10(480),	ERROR_ENTRY_NODEF10(490),
		ERROR_ENTRY_NODEF100(500),	ERROR_ENTRY_NODEF100(600),
		ERROR_ENTRY_NODEF100(700),	ERROR_ENTRY_NODEF100(800),
		ERROR_ENTRY_NODEF100(900)
	};

	ERROR geterror(int id)
	{
		if (id > 0 && id < ERROR_MAX_ENTRY)
			return errors[id];
		else
			return errors[0];
	}

	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		if (id > 0 && id < ERROR_MAX_ENTRY)
		{
			errors[id].errorPosition.col = col;
			errors[id].errorPosition.line = line;
			return errors[id];
		}
		else
			return errors[0];
	}
}