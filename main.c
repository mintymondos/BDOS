#include "MISL.h"
#include "SHELL.h"
#include "MAIN.h"
#include "LOAD.h"
#include "ATA.h"
#include "DET.h"

static char* const VGA_MEMORY = (char*) 0xb8000;
static unsigned int cursor_pos = 0;
int input_floor = 0;
int strcmp(const char *str, const char *str2) {
    while (*str && (*str == *str2)) {
        str++;
        str2++;
    }
    return *(unsigned const char*)str - *(unsigned const char*)str2;
}

char ind[6556];
char *findindex(const char *str, int start, int end) {
    for (int i = 0; i < 6556; i++) {
        ind[i] = '\0';
    }
    int thingamasilly = 0;

    for (int i = start; i < end; i++) {
        if (str[i] != '\0') {
            ind[thingamasilly] = str[i];
            thingamasilly++;
        }
        else {
            break;
        }
    }
    return ind;
}


int stringlen(const char *str) {
    int stringlength = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        stringlength++;
    }
    return stringlength;
}

void scroll(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n"
        "push es\n"
        "push ds\n"
        "mov ax, 0xB800\n"
        "mov es, ax\n"
        "mov ds, ax\n"
        "xor si, si\n"
        "add si, 160\n"
        "xor di, di\n"
        "mov cx, 1920\n"
        "rep movsw\n"
        "pop ds\n"
        "pop es\n"
        ".att_syntax\n"
        :
        :
        : "si", "di", "cx", "memory"
    );
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n"
        "push es\n"
        "mov ax, 0xB800\n"
        "mov es, ax\n"
        "xor di, di\n"
        "add di, 3840\n"
        "mov ax, 0x0720\n"
        "mov cx, 80\n"
        "rep stosw\n"
        "pop es\n"
        ".att_syntax\n"
        :
        :
        : "ax", "di", "cx", "memory"
    );

    cursor_pos  = (25 - 1) * 80;
    input_floor = cursor_pos;
}

void printch(char c, int color) {
    if (c == '\n') {
        cursor_pos = (cursor_pos / 80 + 1) * 80;
        input_floor = cursor_pos + 2;
        if (cursor_pos >= 2000) scroll();
        return;
    }
    if (c == '\b') {
        if (cursor_pos > input_floor) {
            cursor_pos--;
        }
        char blank = ' ';
        __asm__ __volatile__ (
            ".intel_syntax noprefix\n"
            "push es\n"
            "mov ax, 0xB800\n"
            "mov es, ax\n"
            "mov bx, %w1\n"
            "mov es:[bx], %b0\n"
            "mov es:[bx+1], %b2\n"
            "pop es\n"
            ".att_syntax\n"
            :
            : "q" (blank), "r" ((short)(cursor_pos * 2)), "q" ((char)color)
            : "ax", "bx", "memory"
        );
        return;
    }

    __asm__ __volatile__ (
        ".intel_syntax noprefix\n"
        "push es\n"
        "mov ax, 0xB800\n"
        "mov es, ax\n"
        "mov bx, %w1\n"
        "mov es:[bx], %b0\n"
        "mov es:[bx+1], %b2\n"
        "pop es\n"
        ".att_syntax\n"
        :
        : "q" (c), "r" ((short)(cursor_pos * 2)), "q" ((char)color)
        : "ax", "bx", "memory"
    );
    cursor_pos++;

    if (cursor_pos % 80 == 0) {
        input_floor = cursor_pos;
    }
    if (cursor_pos >= 2000) {
        scroll();
    }
}

void printst(char *str, int color) {
    for (int i = 0; str[i]; i++) {
        printch(str[i], color);
    };
}
int current_bank = -1;
int screen_width = 800;

void set_vesa_bank(int bank) {
    if (bank == current_bank) {
        return;
    }
    __asm__ __volatile__(
        "int $0x10"
        :
        : "a"(0x4F05), "b"(0), "d"(bank)
    );
    current_bank = bank;
}

void putpixel(short pos_x, short pos_y, unsigned char color) {
    long offset = (long)pos_y * screen_width + pos_x;
    int bank = (int)(offset >> 16);
    unsigned int window_offset = (unsigned int)(offset & 0xFFFF);
    set_vesa_bank(bank);
    unsigned char far* location = (unsigned char far*)(0xA0000000L + window_offset);
    *location = color;
}

void C_START() {
    printst("C_START running successfully\n", 14);
    initata();
    initdrives();
    detection();
    load_files();
    printst("Finished init sequence", 14);
    printst("\nR:", 14);

    while (1) {
        shellloop();
    }
    
}
