.586							; система команд (процессор Pentium)
.model flat, stdcall			; модель памяти, соглашение о вызовах
includelib kernel32.lib			; компановщику: компоновать с kernel32
includelib libucrt.lib			; для UCRT - universal C Run-Time library

ExitProcess PROTO: DWORD		; прототип функции для завершения процесса Windows

;-----------console-----------
GetStdHandle PROTO : DWORD	; получить handle вывода на консоль
WriteConsoleA PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD		;вывод на консоль(стандартная ф - ия)
SetConsoleTitleA PROTO: DWORD		; прототип ф-ии устанавливающей заголовок консольного окна
;-----------------------------

;-----------Standart functions-----------
includelib ..\Standart.lib
concat1 PROTO : DWORD, : DWORD
concat2 PROTO : DWORD, : DWORD, : DWORD
concat3 PROTO : DWORD, : DWORD, : DWORD, : DWORD
concat4 PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD
cprint PROTO : DWORD
cprintl PROTO : DWORD
strlength PROTO : DWORD
inttostr PROTO : DWORD , : SDWORD
;----------------------------------------

;-----------DateTime-----------
includelib ..\DateTimelib.lib
getDate PROTO : DWORD	; Возвращает текущую, локальную дату в виде строки
getTime PROTO : DWORD	; Возвращает текущее, локальную время в виде строки
;------------------------------

.stack 4096

;-----------Literals and constants-----------
.const

consoleTitle byte 'HDV2019', 0
INTOVERFLOW_text byte 'INT OVERFLOW EXCEPTION', 10, 0
UBYTEOVERFLOW_text byte 'UBYTE OVERFLOW EXCEPTION', 10, 0
DIVISION_BY_ZERO_text byte 'DIVISION BY ZERO', 10, 0

STROVERFLOW_TEXT byte 'STR OVERFLOW', 0

L2	byte	1	 ; logic literal
L6	byte	7	 ; ubyte literal
L8	byte	122	 ; ubyte literal
L10	sdword	2593	 ; integer literal
L12	byte	200	 ; ubyte literal
L13	sdword	-1	 ; integer literal
L15	byte	0	 ; logic literal
L17	byte	"test", 96 dup(0)	 ; string literal
L18	byte	"binary ", 93 dup(0)	 ; string literal
L19	byte	"eight ", 94 dup(0)	 ; string literal
L20	byte	"hex ", 96 dup(0)	 ; string literal
L21	byte	"test ", 95 dup(0)	 ; string literal
;--------------------------------------------


;----------------Variables-------------------
.data

ifFlag byte 0 	; флаг для входа в блок if
cprint_var byte 100 dup(0)	; variable for cprint
cprintl_var byte 100 dup(0)	; variable for cprintl
global_binary	byte	0	 ; ubyte var
global_eight	byte	0	 ; ubyte var
global_hex	sdword	0	 ; integer var
global_test	sdword	0	 ; integer var
global_test_logic	byte	0	 ; logic var
global_test_str	byte	100 dup(0)	 ; string var
inttostr_47	byte	17 dup(0)	 ; return var
inttostr_55	byte	17 dup(0)	 ; return var
inttostr_63	byte	17 dup(0)	 ; return var
inttostr_70	byte	17 dup(0)	 ; return var
;---------------------------------------------


.code


;----------------Functions-------------------


push 0
call ExitProcess

main endp
end main