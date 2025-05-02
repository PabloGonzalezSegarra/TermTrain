#include "core/game.h"
#include "core/input.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv){
    (void) argc;
    (void) argv;

    Game game;
    createGame(&game);
    setFpsTarget(&game, 5);

    while (update(&game, true)) {}

    deleteGame(&game);
    return 0;
}
