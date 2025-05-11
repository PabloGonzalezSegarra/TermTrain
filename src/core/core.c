#include "core.h"
#include "math.h"

void drawObject(Object* object, char** buffer, Vect2 buffSize) {
    for (int i = 0; i < object->size.y; i++) {
        for (int j = 0; j < object->size.x; j++) {
            if (object->texture[i][j] == ' ') {
                continue;
            }

            Vect2 pixelPos = (Vect2) {
                object->position.x + j,
                object->position.y + i
            };
            if (pixelInBuffer(pixelPos, buffSize)) {
                buffer[(int)pixelPos.y][(int)pixelPos.x] = object->texture[i][j];
            }
        }
        (void)1;
    }
}

bool pixelInBuffer(Vect2 pixelPos, Vect2 buffSize) {
    return pixelPos.y >= 0 && pixelPos.y < buffSize.y && pixelPos.x >= 0 && pixelPos.x < buffSize.x;
}

bool areColliding(Object* object, Object* other) {
    return object->position.x >= other->position.x &&
        object->position.x <= other->position.x + other->size.x && object->position.y >= other->position.y && object->position.y <= other->position.y + object->size.y;
}
