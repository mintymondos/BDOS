#include "ATA.h"
#include "DET.h"
#include "MAIN.h"
#include "INSTALLER.h"
#include <stdint.h>
extern uint16_t identify_data[256];

uint8_t format(driveloco_t *drive) {
    uint16_t sect0[256];
    drive->readsector(0, 1, sect0);
    uint8_t *s0 = (uint8_t *)sect0;
    if (s0[0x27] == 'H' && s0[0x28] == 'P' && s0[0x29] == 'K' && s0[0x2A] == 'G') {
        return 0;
    }
    s0[0x0B] = 512 & 0xFF;
    s0[0x0C] = (512 >> 8) & 0xFF;
    s0[0x0D] = 1;
    s0[0x0E] = 1;
    s0[0x0F] = 0;
    s0[0x10] = 2;
    s0[0x11] = 512 & 0xFF;
    s0[0x12] = (512 >> 8) & 0xFF;
    s0[0x15] = 0xF8;
    uint32_t totsects = (identify_data[60] | identify_data[61] << 16);
    if (totsects > 65535) {
        s0[0x13] = 0;
        s0[0x14] = 0;
    }
    else {
        s0[0x13] = totsects & 0xFF;
        s0[0x14] = (totsects >> 8) & 0xFF;
    }
    s0[0x20] = totsects & 0xFF;
    s0[0x21] = (totsects >> 8) & 0xFF;
    s0[0x22] = (totsects >> 16) & 0xFF;
    s0[0x23] = (totsects >> 24);
    uint32_t fatsize = (totsects * 2) / 512 + 1;
    s0[0x16] = fatsize & 0xFF;
    s0[0x17] = (fatsize >> 8) & 0xFF;
    s0[0x27] = 'H';
    s0[0x28] = 'P';
    s0[0x29] = 'K';
    s0[0x2A] = 'G';
    drive->writesector(0, 1, sect0);
    uint16_t zerobuf[256];
    for (int i = 0; i < 256; i++) {
        zerobuf[i] = 0;
    }
    uint32_t totalfucksects = fatsize * 2;
    for (uint32_t i = 0; i < totalfucksects; i++) {
        drive->writesector(1 + i, 1, zerobuf);
    }
    uint32_t rootstart = 1 + totalfucksects;
    uint32_t rootsect = (512 * 32) / 512;
    for (uint32_t i = 0; i < rootsect; i++) {
        drive->writesector(rootstart + i, 1, zerobuf);
    }
    return 1;
}
