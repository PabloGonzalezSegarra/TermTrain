#ifndef TERM_TRAIN_SCREEN_INCLUDE__
#define TERM_TRAIN_SCREEN_INCLUDE__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "math.h"
#include "input.h"
#include "EntityHandler.h"
#include "core.h"

#define HEADER_HEIGHT ((uint32_t)4)
#define HEADER_SEPARATOR "_"
#define FRAME_RATE ((uint32_t)25)

#define WIDTH (int32_t)(100)
#define HEIGHT (int32_t)(25)

#define BUFFER_WIDTH (int32_t)(WIDTH)
#define BUFFER_HEIGHT (int32_t)(HEIGHT - HEADER_HEIGHT)

#define GRAVITY 112

#define GROUND_LEVEL (BUFFER_HEIGHT - 2)

#define BASE_SCORE (33)
#define BASE_MULTIPLIER (0.07)

typedef struct {
    Vect2 size;
    uint64_t lastFrameTime;
    uint32_t fpsTarget;
    double deltaTime;
    bool debug;
    char** drawBuffer;
    Vect2 bufferSize;

    UI ui;
    Object player;

    Input input;

    EntityHandler enemies;
    EntityHandler decorators;

    bool created;
    bool collided;
} Game;

void createGame(Game* game);
void deleteGame(Game* game);
void setFpsTarget(Game* game, uint32_t target);
bool update(Game* game);
double getDeltaTime(Game* game);
void enableDebug(Game* game);
void disableDebug(Game* game);

void setPlayerTexture(Game* game, char** texture, Vect2 size);
void addEnemy(Game* game, char** texture, Vect2 size);
void addDecorator(Game* game, char** texture, Vect2 size);

#endif // TERM_TRAIN_SCREEN_INCLUDE__
