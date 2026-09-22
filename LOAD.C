#include "MAIN.h"
#include "MISL.h"
#include "PANIC.H"
#include <stddef.h>
extern const char _binary_CMD_MS_start[];
extern const char _binary_CMD_MS_size[];
extern const char _binary_CMD_MS_end[];
extern const char _binary_KCI_MS_start[];
extern const char _binary_KCI_MS_size[];
extern const char _binary_KCI_MS_end[];

typedef struct {
    size_t line_number;
    size_t column_index;
} MatchLocation;

void load_files(void) {
    const char *CMD_start = _binary_CMD_MS_start;
    const char *CMD_size = _binary_CMD_MS_size;
    const char *CMD_end = _binary_CMD_MS_end;
    const char *KCI_start = _binary_KCI_MS_start;
    const char *KCI_size = _binary_KCI_MS_size;
    const char *KCI_end = _binary_KCI_MS_end;
    
    if (CMD_size == 0 || KCI_size == 0) {
        FUCK();
    }


    while (CMD_start < CMD_end) {
        char c = *CMD_start++;
        printch(c, 14);
    }
    printch('\n', 0);
    while (KCI_start < KCI_end) {
        char c = *KCI_start++;
        printch(c, 14);
    }
}