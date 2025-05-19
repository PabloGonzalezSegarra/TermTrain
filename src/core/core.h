#include <stdbool.h>

#include "math.h"

#ifndef TERM_TRAIN_CORE_INCLUDE__
#define TERM_TRAIN_CORE_INCLUDE__

typedef struct {
    double score;
    double scoreMultiplier;
} UI;

typedef struct {
    Vect2 size;
    char** texture;
} Texture;

typedef struct {
    Vect2 position;
    Vect2 speed;
    Vect2 size;
    char** texture;
    bool jumpAvailable;
    uint32_t jumpSpeed;
} Object;

void drawObject(Object* object, char** buffer, Vect2 buffSize);
void drawBuffer(char** buffer, Vect2 bufferSize, char color[]);
bool pixelInBuffer(Vect2 pixelPos, Vect2 buffSize);
bool areColliding(Object* object, Object* other, float margin);
#endif // TERM_TRAIN_CORE_INCLUDE__