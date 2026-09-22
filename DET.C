#include "DET.h"
#include "MAIN.h"
#include <stdint.h>

uint16_t identify_data[256];

uint8_t atadetectionfuckyou(void) {
    uint8_t status;

    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x1F6\n\t"
        "mov al, 0xA0\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F7\n\t"
        "in al, dx\n\t"
        ".att_syntax prefix\n\t"
        : "=a" (status)
        :
        : "dx"
    );

    return status;
}

__attribute__((noreturn)) void nodrive() {
    printst("ERROR:", 4);
    printst("No ATA PIO drive found on your computer, or the drive is not accessable, make sure your computer is using an ATA PIO hard drive, BDOS will be unusable until a hard drive is detected.\n", 15);
    while (1) {
        __asm__ __volatile__ (
            "hlt"
        );
    }
}

uint8_t identifyata(uint16_t *buffer) {
    uint8_t status;
    uint8_t lbamid;
    uint8_t lbahigh;

    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x1F6\n\t"
        "mov al, 0xA0\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F2\n\t"
        "mov al, 0x00\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F3\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F4\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F5\n\t"
        "out dx, al\n\t"
        ".att_syntax prefix\n\t"
        :
        :
        : "ax", "dx"
    );

    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x1F7\n\t"
        "mov al, 0xEC\n\t"
        "out dx, al\n\t"
        "in al, dx\n\t"
        ".att_syntax prefix\n\t"
        : "=a" (status)
        :
        : "dx"
    );

    if (status == 0x00) {
        return 1;
    }

    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x1F4\n\t"
        "in al, dx\n\t"
        "mov bl, al\n\t"
        "mov dx, 0x1F5\n\t"
        "in al, dx\n\t"
        ".att_syntax prefix\n\t"
        : "=a" (lbahigh), "=b" (lbamid)
        :
        : "dx"
    );

    if (lbamid == 0x14 && lbahigh == 0xEB) {
        return 2;
    }

    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x1F7\n\t"
        "poll_loop_%=:\n\t"
        "in al, dx\n\t"
        "test al, 0x80\n\t"
        "jnz poll_loop_%=\n\t"
        "test al, 0x08\n\t"
        "jz poll_loop_%=\n\t"
        ".att_syntax prefix\n\t"
        : "=a" (status)
        :
        : "dx"
    );

    if (status & 0x01) {
        return 2;
    }

    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x1F0\n\t"
        "mov cx, 256\n\t"
        "read_loop_%=:\n\t"
        "in ax, dx\n\t"
        "stosw\n\t"
        "loop read_loop_%=\n\t"
        ".att_syntax prefix\n\t"
        :
        : "D" (buffer)
        : "ax", "cx", "dx", "memory"
    );

    return 0;
}

void detection() {
    uint8_t atadetection = atadetectionfuckyou();
    if (atadetection == 0xFF) {
        nodrive();
    }

    uint8_t result = identifyata(identify_data);

    if (result == 1) {
        nodrive();
    }
    else if (result == 2) {
        printst("ERROR:", 4);
        printst("A hard drive responded but could not be identified as ATA PIO, make sure you are using an ATA hard drive and not an AHCI hard drive.\n", 15);
        while (1) {
            __asm__ __volatile__ (
                "hlt"
            );
        }
    }
    else {
        printst("Successfully located ATA drive on your computer\n", 14);
    }

}
