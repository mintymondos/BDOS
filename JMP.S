Bits 16
global START
[EXTERN C_START]

START:
    cli
    XOR ax, ax
    MOV ds, ax
    MOV es, ax
    MOV ss, ax
    MOV sp, 0xFFFE
    sti

    MOV ax, 0xB800
    MOV gs, ax
    MOV byte [gs:0x0000], 'A'
    MOV byte [gs:0x0001], 0x07

    call C_START

    MOV byte [gs:0x0000], 'E'
    MOV byte [gs:0x0001], 0x07
    jmp $