format PE console
entry start

include 'include\win32a.inc'

;----------------------------------------SECTION IMPORT FUNCS--------------------------------------------
section '.idata' import data readable

library kernel,'kernel32.dll',\
        msvcrt,'msvcrt.dll'

import  kernel,\
        GetCommandLine,'GetCommandLineA',\
        lstrlen,'lstrlenA',\
        ExitProcess,'ExitProcess'

import msvcrt, printf, 'printf', scanf, 'scanf', getch, '_getch'

;----------------------------------------SECTION DATA DECLARATION-----------------------------------------

section '.data' data readable writeable

resfmt  db 'Sourse string:', 13, 10
        db '%s', 13, 10
        db 'Reversed words:', 13, 10
        db '%s', 0

revtxt  db 256 dup(0)

;----------------------------------------SECTION CODE-----------------------------------------------------

section '.code' code readable executable

start:
; 1) read input args
        call getCmdLines

; 2) start word by word reversing
        call serchBeginOfNewWord

; 3) end program and print result
printResult:
        push revtxt
        push ebx
        push resfmt
        call [printf]
        call [getch]
        push 0
        call [ExitProcess]

;--------------------------------------GET CMD LINE PROCERURES----------------------------------------------

getCmdLines:
        stdcall [GetCommandLine]        ; get cmd line args
        mov edi, eax
        ccall [lstrlen], eax            ; getting cmd line len
        mov ebx,eax
        mov al, ' '                     ; space control
        mov ecx, ebx
        repne scasb                     ; search for space or end of line
        jmp readWord

readWord:
        mov ebx,edi                     ; adress of biginning
        mov esi,edi
        mov edi,revtxt
        mov edx,ecx                     ; remembering numb simbols left
        jcxz skip                       ; if nothing
        rep movsb

skip:
        mov esi,revtxt

;------------------------------------WORD PROCESSING-------------------------------------------------------

serchBeginOfNewWord:
        lodsb                           ; read simbol
        cmp al, 0                       ; if end of line
        jz printResult
        cmp al, ' '                     ; if not end of word
        jnz saveBeginningAdress
        jmp serchBeginOfNewWord

saveBeginningAdress:
        dec esi
        mov edx, esi                    ; adress of word beginning

serchEndOfWord:
        lodsb                           ; read simbol
        cmp al, 0                       ; if end of line
        jz ifWordEnded
        cmp al, ' '                     ; if not end of word
        jz ifWordEnded
        jmp serchEndOfWord

ifWordEnded:
        dec esi                         ; previous simbol
        mov eax,esi                     ; word end adress
        sub eax,edx                     ; get len of word
        ccall reverseWord, edx, eax
        jmp serchBeginOfNewWord

;------------------------------------REVERSING ONE WORD--------------------------------------------------------

reverseWord:
        push ebp
        mov ebp, esp
        push esi
        push edi
        mov esi, [ebp+8]                 ; adress of reversed
        mov ecx, [ebp+12]                ; sourse len
        lea edx, [esi+ecx-1]             ; adress end sourse
        shr ecx, 1
        jecxz cleanRegisters             ; check len

reverseSimbols:
        lodsb                            ; read new simbol
        xchg al, [edx]                   ; reverse
        mov [esi-1], al
        dec edx
        loop reverseSimbols              ; make reverse

cleanRegisters:
        pop edi
        pop esi
        pop ebp
        ret
