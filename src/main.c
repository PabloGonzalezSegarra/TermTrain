#include "core/core.h"
#include "core/game.h"
#include "core/math.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

/**
    1- Cada enemigo debe tener una altura ajusta a su tamaño ✅
    4- Calcular colisiones y listo ✅
    5- Refactor y clean up
    6- Documentar y readme
*/

/**
    1- EntityHandler necesita un método free! ✅
    2- Hay que mejorar el sistema de entities en general:
        a. Spawns en tiempo aleatorios ✅
        b. Ir reduciendo ligeramente el tiempo entre spawns ✅
        c. Permitir tener ajustes en los blueprints, como dificultad, volador etc ❌
        d. Los enemigos mueren demasiado pronto ✅
        e. Spawns en posición aleatorios ✅
    3- Hay que hacer un sistema que permita reiniciar el juego.
    4- Hay que revisar las colisiones ✅
    5- Hay que provar los decoradores ✅
    6- Hay un bug en el dibujado, que desplaza al player un pixel cuando tiene un enemigo detrás. ✅
    7. Dibujar el suelo
    8- Bug en el periodo de aparición
*/

void setUpPlayer(Game* game);
void setUpEnemies(Game* game);
void setUpDecorators(Game* game);
char** allocTexture(Vect2 size);

int main(int argc, char** argv){
    (void) argc;
    (void) argv;

    Game game;
    createGame(&game);
    setFpsTarget(&game, 5);

    setUpPlayer(&game);
    setUpEnemies(&game);
    setUpDecorators(&game);
    enableDebug(&game);

    while (update(&game)) {}

    deleteGame(&game);
    return 0;
}

void setUpPlayer(Game* game) {
    const Vect2 playerSize = (Vect2){5,3};
    char ** texture = allocTexture(playerSize);
    sprintf(texture[0], "H  : ");
    sprintf(texture[1], "C###>");
    sprintf(texture[2], " o o ");
    setPlayerTexture(game, texture, playerSize);
}

void setUpEnemies(Game* game) {
    // Esta forma de crear texturas.............!!##@1!!@#!#!@!!
    const Vect2 enemy1Size = (Vect2){5,3};
    char** enemy1Texture = allocTexture(enemy1Size);
    sprintf(enemy1Texture[0], "  ^  ");
    sprintf(enemy1Texture[1], " / \\ ");
    sprintf(enemy1Texture[2], "/   \\");
    addEnemy(game, enemy1Texture,enemy1Size);

    const Vect2 enemy2Size = (Vect2){5,4};
    char** enemy2Texture = allocTexture(enemy2Size);
    sprintf(enemy2Texture[0], "  ^  ");
    sprintf(enemy2Texture[1], " / \\ ");
    sprintf(enemy2Texture[2], "/   \\");
    sprintf(enemy2Texture[3], "|   |");
    addEnemy(game, enemy2Texture, enemy2Size);

    const Vect2 enemy3Size = (Vect2){6,3};
    char** enemy3Texture = allocTexture(enemy3Size);
    sprintf(enemy3Texture[0], " ____ ");
    sprintf(enemy3Texture[1], "/    \\");
    sprintf(enemy3Texture[2], "\\____/");
    addEnemy(game, enemy3Texture, enemy3Size);
}

void setUpDecorators(Game* game) {
    const Vect2 decorator1Size = (Vect2){12,4};
    char** decorator1Texture = allocTexture(decorator1Size);

    sprintf(decorator1Texture[0], "   __   _   ");
    sprintf(decorator1Texture[1], " _(  )_( )_ ");
    sprintf(decorator1Texture[2], "(_   _    _)");
    sprintf(decorator1Texture[3], "  (_) (__)  ");

    addDecorator(game, decorator1Texture,decorator1Size);
}


char** allocTexture(Vect2 size) {
    char ** text = (char**)malloc(sizeof(char*)*size.y);
    for (int32_t i = 0; i < size.y; i++) {
        text[i] = (char*)malloc(sizeof(char)*size.x);
    }

    return text;
}
