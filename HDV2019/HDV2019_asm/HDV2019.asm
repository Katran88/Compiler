.586							; ������� ������ (��������� Pentium)
.model flat, stdcall			; ������ ������, ���������� � �������
includelib kernel32.lib			; ������������: ����������� � kernel32

ExitProcess PROTO: DWORD		; �������� ������� ��� ���������� �������� Windows

;-----------console-----------
GetStdHandle PROTO: DWORD	; �������� handle ������ �� �������
WriteConsoleA PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD		;����� �� �������(����������� � - ��)
SetConsoleTitleA PROTO: DWORD		; �������� �-�� ��������������� ��������� ����������� ����
;-----------------------------

;-----------DateTime-----------
includelib ..\Debug\DateTimelib.lib
getDate PROTO	; ���������� �������, ��������� ���� � ���� ������
getTime PROTO	; ���������� �������, ��������� ����� � ���� ������
;------------------------------

.stack 4096

;-----------Literals and constants-----------
.const

consoleTitle byte 'HDV2019', 0

L1	byte	7	 ; ubyte literal
L3	byte	122	 ; ubyte literal
L5	sdword	2593	 ; integer literal
L7	byte	200	 ; ubyte literal
L8	sdword	-1	 ; integer literal
L10	byte	1	 ; logic literal
L13	byte	'������ ���!', 89 dup(0)	 ; string literal
L27	byte	1	 ; ubyte literal
L28	byte	5	 ; ubyte literal
L31	byte	10	 ; ubyte literal
L36	byte	'1234567890', 90 dup(0)	 ; string literal
L37	byte	'����������� ������', 82 dup(0)	 ; string literal
;--------------------------------------------


.data


;----------------Variables-------------------

consoleHandle dd 0h	; ��������� �������

V0	byte	0	 ; ubyte var
V2	byte	0	 ; ubyte var
V4	byte	0	 ; ubyte var
V6	byte	0	 ; ubyte var
V9	byte	0	 ; logic var
V12	byte	 dup(0)	 ; string var
V18	byte	 dup(0)	 ; string var
V19	sdword	0	 ; integer var
V21	sdword	0	 ; integer var
V22	sdword	0	 ; integer var
V23	sdword	0	 ; integer var
V24	byte	 dup(0)	 ; string var
V25	byte	 dup(0)	 ; string var
V26	byte	 dup(0)	 ; string var
V30	sdword	0	 ; integer var
V33	sdword	0	 ; integer var
V35	sdword	0	 ; integer var
;---------------------------------------------


.code


;----------------Functions-------------------

hello_world proc 	



hello_world endp

fi proc 	x : sdword, y : sdword



fi endp

;----------------------------------------------
