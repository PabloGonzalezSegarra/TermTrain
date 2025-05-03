
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ncurses.h>
#include <termios.h>

#include "game.h"
#include "input.h"
#include "math.h"
#include "time.h"
#include "ansi.h"

void drawScreen(Game* game);
void drawHeader(Game* game);

/**
 * @brief Updates core info, such as frame rate, terminal size, etc.
 */
void updateCoreInfo(Game* game);

/**
 * @brief Updates player info, such as position, speed, etc.
 */
void updatePlayer(Game* game);

/**
 * @brief Updates enemies info, such as position, speed, etc.
 */
//void updateEnemies(Game* game);

/**
 * @brief Updates decorators info, such as position, speed, etc.
 */
//void updateDecorators(Game* game);

/**
 * @brief Updates collisions, this will probably need more elaboration
 */
//void updateCollisions(Game* game);

/**
 * @brief Updates UI info, such as current score. Maybe not needed.
 */
void updateUI(Game* game);

/**
 * @brief Updates draw buffer, making it ready for next draw
 */
void updateBuffer(Game* game);
void cleanBuffer(Game* game);

bool pixelInScreen(Vect2 pixelPos);

void limitFps(Game* game);
void cleanScreen();

void drawTermToSmallError();

// Public:
void createGame(Game* game) {
    initTerm();

    game->debug = false;

    createInput(&game->input);
    game->fpsTarget = UINT32_MAX;

    game->lastFrameTime = getCurrentMicroseconds();
    updateCoreInfo(game);

    game->created = true;

    game->player.jumpAvailable = false;
    game->player.jumpSpeed = 40;
    game->player.position = (Vect2){4 ,0 };
    game->player.speed = (Vect2){0 ,0 };
    game->player.size = (Vect2){0 ,0 };

    game->ui.score = 0;
    game->ui.scoreMultiplier = 1.f;
    cleanBuffer(game);
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

bool update(Game* game) {
    if (!game->created) {
        return false;
    }
    // Core stuff
    updateCoreInfo(game);

    // Game logic

    if (isKeyPressed(&game->input, 'q')) {
        return false;
    }

    updatePlayer(game);

    updateUI(game);

    // Draw
    updateBuffer(game);
    drawScreen(game);
    return true;
}

double getDeltaTime(Game* game) {
    if (!game->created) {
        return -1.;
    }

    return game->deltaTime;
}

void enableDebug(Game *game) {
    game->debug = true;
}

void disableDebug(Game *game) {
    game->debug = false;
}

void setPlayerTexture(Game* game, char** texture, Vect2 size) {
    game->player.texture = texture;
    game->player.size = size;
}

// Private:

void drawScreen(Game* game) {
    cleanScreen();

    drawHeader(game);

    if (game->size.y < HEIGHT || game->size.x < WIDTH) {
        drawTermToSmallError();
        return;
    }

    setColor(ANSI_YELLOW);
    for (uint32_t i = 0; i < BUFFER_HEIGHT; i++) {
        for (uint32_t j = 0; j < BUFFER_WIDTH; j++) {
            printf("%c", game->drawBuffer[i][j]);
        }
        printf("\n");
    }
    resetColor();

    fflush(stdout);
}


void updateCoreInfo(Game* game) {
    processInput(&game->input);

    game->deltaTime = (getCurrentMicroseconds() - game->lastFrameTime) / 1000000.0f;
    game->lastFrameTime = getCurrentMicroseconds();

    struct winsize termSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &termSize);
    game->size = (Vect2){.x = termSize.ws_col, .y = termSize.ws_row - HEADER_HEIGHT};

    if (game->fpsTarget != UINT32_MAX) {
        limitFps(game);
    }
}

void updatePlayer(Game* game) {
    Object* player = &game->player;
    Vect2* playerSpeed = &player->speed;

    if (player->jumpAvailable && isKeyPressed(&game->input, ' ')) {
        playerSpeed->y = player->jumpSpeed;
        player->jumpAvailable = false;
    }

    //Vect2* playerSize =  &player->object.size;
    Vect2* playerPos = &player->position;

    double deltaTime = getDeltaTime(game);

    playerPos->y -= playerSpeed->y * deltaTime;
    playerSpeed->y -= GRAVITY * deltaTime;

    if (playerPos->y + player->size.y > GROUND_LEVEL) {
        playerPos->y = GROUND_LEVEL;
        playerSpeed->y = 0;
        player->jumpAvailable = true;
    }
}

void updateUI(Game* game) {
    const double deltaTime = getDeltaTime(game);
    game->ui.score += (deltaTime * BASE_SCORE) * game->ui.scoreMultiplier;

    game->ui.scoreMultiplier += BASE_MULTIPLIER * deltaTime;
}

void updateBuffer(Game* game) {
    cleanBuffer(game);

    Object* player = &game->player;

    for (int i = 0; i < player->size.y; i++) {
        for (int j = 0; j < player->size.x; j++) {
            Vect2 pixelPos = (Vect2) {
                player->position.x + j,
                player->position.y + i
            };
            if (pixelInScreen(pixelPos)) {
                game->drawBuffer[pixelPos.y][pixelPos.x] = player->texture[i][j];
            }
        }
    }

    // TODO
    return;
}

void cleanBuffer(Game* game) {
    for (uint32_t i = 0; i < BUFFER_HEIGHT; i++) {
        memset(game->drawBuffer[i], ' ', BUFFER_WIDTH);
    }
}

bool pixelInScreen(Vect2 pixelPos) {
    return pixelPos.y >= 0 && pixelPos.y < BUFFER_HEIGHT &&
           pixelPos.x >= 0 && pixelPos.x < BUFFER_WIDTH;
}


void limitFps(Game* game) {
    const uint64_t basicSleepTime = 1000000 / FRAME_RATE;
    const uint64_t deltaTime = getDeltaTime(game) * 1000000;

    uint64_t offset = 0;
    if (deltaTime > basicSleepTime) {
        offset = deltaTime - basicSleepTime;
    }
    usleep(basicSleepTime - offset);
}

void cleanScreen() {
    (void)system("clear");
}

void drawHeader(Game *game) {


    setColor(ANSI_B_PINK);
    printf("\n\tScore: %ld\n", (uint64_t)game->ui.score);
    resetColor();

    uint32_t separatorNum = WIDTH <= game->size.x ? WIDTH : game->size.x;

    if (game->debug) {
        setColor(ANSI_CYAN);
        printf("\tWidth: %d | HEIGHT: %d | Frame{rate, time}: {%f, %fs} | Player pos: {%d, %d} | Jump: %d\n | {Score, mult}: {%f, %f}\n", game->size.x, game->size.y, 1./getDeltaTime(game), getDeltaTime(game), game->player.position.x, game->player.position.y, game->player.jumpAvailable, game->ui.score, game->ui.scoreMultiplier);
        resetColor();
    }else {
        printf("\n");
    }

    setColor(ANSI_B_PINK);
    for (uint32_t i = 0; i < separatorNum; i++) {
        printf(HEADER_SEPARATOR);
    }
    printf("\n");
    resetColor();
}

void drawTermToSmallError() {

    char widthText[10];
    sprintf(widthText, "%d", WIDTH);
    char HEIGHTText[10];
    sprintf(HEIGHTText, "%d", HEIGHT);

    setColor(ANSI_BH_RED);
    printf("\n\tTerminal size to small, min size {%s, %s}\n", widthText, HEIGHTText);
    resetColor();
}
