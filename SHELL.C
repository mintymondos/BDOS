#include "MAIN.h"
#include "KEY.h"
#include "LOAD.h"
#include "PANIC.H"
#include "ATA.h"
#include "INSTALLER.h"
#include "HPPKG.h"
#include "ugui.h"


extern char keystrokes[2048];
extern int maxsaveablekeys;
extern char ind[6556];
extern const char _binary_HumbleBumble_txt_end[];
extern const int _binary_HumbleBumble_txt_size[];
extern const char _binary_HumbleBumble_txt_start[];
int framebuffer = 0;
char padded[11];
uint16_t execbuf[2048];

void padname(char *input, char *output) {
    for (int i = 0; i < 11; i++) {
        output[i] = ' ';
    }
    for (int i = 0; i < 8; i++) {
        if (input[i] == '\0') {
            break;
        }
        else {
            output[i] = input[i];
        }
    }
}


void shellloop() {
    if (framebuffer == 0) {
        if (keycodeprint() == '\n') {
            padname(keystrokes, padded);
            if (strcmp(keystrokes, "framebuffer") == 0) {
                printst("entering framebuffer\n", 15);
                __asm__ __volatile__ (
                    "int $0x10"
                    :
                    : "a" (0x0013)
                );
                framebuffer = 1;
            }
            else if (strcmp(keystrokes, "KERNELPANICTRUE") == 0) {
                FUCK();
            }
            else if (strcmp(keystrokes, "help") == 0 || strcmp(keystrokes, "HELP") == 0 ) {
                printst("COMMANDS\n", 14);
                printst("DIR\n", 15);
                printst("[FILENAME] - runs programs\n", 15);
                printst("CHDIR [ARG]\n", 15);
                printst("DEL [ARG]\n", 15);
                printst("MKDIR [ARG]\n", 15);
                printst("VER\n", 15);
                printst("FORMAT [ARG]\n", 15);
                printst("A:\\\n", 15);
                printst("R:\\\n", 15);
                printst("KERNELPANICTRUE - triggers a forced kernel panic\n", 4);

            }
            else if (strcmp(keystrokes, "DIR") == 0) {

            }
            else if (strcmp(keystrokes, "CHDIR") == 0) {

            }
            else if (strcmp(keystrokes, "DEL") == 0) {

            }
            else if (strcmp(findindex(keystrokes, 0, 6), "MKDIR ") == 0 || strcmp(findindex(keystrokes, 0, 6), "mkdir ") == 0) {
            if (strcmp(keystrokes + 6, '\0') == 0 || strcmp(keystrokes, " ") == 0) {
                    printst("Not a valid directory name", 15);
            }
            else {
                makedir(current_drive, findindex(keystrokes + 6, 6, stringlen(keystrokes)));
                }
            }
            else if (strcmp(keystrokes, "VER") == 0) {
                printst("BDOS - Bumble DOS\n", 14);
                printst("V0.01\n", 15);
                printst("HPPKG VERS\n", 14);
                printst("V0.01\n", 15);
                const char *hbstart = _binary_HumbleBumble_txt_start;
                while (hbstart < _binary_HumbleBumble_txt_end) {
                    char c = *hbstart++;
                    printch(c, 14);
                }
            }
            else if (strcmp(keystrokes, "FORMAT R") == 0) {
                format(&driver);
            }
            else if (strcmp(keystrokes, "FORMAT A") == 0) {
                format(&drivea);
            }
            else if (strcmp(keystrokes, "A:") == 0) {
                current_drive = &drivea;
            }
            else if (strcmp(keystrokes, "R:") == 0) {
                current_drive = &driver;
            }
            else if (strcmp(findindex(keystrokes, 0, 14), "HPPKG-INSTALL ") == 0) {
                padname(keystrokes + 14, padded);
                installpkg(&drivea, current_drive, padded, padded);
            }
            else if (strcmp(keystrokes, "") == 0) {

            }
            else if (exfile(current_drive, padded, execbuf) == 0) {
                printst("WIP bro\n", 15);
            }
            else {
                printst("error: couldnt find command or file\"", 4);
                printst(keystrokes, 13);
                printch('\"', 4);
                printch('\n', 0);
            }
            for (int i = 0; i < 2048; i++) {
                keystrokes[i] = '\0';
            }
            maxsaveablekeys = 0;
            printst("R:", 14);
        }
    }
}
