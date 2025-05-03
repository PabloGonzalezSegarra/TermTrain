#ifndef TERM_TRAIN_SCREEN_INCLUDE__
#define TERM_TRAIN_SCREEN_INCLUDE__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "math.h"
#include "input.h"

#define HEADER_HEIGHT ((uint32_t)4)
#define HEADER_SEPARATOR "_"
#define FRAME_RATE ((uint32_t)25)

#define WIDTH (int32_t)(100)
#define HEIGHT (int32_t)(25)

#define BUFFER_WIDTH (int32_t)(WIDTH)
#define BUFFER_HEIGHT (int32_t)(HEIGHT - HEADER_HEIGHT)

#define GRAVITY 80

#define GROUND_LEVEL (BUFFER_HEIGHT - 2)

#define BASE_SCORE (4)
#define BASE_MULTIPLIER (0.01)

typedef struct {
    double score;
    double scoreMultiplier;
} UI;

typedef struct {
    Vect2 size;
    char** texture;
} Texture;

typedef struct oject_s{
    Vect2 position;
    Vect2 speed;
    Vect2 size;
    char** texture;
    bool jumpAvailable;
    uint32_t jumpSpeed;
} Object;

typedef struct {
    Vect2 size;
    uint64_t lastFrameTime;
    uint32_t fpsTarget;
    double deltaTime;
    bool debug;
    char drawBuffer[HEIGHT - HEADER_HEIGHT][WIDTH];

    UI ui;
    Object player;

    Input input;

// TODO - Seguramente hay que tener handlers
    size_t numEnemies;
    Object** enemies;
    uint64_t enemiesPeriod_us;
    uint64_t lastEnemyTime_us;

    size_t numDecorators;
    Object** decorators;
    uint64_t decoratorsPeriod_us;
    uint64_t lastDecoratorTime_us;

    bool created;
} Game;

void createGame(Game* game);
void deleteGame(Game* game);
void setFpsTarget(Game* game, uint32_t target);
bool update(Game* game);
double getDeltaTime(Game* game);
void enableDebug(Game* game);
void disableDebug(Game* game);

void setPlayerTexture(Game* game, char** texture, Vect2 size); // TODO
void addEnemy(Game* game, Object* object); // TODO
void addDecorator(Game* game, Object* object); // TODO
#endif