#include "Error.h"

namespace Error
{
	// ñåğèè îøèáîê: 0-99 - ñèñòåìíûå îøèáêè
	//				 100-109 - îøèáêè ïàğàìåòğîâ êîìàíäíîé ñòğîêè
	//				 110-119 - îøèáêè îòêğûòèÿ è ÷òåíèÿ ôàéëîâ
	//				 120-129 - ëåêñè÷åñêèå îøèáêè
	//				 130-139 - ñèíòàêñè÷åñêèå îøèáêè
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Íåäîïóñòèìûé êîä îøèáêè"),
		ERROR_ENTRY(1, "Ñèñòåìíûé ñáîé"),

		ERROR_ENTRY_NODEF(2),		ERROR_ENTRY_NODEF(3),		ERROR_ENTRY_NODEF(4),
		ERROR_ENTRY_NODEF(5),		ERROR_ENTRY_NODEF(6),		ERROR_ENTRY_NODEF(7),
		ERROR_ENTRY_NODEF(8),		ERROR_ENTRY_NODEF(9),		ERROR_ENTRY_NODEF10(10),
		ERROR_ENTRY_NODEF10(20),	ERROR_ENTRY_NODEF10(30),	ERROR_ENTRY_NODEF10(40),
		ERROR_ENTRY_NODEF10(50),	ERROR_ENTRY_NODEF10(60),	ERROR_ENTRY_NODEF10(70),
		ERROR_ENTRY_NODEF10(80),	ERROR_ENTRY_NODEF10(90),

		ERROR_ENTRY(100, "Ïàğàìåòğ -in äîëæåí áûòü çàäàí"),

		ERROR_ENTRY_NODEF(101),		ERROR_ENTRY_NODEF(102),		ERROR_ENTRY_NODEF(103),

		ERROR_ENTRY(104, "Ïğåâûøåíà äëèíà âõîäíîãî ïàğàìåòğà"),

		ERROR_ENTRY_NODEF(105),		ERROR_ENTRY_NODEF(106),		ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108),		ERROR_ENTRY_NODEF(109),

		ERROR_ENTRY(110, "Îøèáêà ïğè îòêğûòèè ôàéëà ñ èñõîäíûì êîäîì (-in)"),
		ERROR_ENTRY(111, "Íåäîïóñòèìûé ñèìâîë â èñõîäíîì êîäå (-in)"),
		ERROR_ENTRY(112, "Îøèáêà ïğè ñîçäàíèè ôàéëà ïğîòîêîëà (-log)"),
		ERROR_ENTRY(113, "Ôàéë, ïåğåäàííûé êàê -in:, ïóñò"),
		ERROR_ENTRY_NODEF(114),		ERROR_ENTRY_NODEF(115),
		ERROR_ENTRY_NODEF(116),		ERROR_ENTRY_NODEF(117),
		ERROR_ENTRY_NODEF(118),		ERROR_ENTRY_NODEF(119),

		ERROR_ENTRY(120, "Òàáëèöà ëåêñåì ïåğåïîëíåíà."),
		ERROR_ENTRY(121, "Íåîğàñïîçíàííàÿ ëåêñåìà"),
		ERROR_ENTRY(122, "Òàáëèöà èäåíòèôèêàòîğîâ ïåğåïîëíåíà."),
		ERROR_ENTRY(123, "Ïåğåçàïèñü èäåíòèôèêàòîğà"),
		ERROR_ENTRY(124, "Ëåêñè÷åñêàÿ îøèáêà"),
		ERROR_ENTRY(125, "Íå óäàëîñü ñîçäàòü ôàéë ñ ëåêñåìàìè(èëè èäåíòèôèêàòîğàìè)"),
		ERROR_ENTRY(126, "Ñëèøêîì äëèííûé ëèòåğàë"),
		ERROR_ENTRY(127, "Íåâåğíûé ôîğìàò ñòğîêîâîãî ëèòåğàëà"),
		ERROR_ENTRY(128, "Ñëèøêîì áîëüøîå èìÿ ïåğåìåííîé"),
		ERROR_ENTRY(129, "Íåèçâåñòíàÿ ïåğåìåííàÿ"),

		ERROR_ENTRY(130, "Íåâåğíàÿ ñòğóêòóğà ïğîãğàììû"),
		ERROR_ENTRY(131, "Îøèáî÷íûé îïåğàíä â âûğàæåíèè"),
		ERROR_ENTRY(132, "Îøèáêà â âûğàæåíèè"),
		ERROR_ENTRY(133, "Îøèáêà â ïàğàìåòğàõ ôóíêöèè"),
		ERROR_ENTRY(134, "Îøèáêà â ïàğàìåòğàõ âûçûûâàåìîé ôóíêöèè"),
		ERROR_ENTRY(135, "Îøèáî÷íûé îïåğàòîğ â âûğàæåíèè"),
		ERROR_ENTRY(136, "Îøèáêà â óñëîâèè âõîäà â áëîê"),
		ERROR_ENTRY(137, "Îøèáî÷íûé îïåğàòîğ â óñëîâèè âõîäà â áëîê"),
		ERROR_ENTRY(138, "Íåâåğíûé ëîãè÷åñêèé îïåğàòîğ"),
		ERROR_ENTRY(139, "Èñïîëüçîâàíà ğàğåçåğâèğîâàííàÿ ïîñëåäîâàòåëüíîñòü ñèìâîëîâ"),
		ERROR_ENTRY_NODEF10(140),	ERROR_ENTRY_NODEF10(150),	
		ERROR_ENTRY_NODEF10(160),	ERROR_ENTRY_NODEF10(170),	
		ERROR_ENTRY_NODEF10(180),	ERROR_ENTRY_NODEF10(190),	
		ERROR_ENTRY_NODEF100(200),
		ERROR_ENTRY_NODEF100(300),
		ERROR_ENTRY(400, "Íåâîçìîæíî ïğåîáğàçîâàòü òèï ïğàâîñòîğîííåãî îïåğàíäà ê òèïó ëåâîñòîğîííåãî"),
		ERROR_ENTRY(401, "Îòñóòñòâóåò òî÷êà âõîäà â ïğîãğàììó(main)"),
		ERROR_ENTRY(402, "Íåâåğíûé òèï âîçâğàùàåìîãî çíà÷åíèÿ"),
		ERROR_ENTRY(403, "Ïğåâûøåíî ìàêñèìàëüíîå êîëè÷åñòâî ïàğàìåòğîâ ôóíêöèè"),
		ERROR_ENTRY(404, "Íåñîîîòâåòñòâèå òèïîâ ôîğìàëüíûõ è ôàêòè÷åñêèõ ïàğàìåòğîâ ïğè âûçîâå ôóíêöèè"),
		ERROR_ENTRY(405, "Ïğîìåæóòîê çíà÷åíèé èòåğàòîğà öèêëà çàäàí íåâåğíî"),
		ERROR_ENTRY_NODEF(406),
		ERROR_ENTRY_NODEF(407),
		ERROR_ENTRY_NODEF(408),
		ERROR_ENTRY_NODEF(409),
		ERROR_ENTRY_NODEF10(410),
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