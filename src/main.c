#include "core/game.h"
#include "core/math.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv){
    (void) argc;
    (void) argv;

    Game game;
    createGame(&game);
    setFpsTarget(&game, 5);

    const Vect2 playerSize = (Vect2){2, 2};
    char ** text = (char**)malloc(sizeof(char*)*playerSize.y);
    for (int32_t i = 0; i < playerSize.y; i++) {
        text[i] = (char*)malloc(sizeof(char)*playerSize.x);
    }

    sprintf(text[0], "##");
    sprintf(text[1], "##");

    setPlayerTexture(&game, text, playerSize);

    enableDebug(&game);

    while (update(&game)) {}

    deleteGame(&game);
    return 0;
}
