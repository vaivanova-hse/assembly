format PE console
entry start

include 'win32a.inc'
; ÃÕ≈ Õ¿ƒŒ ¬€¬≈—“» Ã¿——»¬ ¡≈« œ≈–¬Œ√Œ œŒÀŒ∆»“≈À‹ÕŒ√Œ ◊»—À¿
; VAR - 11.
;----------------------------Some useful data----------------------------------------------
section '.data' data readable writable

        strVecSize   db 'enter size of the vector: ', 0
        strIncorSize db 'Incorrect size of vector A = %d', 10, 0
        strVecElemI  db '[%d]? ', 0
        strScanInt   db '%d', 0
        strVecElemOut  db '[%d] = %d', 10, 0

        vec_size     dd 0
        i            dd ?
        tmp          dd ?
        tmpStack     dd ?
        vec_a         rd 100
        vec_b         rd 100

;------------------------------------Sections of code--------------------------------------
section '.code' code readable executable
start:
; 1) vector A input
        call VectorInput
; 2) get vector B (without first positive)
        call CreateVectorB

; 3) vector B output
        call VectorOut
finish:
        call [getch]

        push 0
        call [ExitProcess]

;---------------------------------Getting vector A-----------------------------------------
VectorInput:
        push strVecSize
        call [printf]
        add esp, 4

        push vec_size
        push strScanInt
        call [scanf]
        add esp, 8

        mov eax, [vec_size]
        cmp eax, 0
        jg  getVector
; if bad enter
        push vec_size
        push strIncorSize
        call [printf]
        push 0
        call [ExitProcess]

; else everything OK
getVector:
        xor ecx, ecx
        mov ebx, vec_a
getVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        jge endInputVector


        mov [i], ecx
        push ecx
        push strVecElemI
        call [printf]
        add esp, 8

        push ebx
        push strScanInt
        call [scanf]
        add esp, 8

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp getVecLoop
endInputVector:
        ret
;------------------------------Creating new vector B--------------------------------------------
CreateVectorB:
        xor ecx, ecx              ; ecx = 0
        mov ebx, vec_a            ; ebx = &vec_a
        mov esi, vec_b            ; esi = &vec_b
        jmp CompareLoop
CompareLoop:
        mov eax, [ebx]
        mov [tmp], ebx
        cmp ecx, [vec_size]
        je endOfCompareLoop

        mov [i], ecx
        cmp eax, 0
        jg  ifPositive

        mov [esi], eax
        add esi, 4
       add  ebx, 4
        inc ecx
        jmp CompareLoop
ifPositive:
       cmp ecx, [vec_size]
       je endOfCompareLoopG

       add  ebx, 4
       mov eax, [ebx]
       mov [esi], eax
       add esi, 4

       inc ecx
       jmp  ifPositive
endOfCompareLoopG:
       dec [vec_size]
       ret
endOfCompareLoop:
        ret


;---------------------------------Printing new Vector B-----------------------------------------

VectorOut:
        mov [tmpStack], esp
        xor ecx, ecx
        mov ebx, vec_b
        jmp putVecLoop

putVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        je endOutputVector
        mov [i], ecx

        ; output element
        push dword [ebx]
        push ecx
        push strVecElemOut
        call [printf]

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp putVecLoop
endOutputVector:
        mov esp, [tmpStack]
        ret
;-------------------------------Including HeapApi--------------------------
                                                 
section '.idata' import data readable
    library kernel, 'kernel32.dll',\
            msvcrt, 'msvcrt.dll',\
            user32,'USER32.DLL'

include 'api\user32.inc'
include 'api\kernel32.inc'
    import kernel,\
           ExitProcess, 'ExitProcess',\
           HeapCreate,'HeapCreate',\
           HeapAlloc,'HeapAlloc'
  include 'api\kernel32.inc'
    import msvcrt,\
           printf, 'printf',\
           scanf, 'scanf',\
           getch, '_getch'