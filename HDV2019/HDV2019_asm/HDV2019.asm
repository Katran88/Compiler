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
func_strlength PROTO : DWORD
func_inttostr PROTO : DWORD , : SDWORD
func_random PROTO : SDWORD , : SDWORD
;----------------------------------------

;-----------DateTime-----------
includelib ..\DateTimelib.lib
func_getDate PROTO : DWORD	; Возвращает текущую, локальную дату в виде строки
func_getTime PROTO : DWORD	; Возвращает текущее, локальную время в виде строки
;------------------------------

.stack 4096

;-----------Literals and constants-----------
.const

consoleTitle byte 'HDV2019', 0
INTOVERFLOW_text byte 'INT OVERFLOW EXCEPTION', 10, 0
UBYTEOVERFLOW_text byte 'UBYTE OVERFLOW EXCEPTION', 10, 0
DIVISION_BY_ZERO_text byte 'DIVISION BY ZERO', 10, 0

STROVERFLOW_TEXT byte 'STR OVERFLOW', 0

L6	byte	7	 ; ubyte literal
L8	byte	122	 ; ubyte literal
L10	sdword	2593	 ; integer literal
L12	byte	4	 ; ubyte literal
L13	byte	15	 ; ubyte literal
L14	byte	"abc", 97 dup(0)	 ; string literal
L15	byte	6	 ; ubyte literal
L17	byte	0	 ; logic literal
L22	byte	0	 ; ubyte literal
L24	byte	1	 ; ubyte literal
L30	byte	"Цикл закончил свою работу", 75 dup(0)	 ; string literal
L34	sdword	256	 ; integer literal
L40	byte	" ", 99 dup(0)	 ; string literal
L43	byte	"Today: ", 93 dup(0)	 ; string literal
L44	byte	"-------------------------------------------------", 51 dup(0)	 ; string literal
L45	byte	"Максимальное значение, которое может хранится в: ", 51 dup(0)	 ; string literal
L50	byte	30	 ; ubyte literal
L51	byte	2	 ; ubyte literal
L52	byte	" битах : ", 91 dup(0)	 ; string literal
L55	byte	3	 ; ubyte literal
L56	byte	5	 ; ubyte literal
L57	byte	"HDV", 97 dup(0)	 ; string literal
L58	byte	"it's works", 90 dup(0)	 ; string literal
L62	byte	20	 ; ubyte literal
L64	byte	"Рандомное значение от ", 78 dup(0)	 ; string literal
L65	byte	" до ", 96 dup(0)	 ; string literal
L66	byte	": ", 98 dup(0)	 ; string literal
;--------------------------------------------


;----------------Variables-------------------
.data

ifFlag byte 0 	; флаг для входа в блок if
cprint_var byte 100 dup(0)	; variable for cprint
cprintl_var byte 100 dup(0)	; variable for cprintl
global_binary	byte	0	 ; ubyte var
global_eight	byte	0	 ; ubyte var
global_hex	sdword	0	 ; integer var
global_value	sdword	0	 ; integer var
global_flagFor_ubyteMaxValue	byte	0	 ; logic var
pow_result	sdword	0	 ; integer var
loop1_iterator	sdword	0	 ; integer var
getFullTime_rc	byte	100 dup(0)	 ; string var
main_maxValue	sdword	0	 ; integer var
main_counter	sdword	0	 ; integer var
loop2_i	sdword	0	 ; integer var
main_a	sdword	0	 ; integer var
main_b	sdword	0	 ; integer var
main_randVar	sdword	0	 ; integer var
getDate_131	byte	1024 dup(0)	 ; return var
getTime_134	byte	1024 dup(0)	 ; return var
inttostr_195	byte	13 dup(0)	 ; return var
inttostr_200	byte	13 dup(0)	 ; return var
inttostr_251	byte	13 dup(0)	 ; return var
inttostr_256	byte	13 dup(0)	 ; return var
inttostr_266	byte	13 dup(0)	 ; return var
;---------------------------------------------


.code


;----------------Functions-------------------

func_pow proc  pow_x : sdword, pow_power : byte



;<<<<<<<<<<<<<<<<<<<<<<<<<<<if block begin
mov ifFlag, 0
movzx eax, pow_power
movzx ebx, L22
cmp eax, ebx 
jne s65	; ==
or ifFlag, 1

s65:	; condition is not done

 cmp ifFlag, 0	; check for result flag
je if_block1


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<return begin
movzx eax, L24 	;return UBYTE or LOGIC
ret 8
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<return end

if_block1:	;<<<<<<<<<<<<<<<<<<<<<<<<<<< if block end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


movzx eax, L24
push eax

pop eax	;assigment to INT begin
mov pow_result, eax
jno isNotOverflow0

	;INT OVERFLOW
push offset INTOVERFLOW_text
call cprintl
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow0: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<loop begin

movzx eax, L24
mov loop1_iterator, eax
loop1: 
mov eax, loop1_iterator

movzx ebx, pow_power
cmp eax, ebx
jg loop1_end

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


mov eax, pow_result
push eax

mov eax, pow_x
push eax

pop ebx; 	*
pop eax
imul eax, ebx
push eax

pop eax	;assigment to INT begin
mov pow_result, eax
jno isNotOverflow1

	;INT OVERFLOW
push offset INTOVERFLOW_text
call cprintl
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow1: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end



;<<<<<<<<<<<<<<<<<<<<<<<<<<<if block begin
mov ifFlag, 0
mov eax, loop1_iterator
movzx ebx, pow_power
cmp eax, ebx 
jne s96	; ==
or ifFlag, 1

s96:	; condition is not done

 cmp ifFlag, 0	; check for result flag
je if_block2


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl begin
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


push offset L30

push offset cprintl_var	;assigment to STR begin
call concat1
cmp cprintl_var, 0
jne notOverflow2
push offset STROVERFLOW_TEXT
call cprintl
push 0
call ExitProcess
notOverflow2: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

push offset cprintl_var
call cprintl
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl end

if_block2:	;<<<<<<<<<<<<<<<<<<<<<<<<<<< if block end

inc loop1_iterator
mov eax, loop1_iterator
cmp eax, ebx
jle loop1
loop1_end:

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<loop end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<if block begin
mov ifFlag, 0
mov eax, pow_result
mov ebx, L34
cmp eax, ebx 
jl s108	; >=
or ifFlag, 1

s108:	; condition is not done

 cmp ifFlag, 0	; check for result flag
je if_block3


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


movzx eax, global_flagFor_ubyteMaxValue
push eax

pop eax; 	!
not eax
push eax

pop eax	;assigment to LOGIC begin
cmp eax, 0
je zeroEqual3
mov global_flagFor_ubyteMaxValue, 1
jmp ZeroNOTequal3
zeroEqual3:
mov global_flagFor_ubyteMaxValue, 0
zeroNOTequal3: 	;assigment to LOGIC done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

if_block3:	;<<<<<<<<<<<<<<<<<<<<<<<<<<< if block end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<return begin
mov eax, pow_result 	;return INT
ret 8
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<return end


func_pow endp

func_getFullTime proc  


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


push offset getDate_131
call func_getDate
push eax

push offset L40

push offset getTime_134
call func_getTime
push eax

push offset getFullTime_rc	;assigment to STR begin
call concat3
cmp getFullTime_rc, 0
jne notOverflow4
push offset STROVERFLOW_text
call cprintl
push 0
call ExitProcess
notOverflow4: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<return begin
mov eax, offset getFullTime_rc 	;return STR
ret 0
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<return end


func_getFullTime endp

;----------------------------------------------
main proc 	;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<MAIN

;----------Initialization of global variables----------

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


movzx eax, L6
push eax

pop eax	;assigment to UBYTE begin
mov global_binary, al
jno isNotOverflow5

	;UBYTE OVERFLOW
push offset UBYTEOVERFLOW_text
call cprintl
push 0
call ExitProcess 	;UBYTE OVERFLOW

isNotOverflow5: 	;assigment to UBYTE done

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


movzx eax, L8
push eax

pop eax	;assigment to UBYTE begin
mov global_eight, al
jno isNotOverflow6

	;UBYTE OVERFLOW
push offset UBYTEOVERFLOW_text
call cprintl
push 0
call ExitProcess 	;UBYTE OVERFLOW

isNotOverflow6: 	;assigment to UBYTE done

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


mov eax, L10
push eax

pop eax	;assigment to INT begin
mov global_hex, eax
jno isNotOverflow7

	;INT OVERFLOW
push offset INTOVERFLOW_text
call cprintl
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow7: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


movzx eax, L12
push eax

movzx eax, L13
push eax

pop ebx; 	*
pop eax
imul eax, ebx
push eax

push offset L14

call func_strlength
push eax

pop ebx; 	/
pop eax
cdq
cmp ebx, 0
jne NOzero8

	;DIVISION_BY_ZERO
push offset DIVISION_BY_ZERO_text
call cprintl
push 0
call ExitProcess
NOzero8:
idiv ebx
push eax

movzx eax, L13
push eax

pop ebx; 	-
pop eax
sub eax, ebx
push eax

movzx eax, L15
push eax

pop ebx; 	|
pop eax
or eax, ebx
push eax

movzx eax, global_binary
push eax

pop ebx; 	&
pop eax
and eax, ebx
push eax

pop eax	;assigment to INT begin
mov global_value, eax
jno isNotOverflow9

	;INT OVERFLOW
push offset INTOVERFLOW_text
call cprintl
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow9: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


movzx eax, L17
push eax

pop eax	;assigment to LOGIC begin
cmp eax, 0
je zeroEqual10
mov global_flagFor_ubyteMaxValue, 1
jmp ZeroNOTequal10
zeroEqual10:
mov global_flagFor_ubyteMaxValue, 0
zeroNOTequal10: 	;assigment to LOGIC done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

;------------------------------------------------------

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl begin
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


push offset L43

call func_getFullTime
push eax

push offset cprintl_var	;assigment to STR begin
call concat2
cmp cprintl_var, 0
jne notOverflow11
push offset STROVERFLOW_TEXT
call cprintl
push 0
call ExitProcess
notOverflow11: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

push offset cprintl_var
call cprintl
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl begin
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


push offset L44

push offset cprintl_var	;assigment to STR begin
call concat1
cmp cprintl_var, 0
jne notOverflow12
push offset STROVERFLOW_TEXT
call cprintl
push 0
call ExitProcess
notOverflow12: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

push offset cprintl_var
call cprintl
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl begin
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


push offset L45

push offset cprintl_var	;assigment to STR begin
call concat1
cmp cprintl_var, 0
jne notOverflow13
push offset STROVERFLOW_TEXT
call cprintl
push 0
call ExitProcess
notOverflow13: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

push offset cprintl_var
call cprintl
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<loop begin

movzx eax, L22
mov loop2_i, eax
loop2: 
mov eax, loop2_i

movzx ebx, L50
cmp eax, ebx
jg loop2_end

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


mov eax, main_maxValue
push eax

mov eax, loop2_i
push eax

movzx eax, L51
push eax

call func_pow
push eax

pop ebx; 	+
pop eax
add eax, ebx
push eax

pop eax	;assigment to INT begin
mov main_maxValue, eax
jno isNotOverflow14

	;INT OVERFLOW
push offset INTOVERFLOW_text
call cprintl
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow14: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


mov eax, loop2_i
push eax

movzx eax, L24
push eax

pop ebx; 	+
pop eax
add eax, ebx
push eax

pop eax	;assigment to INT begin
mov main_counter, eax
jno isNotOverflow15

	;INT OVERFLOW
push offset INTOVERFLOW_text
call cprintl
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow15: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl begin
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


push offset L40

mov eax, main_counter
push eax

push offset inttostr_195
call func_inttostr
push eax

push offset L52

mov eax, main_maxValue
push eax

push offset inttostr_200
call func_inttostr
push eax

push offset cprintl_var	;assigment to STR begin
call concat4
cmp cprintl_var, 0
jne notOverflow16
push offset STROVERFLOW_TEXT
call cprintl
push 0
call ExitProcess
notOverflow16: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

push offset cprintl_var
call cprintl
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl end

inc loop2_i
mov eax, loop2_i

movzx ebx, L50
cmp eax, ebx
jle loop2
loop2_end:

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<loop end

;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl begin
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


push offset L44

push offset cprintl_var	;assigment to STR begin
call concat1
cmp cprintl_var, 0
jne notOverflow17
push offset STROVERFLOW_TEXT
call cprintl
push 0
call ExitProcess
notOverflow17: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

push offset cprintl_var
call cprintl
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl end



;<<<<<<<<<<<<<<<<<<<<<<<<<<<if block begin
mov ifFlag, 0
movzx eax, global_flagFor_ubyteMaxValue
movzx ebx, ifFlag
or ebx, eax
mov ifFlag, bl
mov eax, lengthof L14
movzx ebx, L55
cmp eax, ebx 
jne s216	; ==
and ifFlag, 1

s216:	; condition is not done
movzx eax, L56
mov ebx, lengthof L57
cmp eax, ebx 
jg s220	; <=
or ifFlag, 1

s220:	; condition is not done

 cmp ifFlag, 0	; check for result flag
je if_block4


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl begin
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


push offset L58

push offset cprintl_var	;assigment to STR begin
call concat1
cmp cprintl_var, 0
jne notOverflow18
push offset STROVERFLOW_TEXT
call cprintl
push 0
call ExitProcess
notOverflow18: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

push offset cprintl_var
call cprintl
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl end

if_block4:	;<<<<<<<<<<<<<<<<<<<<<<<<<<< if block end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


movzx eax, L22
push eax

pop eax	;assigment to INT begin
mov main_a, eax
jno isNotOverflow19

	;INT OVERFLOW
push offset INTOVERFLOW_text
call cprintl
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow19: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


movzx eax, L62
push eax

pop eax	;assigment to INT begin
mov main_b, eax
jno isNotOverflow20

	;INT OVERFLOW
push offset INTOVERFLOW_text
call cprintl
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow20: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


mov eax, main_b
push eax

mov eax, main_a
push eax

call func_random
push eax

pop eax	;assigment to INT begin
mov main_randVar, eax
jno isNotOverflow21

	;INT OVERFLOW
push offset INTOVERFLOW_text
call cprintl
push 0
call ExitProcess 	;INT OVERFLOW

isNotOverflow21: 	;assigment to INT done


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprint begin
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


push offset L64

mov eax, main_a
push eax

push offset inttostr_251
call func_inttostr
push eax

push offset L65

mov eax, main_b
push eax

push offset inttostr_256
call func_inttostr
push eax

push offset cprint_var	;assigment to STR begin
call concat4
cmp cprint_var, 0
jne notOverflow22
push offset STROVERFLOW_TEXT
call cprintl
push 0
call ExitProcess
notOverflow22: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

push offset cprint_var
call cprint
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprint end


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl begin
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment begin


push offset L66

mov eax, main_randVar
push eax

push offset inttostr_266
call func_inttostr
push eax

push offset cprintl_var	;assigment to STR begin
call concat2
cmp cprintl_var, 0
jne notOverflow23
push offset STROVERFLOW_TEXT
call cprintl
push 0
call ExitProcess
notOverflow23: 	;assigment to STR begin


;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assigment end

push offset cprintl_var
call cprintl
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<cprintl end


push 0
call ExitProcess

main endp
end main