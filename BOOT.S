bits 16
org 0x7c00
global _START
_START:
    jmp short boot_start
    nop
    times 59 db 0
boot_start:
    XOR ax, ax
    MOV ds, ax
    MOV es, ax
    MOV ss, ax
    MOV sp, 0x7C00
    MOV [BOOT_DRIVE], dl
    MOV ah, 0x08
    MOV dl, [BOOT_DRIVE]
    INT 0x13
    jc .geom_fail
    AND dh, 0x0F
    MOV [MAX_HEAD], dh
    AND cl, 0x3F
    MOV [MAX_SECTOR], cl
    jmp .geom_done
.geom_fail:
    MOV byte [MAX_HEAD], 1
    MOV byte [MAX_SECTOR], 18
.geom_done:
    XOR ax, ax
    MOV es, ax

    MOV byte [CUR_CYL], 0
    MOV byte [CUR_HEAD], 0
    MOV byte [CUR_SECTOR], 2
    MOV byte [SECTORS_LEFT], STAGE2_SECTORS
    MOV bx, 0x8000

.sector_loop:
    CMP byte [SECTORS_LEFT], 0
    je .load_done

    MOV byte [RETRY_COUNT], 3

.retry_sector:
    MOV dl, [BOOT_DRIVE]
    XOR ah, ah
    INT 0x13
    jc .sector_fail

    MOV ah, 0x02
    MOV al, 1
    MOV ch, [CUR_CYL]
    MOV cl, [CUR_SECTOR]
    MOV dh, [CUR_HEAD]
    MOV dl, [BOOT_DRIVE]
    INT 0x13
    jnc .sector_ok

.sector_fail:
    MOV [DISK_ERROR], ah
    DEC byte [RETRY_COUNT]
    jnz .retry_sector
    jmp error

.sector_ok:
    ADD bx, 512
    DEC byte [SECTORS_LEFT]

    MOV al, [CUR_SECTOR]
    INC al
    CMP al, [MAX_SECTOR]
    jle .store_sector

    MOV byte [CUR_SECTOR], 1
    MOV al, [CUR_HEAD]
    INC al
    CMP al, [MAX_HEAD]
    jg .next_cyl
    MOV [CUR_HEAD], al
    jmp .sector_loop

.next_cyl:
    MOV byte [CUR_HEAD], 0
    INC byte [CUR_CYL]
    jmp .sector_loop

.store_sector:
    MOV [CUR_SECTOR], al
    jmp .sector_loop

.load_done:
    jmp 0000:0x8000

error:
    MOV ax, 0xB800
    MOV es, ax
    MOV si, str_panic
    MOV di, 0x00A0
    MOV bl, 0x04
    CALL print_string
    MOV al, [DISK_ERROR]
    CALL print_hex_byte
    jmp $

print_string:
.loop:
    lodsb
    or al, al
    jz .done
    MOV [es:di], al
    MOV [es:di+1], bl
    add di, 2
    jmp .loop
.done:
    ret

print_hex_byte:
    push ax
    MOV ah, al
    SHR al, 4
    CALL hex_digit
    MOV [es:di], al
    MOV byte [es:di+1], 0x0A
    pop ax
    AND al, 0x0F
    CALL hex_digit
    MOV [es:di+2], al
    MOV byte [es:di+3], 0x0A
    RET

hex_digit:
    CMP al, 10
    jl .digit
    ADD al, 'A' - 10 - '0'
.digit:
    ADD al, '0'
    RET

str_panic db '!!! KERNEL PANIC !!! ERR:', 0

BOOT_DRIVE    db 0
RETRY_COUNT   db 0
DISK_ERROR    db 0
MAX_HEAD      db 0
MAX_SECTOR    db 0
CUR_CYL       db 0
CUR_HEAD      db 0
CUR_SECTOR    db 0
SECTORS_LEFT  db 0

TIMES 510 - ($ - $$) db 0
dw 0xaa55
