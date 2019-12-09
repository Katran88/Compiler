.586							; система команд (процессор Pentium)
.model flat, stdcall			; модель памяти, соглашение о вызовах
includelib kernel32.lib			; компановщику: компоновать с kernel32
includelib libucrt.lib
ExitProcess PROTO: DWORD		; прототип функции для завершения процесса Windows

;-----------console-----------
GetStdHandle PROTO: DWORD	; получить handle вывода на консоль
WriteConsoleA PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD		;вывод на консоль(стандартная ф - ия)
SetConsoleTitleA PROTO: DWORD		; прототип ф-ии устанавливающей заголовок консольного окна
;-----------------------------

;-----------Standart functions-----------
includelib ../Standart.lib
concat1 PROTO : dword, :dword
concat2 PROTO : dword, :dword, :dword
concat3 PROTO : dword, :dword, :dword, :dword
concat4 PROTO : dword, :dword, :dword, :dword, :dword
;----------------------------------------

;-----------DateTime-----------
includelib ../DateTimelib.lib
getDate PROTO	; Возвращает текущую, локальную дату в виде строки
getTime PROTO	; Возвращает текущее, локальную время в виде строки
;------------------------------

.stack 4096

;-----------Literals and constants-----------
.const

consoleTitle byte 'HDV2019', 0
INTOVERFLOW_text byte 'INT OVERFLOW EXCEPTION', 10, 0
UBYTEOVERFLOW_text byte 'UBYTE OVERFLOW EXCEPTION', 10, 0
DIVISION_BY_ZERO_text byte 'DIVISION BY ZERO', 10, 0

L1	byte	7	 ; ubyte literal
L3	byte	122	 ; ubyte literal
L5	sdword	2593	 ; integer literal
L7	byte	200	 ; ubyte literal
L8	sdword	-1	 ; integer literal
L10	byte	1	 ; logic literal
L12	byte	'test', 96 dup(0)	 ; string literal
L16	byte	'привет мир!', 89 dup(0)	 ; string literal
L22	byte	10	 ; ubyte literal
L31	byte	1	 ; ubyte literal
L32	byte	5	 ; ubyte literal
L39	byte	'1234567890', 90 dup(0)	 ; string literal
L40	byte	11	 ; ubyte literal
L41	byte	'контрольный пример', 82 dup(0)	 ; string literal
;--------------------------------------------


;----------------Variables-------------------
.data

consoleHandle dd 0h	; состояние консоли

global_binary	byte	0	 ; ubyte var
global_eight	byte	0	 ; ubyte var
global_hex	byte	0	 ; ubyte var
global_test	byte	0	 ; ubyte var
global_test_logic	byte	0	 ; logic var
global_test_str	byte	100 dup(0)	 ; string var
hello_world_k	sdword	0	 ; integer var
hello_world_a	byte	100 dup(0)	 ; string var
fi_x	sdword	0	 ; integer var
fi_y	byte	0	 ; ubyte var
fi_k	byte	100 dup(0)	 ; string var
fi_z	sdword	0	 ; integer var
main_x	sdword	0	 ; integer var
main_y	sdword	0	 ; integer var
main_z	sdword	0	 ; integer var
main_sa	byte	100 dup(0)	 ; string var
main_sb	byte	100 dup(0)	 ; string var
main_sc	byte	100 dup(0)	 ; string var
loop1_i	sdword	0	 ; integer var
if1_p	sdword	0	 ; integer var
if2_u	sdword	0	 ; integer var
;---------------------------------------------

str_var	byte 'аа', 0
str_var2 byte 'бб', 0
str_var3 byte 'вв', 0
.code

;----------------Functions-------------------

main proc

main endp  
end main  