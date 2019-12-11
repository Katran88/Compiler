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
L23	byte	15	 ; ubyte literal
L41	byte	5	 ; ubyte literal
L45	byte	'1234567890', 90 dup(0)	 ; string literal
L46	byte	11	 ; ubyte literal
L47	byte	'контрольный пример', 82 dup(0)	 ; string literal
;--------------------------------------------


;----------------Variables-------------------
.data

consoleHandle dd 0h	; состояние консоли
ifFlag byte 0 	; флаг для входа в блок ifglobal_binary	byte	0	 ; ubyte var
global_eight	byte	0	 ; ubyte var
global_hex	byte	0	 ; ubyte var
global_test	byte	0	 ; ubyte var
global_test_logic	byte	0	 ; logic var
global_test_str	byte	100 dup(0)	 ; string var
hello_world_a	byte	100 dup(0)	 ; string var
loop1_i	sdword	0	 ; integer var
loop2_iterator	sdword	0	 ; integer var
fi_k	byte	100 dup(0)	 ; string var
fi_z	sdword	0	 ; integer var
main_x	sdword	0	 ; integer var
main_y	sdword	0	 ; integer var
main_z	sdword	0	 ; integer var
main_sa	byte	100 dup(0)	 ; string var
main_sb	byte	100 dup(0)	 ; string var
main_sc	byte	100 dup(0)	 ; string var
loop3_i	sdword	0	 ; integer var
;---------------------------------------------


.code


;----------------Functions-------------------

hello_world proc  hello_world_k : sdword


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


push offset L16

push offset hello_world_a	;assigment to STR begin
call concat1
cmp hello_world_a, 0
jne notOverflow0
push 0
call ExitProcess
notOverflow0: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<loop begin

movzx eax, L19
mov loop1_i, eax
loop1: 

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<loop begin

movzx eax, L23
mov loop2_iterator, eax
loop2: 

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


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
jg isOverflow1
cmp eax, -2147483648
jl isOverflow1
mov hello_world_k, eax
jmp isNotOverflow1
isOverflow1:

push 0 	;INT OVERFLOW
push 0
push 0
push sizeof INTOVERFLOW_text
push offset INTOVERFLOW_text
push consoleHandle
call WriteConsoleA
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow1: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

dec loop2_iterator
mov eax, loop2_iterator
movzx ebx, L20
cmp eax, ebx
jg loop2

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<loop end

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


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
jg isOverflow2
cmp eax, -2147483648
jl isOverflow2
mov hello_world_k, eax
jmp isNotOverflow2
isOverflow2:

push 0 	;INT OVERFLOW
push 0
push 0
push sizeof INTOVERFLOW_text
push offset INTOVERFLOW_text
push consoleHandle
call WriteConsoleA
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow2: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

inc loop1_i
mov eax, loop1_i
movzx ebx, L20
cmp eax, ebx
jl loop1

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<loop end

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<return begin


mov eax, offset hello_world_a 	;return STR
ret 4

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<return end


hello_world endp

fi proc  fi_x : sdword, fi_y : byte


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


movzx eax, L20
push eax

call hello_world
push eax

push offset global_test_str

push offset fi_k	;assigment to STR begin
call concat2
cmp fi_k, 0
jne notOverflow3
push 0
call ExitProcess
notOverflow3: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


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
jg isOverflow4
cmp eax, -2147483648
jl isOverflow4
mov fi_z, eax
jmp isNotOverflow4
isOverflow4:

push 0 	;INT OVERFLOW
push 0
push 0
push sizeof INTOVERFLOW_text
push offset INTOVERFLOW_text
push consoleHandle
call WriteConsoleA
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow4: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<return begin


mov eax, fi_z 	;return INT
ret 8

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<return end


fi endp

;----------------------------------------------
main proc 	;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<MAIN

;----------Initialization of global variables----------

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


mov eax, main_x
push eax

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


mov eax, main_x
push eax

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


mov eax, main_x
push eax

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


mov eax, main_x
push eax

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


mov eax, main_x
push eax

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


mov eax, main_x
push eax

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

;------------------------------------------------------

;-----------Console setup-----------
push offset consoleTitle		; помещаем в стек параметр функции SetConsoleTitle строку
call SetConsoleTitleA		; вызываем функцию устанвки заголовка окна
push -11		; помещаем в стек код ращзрешения на вывод в консоли
call GetStdHandle			; вызываем ф-ию проверки разрешения на вывод
mov consoleHandle, eax		; копируем полученное разрешение из регистра eax

;-----------------------------------

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


movzx eax, L19
push eax

pop eax	;assigment to INT begin
cmp eax, 2147483647
jg isOverflow11
cmp eax, -2147483648
jl isOverflow11
mov main_x, eax
jmp isNotOverflow11
isOverflow11:

push 0 	;INT OVERFLOW
push 0
push 0
push sizeof INTOVERFLOW_text
push offset INTOVERFLOW_text
push consoleHandle
call WriteConsoleA
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow11: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


movzx eax, L41
push eax

pop eax	;assigment to INT begin
cmp eax, 2147483647
jg isOverflow12
cmp eax, -2147483648
jl isOverflow12
mov main_y, eax
jmp isNotOverflow12
isOverflow12:

push 0 	;INT OVERFLOW
push 0
push 0
push sizeof INTOVERFLOW_text
push offset INTOVERFLOW_text
push consoleHandle
call WriteConsoleA
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow12: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<loop begin

movzx eax, L19
mov loop3_i, eax
loop3: 

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


mov eax, main_x
push eax

mov eax, loop3_i
push eax

pop ebx; 	+
pop eax
add eax, ebx
push eax

pop eax	;assigment to INT begin
cmp eax, 2147483647
jg isOverflow13
cmp eax, -2147483648
jl isOverflow13
mov main_x, eax
jmp isNotOverflow13
isOverflow13:

push 0 	;INT OVERFLOW
push 0
push 0
push sizeof INTOVERFLOW_text
push offset INTOVERFLOW_text
push consoleHandle
call WriteConsoleA
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow13: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

inc loop3_i
mov eax, loop3_i
movzx ebx, L20
cmp eax, ebx
jl loop3

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<loop end

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


push offset L45

push offset main_sa	;assigment to STR begin
call concat1
cmp main_sa, 0
jne notOverflow14
push 0
call ExitProcess
notOverflow14: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


push offset L45

push offset main_sb	;assigment to STR begin
call concat1
cmp main_sb, 0
jne notOverflow15
push 0
call ExitProcess
notOverflow15: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


mov eax, main_x
push eax

mov eax, main_y
push eax

call fi
push eax

movzx eax, L46
push eax

pop ebx; 	*
pop eax
imul eax, ebx
push eax

pop eax	;assigment to INT begin
cmp eax, 2147483647
jg isOverflow16
cmp eax, -2147483648
jl isOverflow16
mov main_z, eax
jmp isNotOverflow16
isOverflow16:

push 0 	;INT OVERFLOW
push 0
push 0
push sizeof INTOVERFLOW_text
push offset INTOVERFLOW_text
push consoleHandle
call WriteConsoleA
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow16: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


call hello_world
push eax

push offset main_sc	;assigment to STR begin
call concat1
cmp main_sc, 0
jne notOverflow17
push 0
call ExitProcess
notOverflow17: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprint begin
push 0 	;cprint
push 0
push sizeof L47
push offset L47
call WriteConsoleA
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprint end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprint begin
push 0 	;cprint
push 0
push sizeof main_sa
push offset main_sa
call WriteConsoleA
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprint end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprint begin
push 0 	;cprint
push 0
push sizeof main_sc
push offset main_sc
call WriteConsoleA
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprint end


main endp
end main