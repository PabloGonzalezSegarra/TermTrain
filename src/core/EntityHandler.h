#ifndef TERM_TRAIN_ENTITY_HANDLER_INCLUDE__
#define TERM_TRAIN_ENTITY_HANDLER_INCLUDE__


#include <stdbool.h>
#include "math.h"
#include "core.h"

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
    uint64_t lastSpawn_us;

} EntityHandler;

void createEntityHandler(EntityHandler* handler, Vect2 position, Vect2 speed, uint64_t period);
void addBlueprint(EntityHandler* handler, char** texture, Vect2 size);
void handleUpdate(EntityHandler* handler,  double deltaTime );
void drawEntities(EntityHandler* handler, char** buffer, Vect2 buffSize);
void deleteEntityHandler(EntityHandler* handler);
#endif // TERM_TRAIN_ENTITY_HANDLER_INCLUDE__