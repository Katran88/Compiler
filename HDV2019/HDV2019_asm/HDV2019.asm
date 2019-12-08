.586							; система команд (процессор Pentium)
.model flat, stdcall			; модель памяти, соглашение о вызовах
includelib kernel32.lib			; компановщику: компоновать с kernel32

ExitProcess PROTO: DWORD		; прототип функции для завершения процесса Windows

;-----------console-----------
GetStdHandle PROTO: DWORD	; получить handle вывода на консоль
WriteConsoleA PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD		;вывод на консоль(стандартная ф - ия)
SetConsoleTitleA PROTO: DWORD		; прототип ф-ии устанавливающей заголовок консольного окна
;-----------------------------

;-----------Standart functions-----------
includelib ..\Standart.lib
concat PROTO: DWORD,: DWORD
;----------------------------------------

;-----------DateTime-----------
includelib ..\DateTimelib.lib
getDate PROTO	; Возвращает текущую, локальную дату в виде строки
getTime PROTO	; Возвращает текущее, локальную время в виде строки
;------------------------------

.stack 4096

;-----------Literals and constants-----------
.const

consoleTitle byte 'HDV2019', 0
INTOVERFLOW_text byte 'INT OVERFLOW EXCEPTION', 10, 0
UBYTEOVERFLOW_text byte 'UBYTE OVERFLOW EXCEPTION', 10, 0

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

V0	byte	0	 ; ubyte var
V2	byte	0	 ; ubyte var
V4	byte	0	 ; ubyte var
V6	byte	0	 ; ubyte var
V9	byte	0	 ; logic var
V11	byte	100 dup(0)	 ; string var
V15	byte	100 dup(0)	 ; string var
V21	byte	100 dup(0)	 ; string var
V23	sdword	0	 ; integer var
V25	sdword	0	 ; integer var
V26	sdword	0	 ; integer var
V27	sdword	0	 ; integer var
V28	byte	100 dup(0)	 ; string var
V29	byte	100 dup(0)	 ; string var
V30	byte	100 dup(0)	 ; string var
V34	sdword	0	 ; integer var
V36	sdword	0	 ; integer var
V38	sdword	0	 ; integer var
;---------------------------------------------

a dword 10
b dword 0

.code

main proc 

mov eax, a
mov ebx, 1

cdq

cmp ebx, 0

jne NOzero
idiv ebx

NOzero:
push ebx

main endp 
end main
