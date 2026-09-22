#include <stdint.h>
void lbatochs(uint16_t lba, uint8_t *outcyl, uint8_t *outhead, uint8_t *outsect);
uint8_t floopyrlba(uint16_t lba, uint16_t *buffer);
uint8_t floopywlba(uint16_t lba, uint16_t *buffer);
