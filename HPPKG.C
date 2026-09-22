#include "ATA.h"
#include "FDC.h"
#include "MAIN.h"
#include "DET.h"

uint16_t pkgbuffer[2048];


void probablysystemdirs(driveloco_t *drive) {
    uint32_t sz1 = 0;
    uint16_t check = fat16(drive, "SYSTEM     ", &sz1);
    if (check == 0) {
        makedir(drive, "SYSTEM     ");
    }
    chdir(drive, "SYSTEM     ");
    uint32_t sz2 = 0;
    uint16_t check2 = fat16(drive, "HPPKG      ", &sz2);
    if (check2 == 0) {
        makedir(drive, "HPPKG      ");
    }
    chdir(drive, "HPPKG      ");
    uint32_t sz3 = 0;
    uint16_t check3 = fat16(drive, "INSTALLD   ", &sz3);
    if (check3 == 0) {
        makedir(drive, "INSTALLD   ");
    }
    chdir(drive, "INSTALLD   ");
    chdir(drive, "..         ");
    chdir(drive, "..         ");
    uint32_t sz4 = 0;
    uint16_t check4 = fat16(drive, "CMD        ", &sz4);
    if (check4 == 0) {
        makedir(drive, "CMD        ");
    }
    chdir(drive, "CMD        ");
}

uint8_t installpkg(driveloco_t *floopy, driveloco_t *harddisk, char *pkgname, char *cmdname) {
    uint32_t pkgsize = 0;

    uint16_t pkgclust = fat16(floopy, pkgname, &pkgsize);
    if (pkgclust == 0) {
        printst("HPPKG ERROR ", 4);
        printst("Couldn't find package on floppy.\n", 15);
        return 1;
    }
    readfile(floopy, pkgclust, pkgbuffer, 2048);
    probablysystemdirs(harddisk);
    chdir(harddisk, "..         ");
    chdir(harddisk, "HPPKG      ");
    chdir(harddisk, "INSTALLD   ");
    writefile(harddisk, pkgname, pkgbuffer, pkgsize);
    chdir(harddisk, "..         ");
    chdir(harddisk, "..         ");
    chdir(harddisk, "CMD        ");
    writefile(harddisk, cmdname, (uint16_t *)pkgname, 11);
    return 0;
}

uint8_t exfile(driveloco_t *harddisk, char *cmdname, uint16_t *buffer) {
    probablysystemdirs(harddisk);
    uint32_t linksize = 0;
    uint16_t linkclus = fat16(harddisk, cmdname, &linksize);
    if (linkclus == 0) {
        printst("HPPKG ERROR ", 4);
        printst("Couldn't find command link\n", 15);
        return 1;
    }
    uint16_t linkbuff[8];
    readfile(harddisk, linkclus, linkbuff, 8);
    uint8_t *lb = (uint8_t *)linkbuff;
    chdir(harddisk, "..         ");
    chdir(harddisk, "HPPKG      ");
    chdir(harddisk, "INSTALLD   ");
    uint32_t realsize = 0;
    uint16_t realclus = fat16(harddisk, (char *)lb, &realsize);
    if (realclus == 0 ) {
        printst("HPPKG ERROR ", 4);
        printst("/SYSTEM/CMD/", 14);
        printst(cmdname, 14);
        printst(" Linked to unknown file\n", 15);
        return 1;
    }
    readfile(harddisk, realclus, buffer, 2048);
    typedef void (*entry_t)(void);
    entry_t entry = (entry_t)buffer;
    entry();
    return 0;

}
