#include <stdbool.h>

#include "math.h"
#include "core.h"

#ifndef TERM_TRAIN_ENTITY_HANDLER_INCLUDE__
#define TERM_TRAIN_ENTITY_HANDLER_INCLUDE__

#define MAX_BLUEPRINTS 16
#define MAX_ACTIVE_ENTITIES 8

typedef struct {
    Vect2 startPosition;
    Vect2 speed;

    uint8_t numBlueprints;
    Object* blueprints[16];

    uint8_t numActiveEntities;
    Object* activeEntities[8];

    uint64_t period_us;
    uint64_t minPeriod_us;
    uint64_t nextSpawn_us;

    Vect2 posRand;
    float timeRand; // % respect period_us, from 0
    float diffIncrease;
    float currentDiff;
    float maxDiff;

    uint64_t entityCounter;

} EntityHandler;

void createEntityHandler(EntityHandler* handler, Vect2 position, Vect2 speed, uint64_t period, uint64_t minPeriod, Vect2 posRand, float timeRand, float diffIncrease, float maxDiff);
void addBlueprint(EntityHandler* handler, char** texture, Vect2 size);
void handleUpdate(EntityHandler* handler,  double deltaTime );
void drawEntities(EntityHandler* handler, char** buffer, Vect2 buffSize);
void deleteEntityHandler(EntityHandler* handler);

#endif // TERM_TRAIN_ENTITY_HANDLER_INCLUDE__
