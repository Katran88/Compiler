.586							; система команд (процессор Pentium)
.model flat, stdcall			; модель памяти, соглашение о вызовах
includelib kernel32.lib			; компановщику: компоновать с kernel32

ExitProcess PROTO: DWORD		; прототип функции для завершения процесса Windows

;-----------console-----------
GetStdHandle PROTO: DWORD	; получить handle вывода на консоль
WriteConsoleA PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD		;вывод на консоль(стандартная ф - ия)
SetConsoleTitleA PROTO: DWORD		; прототип ф-ии устанавливающей заголовок консольного окна
;-----------------------------

;-----------DateTime-----------
includelib ..\Debug\DateTimelib.lib
getDate PROTO	; Возвращает текущую, локальную дату в виде строки
getTime PROTO	; Возвращает текущее, локальную время в виде строки
;------------------------------

.stack 4096

;-----------Literals and constants-----------
.const

consoleTitle byte 'HDV2019', 0

L1	ubyte	7	 ; ubyte literal
L3	ubyte	122	 ; ubyte literal
L5	dword	2593	 ; integer literal
L7	ubyte	200	 ; ubyte literal
L8	dword	-1	 ; integer literal
L10	byte	1	 ; logic literal
L13	byte	'привет мир!', 89 dup(0)	 ; string literal
L27	ubyte	1	 ; ubyte literal
L28	ubyte	5	 ; ubyte literal
L31	ubyte	10	 ; ubyte literal
L36	byte	'1234567890', 90 dup(0)	 ; string literal
L37	byte	'контрольный пример', 82 dup(0)	 ; string literal
;--------------------------------------------
