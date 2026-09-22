#include "LOAD.h"
#include "stddef.h"



int find_char_on_line(const char *buffer, size_t buffer_size, size_t target_line, char target_char) {
    size_t current_line = 1;
    size_t current_col = 0;

    for (size_t i = 0; i < buffer_size; i++) {
        char c = buffer[i];

        if (current_line == target_line) {
            if (c == '\n' || c == '\r') {
                break;
            }
            
            if (c == target_char) {
                return (int)current_col;
            }
            current_col++;
        }

        if (c == '\n') {
            current_line++;
            if (current_line > target_line) {
                break;
            }
        }
    }

    return -1;
}






