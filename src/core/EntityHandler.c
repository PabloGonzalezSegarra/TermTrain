#include "EntityHandler.h"
#include "core.h"
#include "game.h"
#include "math.h"
#include "time.h"
#include <stdio.h>
#include <stdlib.h>

void spawnEntity(EntityHandler* handler);

void createEntityHandler(EntityHandler* handler, Vect2 position, Vect2 speed, uint64_t period, uint64_t minPeriod, Vect2 posRand, float timeRand, float diffIncrease, float maxDiff) {

    handler->startPosition = position;
    handler->speed = speed;

    handler->numBlueprints = 0;
    handler->numActiveEntities = 0;

    handler->period_us = period;
    handler->minPeriod_us = minPeriod;
    handler->nextSpawn_us = getCurrentMicroseconds();

    handler->posRand = posRand;
    handler->timeRand = timeRand > 1.f ? 1.f : timeRand;
    handler->diffIncrease = diffIncrease;
    handler->currentDiff = 1.f;
    handler->maxDiff = maxDiff;
    handler->entityCounter = 0;

    for (uint32_t i = 0; i < MAX_BLUEPRINTS; i++) {
        handler->blueprints[i] = NULL;
    }

    for (uint32_t i = 0; i < MAX_ACTIVE_ENTITIES; i++) {
        handler->activeEntities[i] = NULL;
    }
}

void addBlueprint(EntityHandler* handler, char** texture, Vect2 size) {
    if (handler->numBlueprints >= MAX_BLUEPRINTS) {
        return;
    }

    Object* entity = malloc(sizeof(Object));

    entity->position = handler->startPosition;
    entity->texture = texture;
    entity->size = size;
    entity->speed = handler->speed;
    entity->jumpSpeed = 0;

    handler->blueprints[handler->numBlueprints++] = entity;
}

void spawnEntity(EntityHandler* handler) {
    if (handler->numBlueprints <= 0 || handler->numActiveEntities >= MAX_ACTIVE_ENTITIES) {
        return;
    }

    Object* blueprint = handler->blueprints[rand() % handler->numBlueprints];
    Object* entity = malloc(sizeof(Object));

    if (handler->posRand.x == 0) {
        entity->position.x = handler->startPosition.x;
    }else{
        const int sign = rand() % 2 == 0 ? 1 : -1;
        const int posDiff = rand() % (int)handler->posRand.x;
        entity->position.x = handler->startPosition.x + (sign * posDiff);
    }

    if (handler->posRand.y == 0) {
        entity->position.y = handler->startPosition.y - blueprint->size.y;
    }else{
        const int sign = rand() % 2 == 0 ? 1 : -1;
        const int posDiff = rand() % (int)handler->posRand.y;
        entity->position.y = handler->startPosition.y + (sign * posDiff) - blueprint->size.y;
    }

    entity->speed = (Vect2) {
        blueprint->speed.x * handler->currentDiff,
        blueprint->speed.y * handler->currentDiff
    };

    entity->size = blueprint->size;

    entity->texture = blueprint->texture;

    handler->activeEntities[handler->numActiveEntities] = entity;
    handler->numActiveEntities++;
    handler->entityCounter++;
}

void killEntity(EntityHandler* handler) {
    if (handler->numActiveEntities <= 0) {
        return;
    }

    free(handler->activeEntities[0]);

    for (uint32_t i = 0; i < MAX_ACTIVE_ENTITIES - 1; i++) {
        handler->activeEntities[i] = handler->activeEntities[i + 1];
    }
    handler->activeEntities[MAX_ACTIVE_ENTITIES - 1] = NULL;

    handler->numActiveEntities--;
}

void handleUpdate(EntityHandler *handler, double deltaTime ) {
    const uint64_t currentTime = getCurrentMicroseconds();
    if (currentTime >= handler->nextSpawn_us) {
        spawnEntity(handler);

        handler->nextSpawn_us = currentTime + handler->period_us;
        const int sign = rand() % 2 == 0 ? 1 : -1;
        const uint64_t randomOffset = ((long)rand() % handler->period_us) * handler->timeRand;
        handler->nextSpawn_us = handler->nextSpawn_us + (uint64_t)((float)sign * randomOffset);

        handler->period_us -= (uint64_t)((float)handler->period_us * (1.f - handler->diffIncrease)) * 0.01f;// - handler->period_us;
        if (handler->period_us <= handler->minPeriod_us) {
            handler->period_us = handler->minPeriod_us;
        }
    }

    for (uint32_t i = 0; i < handler->numActiveEntities; i++) {
        Object* entity = handler->activeEntities[i];

        entity->position.x -= entity->speed.x * deltaTime;
        entity->position.y -= entity->speed.y * deltaTime;
        if (entity->position.x + entity->size.x <= 0) {
            killEntity(handler);
        }
    }

    handler->currentDiff += handler->diffIncrease * deltaTime;
    if (handler->currentDiff > handler->maxDiff) {
        handler->currentDiff = handler->maxDiff;
    }
}

void drawEntities(EntityHandler* handler, char** buffer, Vect2 buffSize) {
    for (uint32_t i = 0; i < handler->numActiveEntities; i++) {
        drawObject(handler->activeEntities[i], buffer, buffSize);
    }
}

void deleteEntityHandler(EntityHandler *handler) {
    for (uint32_t i = 0; i < handler->numActiveEntities; i++) {
        free(handler->activeEntities[i]);
    }
    for (uint32_t i = 0; i < handler->numBlueprints; i++) {
        free(handler->blueprints[i]);
    }
}
