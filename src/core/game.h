#ifndef TERM_TRAIN_SCREEN_INCLUDE__
#define TERM_TRAIN_SCREEN_INCLUDE__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "math.h"
#include "input.h"

#define HEADER_SIZE ((uint32_t)4)
#define HEADER_SEPARATOR "_"
#define FRAME_RATE ((uint32_t)10)

#define WIDTH 100
#define HEIGTH 25

//typedef struct {
//    uint64_t points;
//} UI;

typedef struct{
    Vect2 positon;
    Vect2 size;
    char** texture;
} Object;

typedef struct {
    Vect2 size;
    uint64_t lastFrameTime;
    uint32_t fpsTarget;
    double deltaTime;

    Object* player;

    Input input;

// TODO - Seguramente hay que tener handlers
    size_t numeEnemies;
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
bool update(Game* game, bool debug);
double getDeltaTime(Game* game);


void setPlayer(Game* game, Object* object); // TODO
void addEnemy(Game* game, Object* object); // TODO
void addDecorator(Game* game, Object* object); // TODO
#endif