#include "ATA.h"
void probablysystemdirs(driveloco_t *drive);
uint8_t installpkg(driveloco_t *floopy, driveloco_t *harddisk, char *pkgname, char *cmdname);
uint8_t exfile(driveloco_t *harddisk, char *cmdname, uint16_t *buffer);
