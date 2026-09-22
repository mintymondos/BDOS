#include "MAIN.h"
#include "LOAD.h"
extern int input_floor;
void FUCK() {
    input_floor = 0;
    for (int i = 0; i < 80 * 25; i++) {
        printch('\b', 0);
    }
    printst("!KERNEL PANIC!\n", 4);
    printst("BDOS cannot safely continue operating, please reboot your system!", 4);

    while (1) {
        __asm__ __volatile__ (
            "hlt"
        );
    }
}