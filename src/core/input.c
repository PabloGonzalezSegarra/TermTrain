#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "input.h"

static struct termios oldTerm, newTerm;

void initTerm() {
    tcgetattr(STDIN_FILENO, &oldTerm);
    newTerm = oldTerm;
    newTerm.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK );

    fflush(stdout);
}

void restoreTerm() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
    fflush(stdout);
}

void createInput(Input *input) {
    memset(input->current, false, INPUT_SIZE);
    memset(input->previous, false, INPUT_SIZE);
}

void processInput(Input* input) {
    memcpy(input->previous, input->current, INPUT_SIZE);
    memset(input->current, false, INPUT_SIZE);

    unsigned char chr;
    while(read(STDIN_FILENO, &chr, 1) > 0) {
        input->current[chr] = true;
    }
}

bool isKeyPressed(Input *input, unsigned char key) {
    return input->previous[key] == false && input->current[key] == true;
}

bool isKeyReleased(Input* input, unsigned char key) {
    return input->previous[key] == true && input->current[key] == false;
}

bool isKeyHold(Input* input, unsigned char key) {
    return input->previous[key] == true && input->current[key] == true;
}
