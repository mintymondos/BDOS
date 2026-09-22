#include "SHELL.h"
#include "MAIN.h"

unsigned char currentkey;
char keystrokes[2048];
int maxsaveablekeys = 0;
int shifted = 0;

int appendkeystrokes(char c) {
    if (maxsaveablekeys >= 256) {
        return -1;
    }
    
    keystrokes[maxsaveablekeys] = c;
    maxsaveablekeys++;
    
    return 0;
}

static inline unsigned char inb(unsigned short port) { 
    unsigned char result; 
    __asm__ volatile( 
        "inb %1, %0" 
        : "=a" (result) 
        : "Nd" (port)
    ); 
    return result; 
} 

int poll_keyboard(unsigned char *scancode) {
    unsigned char status = inb(0x64);
    
    if ((status & 0x01) && !(status & 0x20)) {
        *scancode = inb(0x60);
        currentkey = *scancode;
        return 1;
    }
    return 0;
}
// wont be addressing the comically large elephant in the room
int keycodeprint() {
    if (poll_keyboard(&currentkey)) {
        if (currentkey == 0x2A) {
            shifted = 1;
        }
        if(currentkey == 0x0E) {
                printch('\b', 15);
                if (keystrokes[0] != '\0') {
                    appendkeystrokes('\b');
                }
                return '\b';
        }    
        if(currentkey == 0x1C) {
                printch('\n', 15);
                return '\n';
        }    
        if(currentkey == 0x10) {
            if (shifted == 1) {
                printch('Q', 15);
                appendkeystrokes('Q');
                return 'Q';
            } else {
                printch('q', 15);
                appendkeystrokes('q');
                return 'q';
            }
        }    
        else if(currentkey == 0x11) {
            if (shifted == 1) {
                printch('W', 15);
                appendkeystrokes('W');
                return 'W';
            } else {
                printch('w', 15);
                appendkeystrokes('w');
                return 'w';
            }
        }    
        else if(currentkey == 0x12) {
            if (shifted == 1) {
                printch('E', 15);
                appendkeystrokes('E');
                return 'E';
            } else {
                printch('e', 15);
                appendkeystrokes('e');
                return 'e';
            }
        }    
        else if(currentkey == 0x13) {
            if (shifted == 1) {
                printch('R', 15);
                appendkeystrokes('R');
                return 'R';
            } else {
                printch('r', 15);
                appendkeystrokes('r');
                return 'r';
            }
        }    
        else if(currentkey == 0x14) {
            if (shifted == 1) {
                printch('T', 15);
                appendkeystrokes('T');
                return 'T';
            } else {
                printch('t', 15);
                appendkeystrokes('t');
                return 't';
            }
        }    
        else if(currentkey == 0x15) {
            if (shifted == 1) {
                printch('Y', 15);
                appendkeystrokes('Y');
                return 'Y';
            } else {
                printch('y', 15);
                appendkeystrokes('y');
                return 'y';
            }
        }    
        else if(currentkey == 0x16) {
            if (shifted == 1) {
                printch('U', 15);
                appendkeystrokes('U');
                return 'U';
            } else {
                printch('u', 15);
                appendkeystrokes('u');
                return 'u';
            }
        }    
        else if(currentkey == 0x17) {
            if (shifted == 1) {
                printch('I', 15);
                appendkeystrokes('I');
                return 'I';
            } else {
                printch('i', 15);
                appendkeystrokes('i');
                return 'i';
            }
        }    
        else if(currentkey == 0x18) {
            if (shifted == 1) {
                printch('O', 15);
                appendkeystrokes('O');
                return 'O';
            } else {
                printch('o', 15);
                appendkeystrokes('o');
                return 'o';
            }
        }    
        else if(currentkey == 0x19) {
            if (shifted == 1) {
                printch('P', 15);
                appendkeystrokes('P');
                return 'P';
            } else {
                printch('p', 15);
                appendkeystrokes('p');
                return 'p';
            }
        }    
        else if(currentkey == 0x1A) {
            if (shifted == 1) {
                printch('{', 15);
                appendkeystrokes('{');
                return '{';
            } else {
                printch('[', 15);
                appendkeystrokes('[');
                return '[';
            }
        }
        else if(currentkey == 0x1B) {
            if (shifted == 1) {
                printch('}', 15);
                appendkeystrokes('}');
                return '}';
            } else {
                printch(']', 15);
                appendkeystrokes(']');
                return ']';
            }
        }    
        else if(currentkey == 0x1E) {
            if (shifted == 1) {
                printch('A', 15);
                appendkeystrokes('A');
                return 'A';
            } else {
                printch('a', 15);
                appendkeystrokes('a');
                return 'a';
            }
        }    
        else if(currentkey == 0x1F) {
            if (shifted == 1) {
                printch('S', 15);
                appendkeystrokes('S');
                return 'S';
            } else {
                printch('s', 15);
                appendkeystrokes('s');
                return 's';
            }
        }    
        else if(currentkey == 0x20) {
            if (shifted == 1) {
                printch('D', 15);
                appendkeystrokes('D');
                return 'D';
            } else {
                printch('d', 15);
                appendkeystrokes('d');
                return 'd';
            }
        }    
        else if(currentkey == 0x21) {
            if (shifted == 1) {
                printch('F', 15);
                appendkeystrokes('F');
                return 'F';
            } else {
                printch('f', 15);
                appendkeystrokes('f');
                return 'f';
            }
        }    
        else if(currentkey == 0x22) {
            if (shifted == 1) {
                printch('G', 15);
                appendkeystrokes('G');
                return 'G';
            } else {
                printch('g', 15);
                appendkeystrokes('g');
                return 'g';
            }
        }    
        else if(currentkey == 0x23) {
            if (shifted == 1) {
                printch('H', 15);
                appendkeystrokes('H');
                return 'H';
            } else {
                printch('h', 15);
                appendkeystrokes('h');
                return 'h';
            }
        }    
        else if(currentkey == 0x24) {
            if (shifted == 1) {
                printch('J', 15);
                appendkeystrokes('J');
                return 'J';
            } else {
                printch('j', 15);
                appendkeystrokes('j');
                return 'j';
            }
        }    
        else if(currentkey == 0x25) {
            if (shifted == 1) {
                printch('K', 15);
                appendkeystrokes('K');
                return 'K';
            } else {
                printch('k', 15);
                appendkeystrokes('k');
                return 'k';
            }
        }    
        else if(currentkey == 0x26) {
            if (shifted == 1) {
                printch('L', 15);
                appendkeystrokes('L');
                return 'L';
            } else {
                printch('l', 15);
                appendkeystrokes('l');
                return 'l';
            }
        }    
        else if(currentkey == 0x27) {
            if (shifted == 1) {
                printch(':', 15);
                appendkeystrokes(':');
                return ':';
            } else {
                printch(';', 15);
                appendkeystrokes(';');
                return ';';
            }
        }    
        else if(currentkey == 0x28) {
            if (shifted == 1) {
                printch('"', 15);
                appendkeystrokes('"');
                return '"';
            } else {
                printch('\'', 15);
                appendkeystrokes('\'');
                return '\'';
            }
        }    
        else if(currentkey == 0x2C) {
            if (shifted == 1) {
                printch('Z', 15);
                appendkeystrokes('Z');
                return 'Z';
            } else {
                printch('z', 15);
                appendkeystrokes('z');
                return 'z';
            }
        }    
        else if(currentkey == 0x2D) {
            if (shifted == 1) {
                printch('X', 15);
                appendkeystrokes('X');
                return 'X';
            } else {
                printch('x', 15);
                appendkeystrokes('x');
                return 'x';
            }
        }    
        else if(currentkey == 0x2E) {
            if (shifted == 1) {
                printch('C', 15);
                appendkeystrokes('C');
                return 'C';
            } else {
                printch('c', 15);
                appendkeystrokes('c');
                return 'c';
            }
        }
        else if(currentkey == 0x2F) {
            if (shifted == 1) {
                printch('V', 15);
                appendkeystrokes('V');
                return 'V';
            } else {
                printch('v', 15);
                appendkeystrokes('v');
                return 'v';
            }
        }    
        else if(currentkey == 0x30) {
            if (shifted == 1) {
                printch('B', 15);
                appendkeystrokes('B');
                return 'B';
            } else {
                printch('b', 15);
                appendkeystrokes('b');
                return 'b';
            }
        }
        else if(currentkey == 0x31) {
            if (shifted == 1) {
                printch('N', 15);
                appendkeystrokes('N');
                return 'N';
            } else {
                printch('n', 15);
                appendkeystrokes('n');
                return 'n';
            }
        }    
        else if(currentkey == 0x32) {
            if (shifted == 1) {
                printch('M', 15);
                appendkeystrokes('M');
                return 'M';
            } else {
                printch('m', 15);
                appendkeystrokes('m');
                return 'm';
            }
        }
        else if(currentkey == 0x33) {
            if (shifted == 1) {
                printch('<', 15);
                appendkeystrokes('<');
                return '<';
            } else {
                printch(',', 15);
                appendkeystrokes(',');
                return ',';
            }
        }    
        else if(currentkey == 0x34) {
            if (shifted == 1) {
                printch('>', 15);
                appendkeystrokes('>');
                return '>';
            } else {
                printch('.', 15);
                appendkeystrokes('.');
                return '.';
            }
        }
        else if(currentkey == 0x35) {
            if (shifted == 1) {
                printch('?', 15);
                appendkeystrokes('?');
                return '?';
            } else {
                printch('/', 15);
                appendkeystrokes('/');
                return '/';
            }
        }    
        else if(currentkey == 0x39) {
            printch(' ', 15);
            appendkeystrokes(' ');
            return ' ';
        }
        else if (currentkey == 0x1C) {
            printch('\n', 15);
            return '\n';
        }
        else if(currentkey == 0x02) {
            if (shifted == 1) {
                printch('!', 15);
                appendkeystrokes('!');
                return '!';
            } else {
                printch('1', 15);
                appendkeystrokes('1');
                return '1';
            }
        }
        else if(currentkey == 0x03) {
            if (shifted == 1) {
                printch('@', 15);
                appendkeystrokes('@');
                return '@';
            } else {
                printch('2', 15);
                appendkeystrokes('2');
                return '2';
            }
        }    
        else if(currentkey == 0x04) {
            if (shifted == 1) {
                printch('#', 15);
                appendkeystrokes('#');
                return '#';
            } else {
                printch('3', 15);
                appendkeystrokes('3');
                return '3';
            }
        }
        else if(currentkey == 0x05) {
            if (shifted == 1) {
                printch('$', 15);
                appendkeystrokes('$');
                return '$';
            } else {
                printch('4', 15);
                appendkeystrokes('4');
                return '4';
            }        
        }    
        else if(currentkey == 0x06) {
            if (shifted == 1) {
                printch('%', 15);
                appendkeystrokes('%');
                return '%';
            } else {
                printch('5', 15);
                appendkeystrokes('5');
                return '5';
            }        
        }
        else if(currentkey == 0x07) {
            if (shifted == 1) {
                printch('^', 15);
                appendkeystrokes('^');
                return '^';
            } else {
                printch('6', 15);
                appendkeystrokes('6');
                return '6';
            }     
        }    
        else if(currentkey == 0x08) {
            if (shifted == 1) {
                printch('&', 15);
                appendkeystrokes('&');
                return '&';
            } else {
                printch('7', 15);
                appendkeystrokes('7');
                return '7';
            }     
        }
        else if(currentkey == 0x09) {
            if (shifted == 1) {
                printch('*', 15);
                appendkeystrokes('*');
                return '*';
            } else {
                printch('8', 15);
                appendkeystrokes('8');
                return '8';
            }     
        }    
        else if(currentkey == 0x0A) {
            if (shifted == 1) {
                printch('(', 15);
                appendkeystrokes('(');
                return '(';
            } else {
                printch('9', 15);
                appendkeystrokes('9');
                return '9';
            }     
        }
        else if(currentkey == 0x0B) {
            if (shifted == 1) {
                printch(')', 15);
                appendkeystrokes(')');
                return ')';
            } else {
                printch('0', 15);
                appendkeystrokes('0');
                return '0';
            }     
        }    
        else if(currentkey == 0x0C) {
            if (shifted == 1) {
                printch('_', 15);
                appendkeystrokes('_');
                return '_';
            } else {
                printch('-', 15);
                appendkeystrokes('-');
                return '-';
            }             
        }
        else if(currentkey == 0x0D) {
            if (shifted == 1) {
                printch('+', 15);
                appendkeystrokes('+');
                return '+';
            } else {
                printch('=', 15);
                appendkeystrokes('=');
                return '=';
            }     
        }
        else if(currentkey == 0x29) {
            if (shifted == 1) {
                printch('~', 15);
                appendkeystrokes('~');
                return '~';
            } else {
                printch('`', 15);
                appendkeystrokes('`');
                return '`';
            }     
        }
        else if(currentkey == 0xAA) {
            shifted = 0;
        }
    }
    return 0;
}
// the end of the comically large elephant in the room
