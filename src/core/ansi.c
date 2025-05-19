#include "ansi.h"

void setColor(char* color) {
    printf("%s", color);
}

void resetColor() {
    printf("%s", ANSI_RESET);
}