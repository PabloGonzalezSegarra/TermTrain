#include "core/core.h"
#include "core/game.h"
#include "core/math.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/**
    1- Cada enemigo debe tener una altura ajusta a su tamaño
    2- Altura configurable, alomejor setear si es volador o terrestre
    3- Mecanismo que acelere el periodo de aparición de enemigos
    4- Calcular colisiones y listo
    5- Refactor y clean up
    6- Documentar y readme
*/

void setUpPlayer(Game* game);
void setUpEnemies(Game* game);
char** allocTexture(Vect2 size);

int main(int argc, char** argv){
    (void) argc;
    (void) argv;

    Game game;
    createGame(&game);
    setFpsTarget(&game, 5);

    setUpPlayer(&game);
    setUpEnemies(&game);
    enableDebug(&game);

    while (update(&game)) {}

    deleteGame(&game);
    return 0;
}

void setUpPlayer(Game* game) {
    const Vect2 playerSize = (Vect2){5,3};
    char ** texture = allocTexture(playerSize);

    sprintf(texture[0], "U  $ ");
    sprintf(texture[1], "#####");
    sprintf(texture[2], " o o ");

    setPlayerTexture(game, texture, playerSize);

}

void setUpEnemies(Game* game) {
    const Vect2 enemy1Size = (Vect2){4,3};
    char** enemy1Texture = allocTexture(enemy1Size);

    sprintf(enemy1Texture[0], "  ^  ");
    sprintf(enemy1Texture[1], " / \\ ");
    sprintf(enemy1Texture[2], "/   \\ ");

    addEnemy(game, enemy1Texture,enemy1Size);
}

char** allocTexture(Vect2 size) {
    char ** text = (char**)malloc(sizeof(char*)*size.y);
    for (int32_t i = 0; i < size.y; i++) {
        text[i] = (char*)malloc(sizeof(char)*size.x);
    }

    return text;
}