
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ncurses.h>
#include <termios.h>

#include "game.h"
#include "input.h"
#include "time.h"
#include "ansi.h"
#include "term.h"

// TODO -
void drawScreen(Game* game,  bool debug);
void drawHeader(Game* game);
// TODO - Update ahora es publica, se encarga de todo
//        Deben haber varias privadas que hagan:
//          - Update Screen
//          - Update Player
//          - Update Enemies
//          - Update Decorators
//          - Update Collisions
//          - Update UI (Quizas no haga falta, quizás si)
void updateScreen(Game* game);
void updatePlayer(Game* game);
void updateEnemies(Game* game);
void updateDecorators(Game* game);
void updateCollisions(Game* game);
void updateUI(Game* game);
void limitFps(Game* game);
void cleanScreen();

void drawTermToSmallError();

// Public:
void createGame(Game* game) {
    initTerm();

    createInput(&game->input);
    game->fpsTarget = UINT32_MAX;

    game->lastFrameTime = getCurrentMicroseconds();
    updateScreen(game);

    game->created = true;
}

void deleteGame(Game* game) {
    if (!game->created) {
        return;
    }
    game->created = false;

    restoreTerm();
    cleanScreen();
}

void setFpsTarget(Game* game, uint32_t target) {
    if (!game->created) {
        return;
    }

    game->fpsTarget = target;
}

bool update(Game* game, bool debug) {
    if (!game->created) {
        return false;
    }

    processInput(&game->input);

    if (isKeyPressed(&game->input, 'q')) {
        return false;
    }

    updateScreen(game);
    limitFps(game);

    drawScreen(game, debug);
    return true;
}

double getDeltaTime(Game* game) {
    if (!game->created) {
        return -1.;
    }

    return game->deltaTime;
}

// Private:

void drawScreen(Game* game, bool debug) {
    cleanScreen();

    if (debug) {
        drawHeader(game);
    }

    if (game->size.y < HEIGTH || game->size.x < WIDTH) {
        drawTermToSmallError();
        return;
    }

    setColor(ANSI_YELLOW);
    for (uint32_t i = 0; i < HEIGTH; i++) {
        for (uint32_t i = 0; i < WIDTH; i++) {
            printf("0");
        }
        printf("\n");
    }
    resetColor();

    fflush(stdout);
}


void updateScreen(Game* game) {
    game->deltaTime = (getCurrentMicroseconds() - game->lastFrameTime) / 1000000.0f;
    game->lastFrameTime = getCurrentMicroseconds();

    struct winsize termSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &termSize);
    game->size = (Vect2){.x = termSize.ws_col, .y = termSize.ws_row - HEADER_SIZE};
}

void limitFps(Game* game) {
    (void) game;
    uint64_t basicSleepTime = 1000000 / FRAME_RATE;
    usleep(basicSleepTime);
}

void cleanScreen() {
    system("clear");
}

void drawHeader(Game *game) {
    setColor(ANSI_CYAN);
    printf("\n\tWidth: %d | Heigth: %d | FrameTime: %fs\n", game->size.x, game->size.y, getDeltaTime(game));
    resetColor();

    uint32_t speratorNum = WIDTH <= game->size.x ? WIDTH : game->size.x;

    setColor(ANSI_MAGENTA);
    for (uint32_t i = 0; i < speratorNum; i++) {
        printf(HEADER_SEPARATOR);
    }
    printf("\n");
    resetColor();
}

void drawTermToSmallError() {

    char widthText[10];
    sprintf(widthText, "%d", WIDTH);
    char heigthText[10];
    sprintf(heigthText, "%d", HEIGTH);

    setColor(ANSI_BH_RED);
    printf("\n\tTerminal size to small, min size {%s, %s}\n", widthText, heigthText);
    resetColor();
}
