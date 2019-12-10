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
DIVISION_BY_ZERO_text byte 'DIVISION BY ZERO', 10, 0

L1	byte	7	 ; ubyte literal
L3	byte	122	 ; ubyte literal
L5	sdword	2593	 ; integer literal
L7	byte	200	 ; ubyte literal
L8	sdword	-1	 ; integer literal
L10	byte	1	 ; logic literal
L12	byte	'test', 96 dup(0)	 ; string literal
L16	byte	'привет мир!', 89 dup(0)	 ; string literal
L19	byte	1	 ; ubyte literal
L20	byte	10	 ; ubyte literal
L38	byte	5	 ; ubyte literal
L42	byte	'1234567890', 90 dup(0)	 ; string literal
L43	byte	11	 ; ubyte literal
L44	byte	'контрольный пример', 82 dup(0)	 ; string literal
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
loop1_i	sdword	0	 ; integer var
global_binar	byte	0	 ; ubyte var
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
loop2_i	sdword	0	 ; integer var
;---------------------------------------------


.code


;----------------Functions-------------------

hello_world proc  hello_world_k : sdword


push offset L16

push offset hello_world_a	;assigment to STR begin
call concat1
cmp hello_world_a, 0
jne notOverflow
push 0
 callExitProccess
notOverflow: 	;assigment to STR begin


movzx eax, L19	;loop begin
mov loop1_i, eax
loop1: 

mov eax, hello_world_k
push eax

movzx eax, L19
push eax

pop ebx; 	+
pop eax
add eax, ebx
push eax

pop eax	;assigment to INT begin
cmp eax, 2147483647
jl isOverflow
cmp eax, -2147483648
jg isOverflow
mov hello_world_k, eax
jmp isNotOverflow
isOverflow:

push 0 	;INT OVERFLOW
push 0
push 0
push sizeof INTOVERFLOW_text
push offset INTOVERFLOW_text
push consoleHandle
call WriteConsoleA
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow: 	;assigment to INT done

inc loop1_i
cmp loop1_i, L20
jg loop1	loop end


mov eax, offset hello_world_a 	;begin of return STR
ret 4	;end of return

hello_world endp

fi proc  fi_x : sdword, fi_y : sdword


movzx eax, L20
push eax

call hello_world
push eax

push offset global_test_str

push offset fi_k	;assigment to STR begin
call concat2
cmp fi_k, 0
jne notOverflow
push 0
 callExitProccess
notOverflow: 	;assigment to STR begin


mov eax, fi_x
push eax

mov eax, fi_x
push eax

movzx eax, fi_y
push eax

pop ebx; 	+
pop eax
add eax, ebx
push eax

pop ebx; 	*
pop eax
imul eax, ebx
push eax

mov eax, fi_z
push eax

pop eax; 	!
not eax
push eax

movzx eax, fi_y
push eax

pop ebx; 	&
pop eax
or eax, ebx
push eax

mov eax, fi_x
push eax

pop eax; 	!
not eax
push eax

pop ebx; 	|
pop eax
or eax, ebx
push eax

pop eax	;assigment to INT begin
cmp eax, 2147483647
jl isOverflow
cmp eax, -2147483648
jg isOverflow
mov fi_z, eax
jmp isNotOverflow
isOverflow:

push 0 	;INT OVERFLOW
push 0
push 0
push sizeof INTOVERFLOW_text
push offset INTOVERFLOW_text
push consoleHandle
call WriteConsoleA
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow: 	;assigment to INT done


mov eax, fi_z 	begin of return INT
ret 8	;end of return

fi endp


movzx eax, L19
push eax

pop eax	;assigment to INT begin
cmp eax, 2147483647
jl isOverflow
cmp eax, -2147483648
jg isOverflow
mov main_x, eax
jmp isNotOverflow
isOverflow:

push 0 	;INT OVERFLOW
push 0
push 0
push sizeof INTOVERFLOW_text
push offset INTOVERFLOW_text
push consoleHandle
call WriteConsoleA
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow: 	;assigment to INT done


movzx eax, L38
push eax

pop eax	;assigment to INT begin
cmp eax, 2147483647
jl isOverflow
cmp eax, -2147483648
jg isOverflow
mov main_y, eax
jmp isNotOverflow
isOverflow:

push 0 	;INT OVERFLOW
push 0
push 0
push sizeof INTOVERFLOW_text
push offset INTOVERFLOW_text
push consoleHandle
call WriteConsoleA
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow: 	;assigment to INT done


movzx eax, L19	;loop begin
mov loop2_i, eax
loop2: 

mov eax, main_x
push eax

mov eax, loop2_i
push eax

pop ebx; 	+
pop eax
add eax, ebx
push eax

pop eax	;assigment to INT begin
cmp eax, 2147483647
jl isOverflow
cmp eax, -2147483648
jg isOverflow
mov main_x, eax
jmp isNotOverflow
isOverflow:

push 0 	;INT OVERFLOW
push 0
push 0
push sizeof INTOVERFLOW_text
push offset INTOVERFLOW_text
push consoleHandle
call WriteConsoleA
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow: 	;assigment to INT done

inc loop2_i
cmp loop2_i, L20
jg loop2	loop end


push offset L42

push offset main_sa	;assigment to STR begin
call concat1
cmp main_sa, 0
jne notOverflow
push 0
 callExitProccess
notOverflow: 	;assigment to STR begin


push offset L42

push offset main_sb	;assigment to STR begin
call concat1
cmp main_sb, 0
jne notOverflow
push 0
 callExitProccess
notOverflow: 	;assigment to STR begin


mov eax, main_x
push eax

mov eax, main_y
push eax

call fi
push eax

movzx eax, L43
push eax

pop ebx; 	*
pop eax
imul eax, ebx
push eax

pop eax	;assigment to INT begin
cmp eax, 2147483647
jl isOverflow
cmp eax, -2147483648
jg isOverflow
mov main_z, eax
jmp isNotOverflow
isOverflow:

push 0 	;INT OVERFLOW
push 0
push 0
push sizeof INTOVERFLOW_text
push offset INTOVERFLOW_text
push consoleHandle
call WriteConsoleA
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow: 	;assigment to INT done


call hello_world
push eax

push offset main_sc	;assigment to STR begin
call concat1
cmp main_sc, 0
jne notOverflow
push 0
 callExitProccess
notOverflow: 	;assigment to STR begin


push 0 	;cprint
push 0
push sizeof L44
push offset L44
call WriteConsoleA


push 0 	;cprint
push 0
push sizeof main_sa
push offset main_sa
call WriteConsoleA


push 0 	;cprint
push 0
push sizeof main_sc
push offset main_sc
call WriteConsoleA

;----------------------------------------------
