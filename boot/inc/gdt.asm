%ifndef __GDT_INC_
%define __GDT_INC_

[bits 16]

;------------------------------------------
;   DATA SECTION
;------------------------------------------
%define GDT_NULL_DESC 0x0
%define GDT_CODE_DESC 0x8
%define GDT_DATA_DESC 0x10

GDT_START: 
    ; First 8 bytes aren't used by the CPU afaik
    .NullDesc:
        dw 0
        dw 0
        dw 0
        dw 0

    ; Setting all code section (0B - 4GB) to be readable/executable
    .CodeDesc:
        dw 0xFFFF   ; Bits 0-15
        dw 0x0000   ; Bits 16-31
        db 0x00     ; Bits 32-39
        db 10011010b; Bits 40-47
        db 11001111b; Bits 48-55
        db 0x00     ; Bits 56-63

    ; Same as CodeDesc only the descriptor type bit is set to `data` instead
    .DataDesc:
        dw 0xFFFF   ; Bits 0-15
        dw 0x0000   ; Bits 16-31
        db 0x00     ; Bits 32-39
        db 10010010b; Bits 40-47
        db 11001111b; Bits 48-55
        db 0x00     ; Bits 56-63
GDT_END:

; LGDT instruction takes 6 bytes (in 32-bit mode)
LGDT_VAL: 
    dw 0    ; size in bytes
    dd 0    ; memory address (3 bytes) and data access rights (1 byte)
;------------------------------------------
;   END OF DATA SECTION
;------------------------------------------


GDT_INSTALL:
    push bp
    mov bp, sp

    ; calculate GDT size - 1 (2 bytes)
    mov ax, WORD GDT_END
    sub ax, GDT_START
    dec ax
    mov WORD [LGDT_VAL], ax

    ; calcluate GDT location (3 bytes)
    mov ax, GDT_START
    mov WORD [LGDT_VAL+2], ax
    mov BYTE [LGDT_VAL+4], 0x00 ; our loc not exceeding 2 bytes
    ; remaining 
    mov BYTE [LGDT_VAL+5], 0x93 ; data cacess rights (can be ignored actually)

    cli ; disable interrupts
    lgdt [LGDT_VAL] ; load GDT
    sti ; enable interrupts

    leave
    ret

%endif