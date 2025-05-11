
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
#include "EntityHandler.h"
#include "core.h"
#include "input.h"
#include "math.h"
#include "time.h"
#include "ansi.h"

void drawScreen(Game* game, bool header, char color[]);
void drawHeader(Game* game);
void drawEndScreen(Game* game);

/**
 * @brief Updates core info, such as frame rate, terminal size, etc.
 */
void updateCoreInfo(Game* game);

/**
 * @brief Updates player info, such as position, speed, etc.
 */
void updatePlayer(Game* game);

/**
 * @brief Updates collisions, this will probably need more elaboration
 */
void updateCollisions(Game* game);

/**
 * @brief Updates UI info, such as current score. Maybe not needed.
 */
void updateUI(Game* game);

/**
 * @brief Updates draw buffer, making it ready for next draw
 */
void updateBuffer(Game* game);
void cleanBuffer(Game* game);

void limitFps(Game* game);
void cleanScreen();

void drawTermToSmallError();

// Public:
void createGame(Game* game) {
    initTerm();

    game->debug = false;
    game->collided = false;

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

    game->bufferSize = (Vect2){BUFFER_WIDTH, BUFFER_HEIGHT};
    game->drawBuffer = malloc(game->bufferSize.y * sizeof(char*));
    for (int32_t i = 0; i < game->bufferSize.y; i++) {
        game->drawBuffer[i] = malloc(game->bufferSize.x * sizeof(char));
    }

    cleanBuffer(game);

    createEntityHandler(&game->enemies, (Vect2){WIDTH, GROUND_LEVEL}, (Vect2){20, 0}, 2000000);
    createEntityHandler(&game->decorators, (Vect2){WIDTH, 4}, (Vect2){8, 0}, 8000000);
}

void deleteGame(Game* game) {
    if (!game->created) {
        return;
    }

    deleteEntityHandler(&game->enemies);
    deleteEntityHandler(&game->decorators);

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

    if (game->collided) {
        drawEndScreen(game);
        drawScreen(game, false, ANSI_BH_RED);
        return true;
    }

    updatePlayer(game);

    const double deltaTime = getDeltaTime(game);
    handleUpdate(&game->enemies, deltaTime);
    handleUpdate(&game->decorators, deltaTime);

    updateUI(game);

   // updateCollisions(game);

    // Draw
    updateBuffer(game);
    drawScreen(game, true, ANSI_YELLOW);

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

void addEnemy(Game* game, char** texture, Vect2 size) {
    addBlueprint(&game->enemies, texture, size);
}

void addDecorator(Game* game,  char** texture, Vect2 size){
    addBlueprint(&game->decorators, texture, size);
}
// Private:

void drawScreen(Game* game, bool header, char color[]) {
    cleanScreen();

    if (header) {
        drawHeader(game);
    }else {
        for (uint32_t i = 0; i < HEADER_HEIGHT; i++) {
            printf("\n");
        }
    }

    if (game->size.y < HEIGHT || game->size.x < WIDTH) {
        drawTermToSmallError();
        return;
    }

    setColor(color);
    for (int32_t i = 0; i < game->bufferSize.y; i++) {
        for (int32_t j = 0; j < game->bufferSize.x; j++) {
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

    Vect2* playerPos = &player->position;

    double deltaTime = getDeltaTime(game);

    playerPos->y -= playerSpeed->y * deltaTime;
    playerSpeed->y -= GRAVITY * deltaTime;
    if (playerPos->y + player->size.y > GROUND_LEVEL) {
        playerPos->y = GROUND_LEVEL - player->size.y;
        playerSpeed->y = 0;
        player->jumpAvailable = true;
    }
}

void updateCollisions(Game* game) {
    Object* player = &game->player;

    for (uint32_t i = 0; i < game->enemies.numActiveEntities; i++) {
        if (areColliding(player, game->enemies.activeEntities[i])) {
            game->collided = true;
            return;
        }
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
    drawObject(player, (char**)game->drawBuffer, game->bufferSize);


    drawEntities(&game->enemies, game->drawBuffer, game->bufferSize);
    drawEntities(&game->decorators, game->drawBuffer, game->bufferSize);

    // TODO
    return;
}

void cleanBuffer(Game* game) {
    for (int32_t i = 0; i < game->bufferSize.y; i++) {
        memset(game->drawBuffer[i], ' ', game->bufferSize.x);
    }
}

void limitFps(Game* game) {
    const uint64_t basicSleepTime = 1000000 / FRAME_RATE;
    const uint64_t deltaTime = getDeltaTime(game) * 1000000;

    uint64_t offset = 0;
    if (deltaTime > basicSleepTime) {
        offset = deltaTime - basicSleepTime;
    }
    if (basicSleepTime - offset > basicSleepTime) {
        offset = 0;
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
        printf("\tWidth: %f | HEIGHT: %f | Frame{rate, time}: {%f, %fs} | Player pos: {%f, %f} | Jump: %d | {Score, mult}: {%f, %f}\n", game->size.x, game->size.y, 1./getDeltaTime(game), getDeltaTime(game), game->player.position.x, game->player.position.y, game->player.jumpAvailable, game->ui.score, game->ui.scoreMultiplier);
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

void drawEndScreen(Game* game) {
    cleanBuffer(game);

    const char *message[] = {
        " _____                           ____                 ",
        "|  __ \\                         / __ \\                ",
        "| |  \\/ __ _ _ __ ___   ___    | |  | |_   _____ _ __ ",
        "| | __ / _` | '_ ` _ \\ / _ \\   | |  | \\ \\ / / _ \\ '__|",
        "| |_\\ \\ (_| | | | | | |  __/   | |__| |\\ V /  __/ |   ",
        " \\____/\\__,_|_| |_| |_|\\___|    \\____/  \\_/ \\___|_|   "
    };

    Vect2 size = {strlen(message[0]),  (float)sizeof(message) / sizeof(message[0])};
    Vect2 pos = {(game->bufferSize.x - size.x) / 2,  game->bufferSize.y / 2 - size.y};

    for (int i = 0; i < size.y; i++) {
        strncpy(&game->drawBuffer[(int)pos.y + i][(int)pos.x], message[i], size.x);
    }

    char shortCutsInfo[] = "Press (q) to exit.";
    Vect2 shortCutsSize = (Vect2){strlen(shortCutsInfo),  1};
    Vect2 shortCutsPos = (Vect2){(game->bufferSize.x - shortCutsSize.x) / 2,  game->bufferSize.y / 2 + shortCutsSize.y};

    strncpy(&game->drawBuffer[(int)shortCutsPos.y + 1][(int)shortCutsPos.x], shortCutsInfo, shortCutsSize.x);


    char scoreBuffer[20];
    sprintf(scoreBuffer, "Score: %ld", (long)game->ui.score);
    Vect2 scoreSize = (Vect2){strlen(scoreBuffer),  1};
    Vect2 scorePos = (Vect2){(game->bufferSize.x - scoreSize.x) / 2,  game->bufferSize.y / 2 + scoreSize.y + 3};

    strncpy(&game->drawBuffer[(int)scorePos.y + 1][(int)scorePos.x], scoreBuffer, scoreSize.x);
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
