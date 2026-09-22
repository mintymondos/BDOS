#include "FDC.h"
#include "MAIN.h"
#include "ATA.h"

void floopywaitwrite(void) {
    uint8_t status;
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x3F4\n\t"
        "wait_write_%=:\n\t"
        "in al, dx\n\t"
        "and al, 0xC0\n\t"
        "cmp al, 0x80\n\t"
        "jne wait_write_%=\n\t"
        ".att_syntax prefix\n\t"
        : "=a" (status)
        :
        : "dx"
    );
}

void floopywaitread(void) {
    uint8_t stat;
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x3F4\n\t"
        "wait_read_%=:\n\t"
        "in al, dx\n\t"
        "and al, 0xC0\n\t"
        "cmp al, 0xC0\n\t"
        "jne wait_read_%=\n\t"
        ".att_syntax prefix\n\t"
        : "=a" (stat)
        :
        : "dx"
    );
}

void floopysendshyt(uint8_t cmd) {
    floopywaitwrite();
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x3F5\n\t"
        "out dx, al\n\t"
        ".att_syntax prefix\n\t"
        :
        : "a" (cmd)
        : "dx"
    );
}

uint8_t floopyreadshyt() {
    floopywaitread();
    uint8_t res;
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x3F5\n\t"
        "in al, dx\n\t"
        ".att_syntax prefix\n\t"
        : "=a" (res)
        :
        : "dx"
    );
    return res;
}

void floopyreset() {
    uint8_t DOR = 0x00;
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x3F2\n\t"
        "out dx, al\n\t"
        ".att_syntax prefix\n\t"
        :
        : "a" (DOR)
        : "dx"
    );
    for (int i = 0; i < 10000; i++) {
        // nothing ever happens
    }
    DOR = 0x14;
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x3F2\n\t"
        "out dx, al\n\t"
        ".att_syntax prefix\n\t"
        :
        : "a" (DOR)
        : "dx"
    );
    floopysendshyt(0x08);
    floopyreadshyt();
    floopyreadshyt();
}

void floopyrecal() {
    floopysendshyt(0x07);
    floopysendshyt(0x00);
    for (int i = 0; i < 10000; i++) {
        // nothing ever happens
    }
    floopysendshyt(0x08);
    floopyreadshyt();
    floopyreadshyt();
}

uint8_t floopyreadsect(uint8_t cylinder, uint8_t head, uint8_t sector, uint16_t *buff) {
    uint8_t DOR = 0x14;
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x3F2\n\t"
        "out dx, al\n\t"
        ".att_syntax prefix\n\t"
        :
        : "a" (DOR)
        : "dx"
    );
    for (int i = 0; i < 10000; i++) {
        // nothing ever happens
    }
    floopysendshyt(0xE6);
    floopysendshyt(head << 2);
    floopysendshyt(cylinder);
    floopysendshyt(head);
    floopysendshyt(sector);
    floopysendshyt(0x02);
    floopysendshyt(0x12);
    floopysendshyt(0x1B);
    floopysendshyt(0xFF);
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x3F5\n\t"
        "mov cx, 256\n\t"
        "read_loop_%=:\n\t"
        "in ax, dx\n\t"
        "stosw\n\t"
        "loop read_loop_%=\n\t"
        ".att_syntax prefix\n\t"
        :
        : "D" (buff)
        : "ax", "dx", "cx", "memory"
    );
    uint8_t st0 = floopyreadshyt();
    floopyreadshyt();
    floopyreadshyt();
    floopyreadshyt();
    floopyreadshyt();
    floopyreadshyt();
    if (st0 & 0x40) {
        return 1;
    }
    else {
        return 0;
    }
}

void lbatochs(uint16_t lba, uint8_t *outcyl, uint8_t *outhead, uint8_t *outsect) {
    *outcyl = lba / (18 * 2);
    *outhead = (lba / 18) % 2;
    *outsect =(lba % 18) + 1;
}


uint8_t floopywritesect(uint8_t cylinder, uint8_t head, uint8_t sector, uint16_t *buff) {
    uint8_t DOR = 0x14;
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x3F2\n\t"
        "out dx, al\n\t"
        ".att_syntax prefix\n\t"
        :
        : "a" (DOR)
        : "dx"
    );
    for (int i = 0; i < 10000; i++) {
        // nothing ever happens
    }
    floopysendshyt(0xC5);
    floopysendshyt(head << 2);
    floopysendshyt(cylinder);
    floopysendshyt(head);
    floopysendshyt(sector);
    floopysendshyt(0x02);
    floopysendshyt(0x12);
    floopysendshyt(0x1B);
    floopysendshyt(0xFF);
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x3F5\n\t"
        "mov cx, 256\n\t"
        "writeloop%=:\n\t"
        "lodsw\n\t"
        "out dx, ax\n\t"
        "loop writeloop%=\n\t"
        ".att_syntax prefix\n\t"
        :
        : "S" (buff)
        : "ax", "dx", "cx", "memory"
    );
    uint8_t st0 = floopyreadshyt();
    floopyreadshyt();
    floopyreadshyt();
    floopyreadshyt();
    floopyreadshyt();
    floopyreadshyt();
    if (st0 & 0x40) {
        return 1;
    }
    else {
        return 0;
    }
}

uint8_t floopyrlba(uint16_t lba, uint16_t *buffer) {
    uint8_t cyl;
    uint8_t head;
    uint8_t sect;
    lbatochs(lba, &cyl, &head, &sect);
    return floopyreadsect(cyl, head, sect, buffer);
}

uint8_t floopywlba(uint16_t lba, uint16_t *buffer) {
    uint8_t cyl;
    uint8_t head;
    uint8_t sect;
    lbatochs(lba, &cyl, &head, &sect);
    return floopywritesect(cyl, head, sect, buffer);
}
