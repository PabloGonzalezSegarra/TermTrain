#include "EntityHandler.h"
#include "core.h"
#include "game.h"
#include "math.h"
#include "time.h"
#include <stdio.h>
#include <stdlib.h>

void spawnEntity(EntityHandler* handler);

void createEntityHandler(EntityHandler* handler, Vect2 position, Vect2 speed, uint64_t period) {

    handler->startPosition = position;
    handler->speed = speed;
    handler->period_us = period;

    handler->numBlueprints = 0;
    handler->numActiveEntities = 0;

    handler->lastSpawn_us = getCurrentMicroseconds();

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

    entity->position = blueprint->position;
    entity->speed = blueprint->speed;
    entity->size = blueprint->size;
    entity->position = handler->startPosition;
    entity->position.y -= blueprint->size.y;
    entity->texture = blueprint->texture;

    handler->activeEntities[handler->numActiveEntities] = entity;
    handler->numActiveEntities++;
}

void killEntity(EntityHandler* handler) {
    if (handler->numActiveEntities <= 0) {
        return;
    }

    free(handler->activeEntities[0]);

    for (uint32_t i = 0; i < MAX_ACTIVE_ENTITIES - 1; i++) {
        handler->activeEntities[i] = handler->activeEntities[i + 1];
    }

    handler->numActiveEntities--;
}

void handleUpdate(EntityHandler *handler, double deltaTime ) {
    if (getCurrentMicroseconds() - handler->lastSpawn_us >= handler->period_us) {
        spawnEntity(handler);
        handler->lastSpawn_us = getCurrentMicroseconds();
    }

    for (uint32_t i = 0; i < handler->numActiveEntities; i++) {
        Object* entity = handler->activeEntities[i];

        entity->position.x -= entity->speed.x * deltaTime;
        entity->position.y -= entity->speed.y * deltaTime;

        if (entity->position.x <= 0) {
            killEntity(handler);
        }
    }
}

void drawEntities(EntityHandler* handler, char** buffer, Vect2 buffSize) {

    for (uint32_t i = 0; i < handler->numActiveEntities; i++) {
        drawObject(handler->activeEntities[i], buffer, buffSize);
    }
}
