

#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>
#include <stdio.h>

#endif

#include "variables.h"
#include "types.h"


Game game = {0, 1, 1, GAME_RUNNING};

// TODO: Aendern
Levels levels[3] = {{
                            {2, {
                                        {P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL},
                                        {P_WALL, P_START, P_FREE, P_BOX, P_OBJECT_TRIANGLE, P_FREE, P_DOOR, P_DOOR_SWITCH, P_WALL},
                                        {P_WALL, P_FREE, P_HOUSE, P_BOX, P_FREE, P_FREE, P_BOX, P_FREE, P_WALL},
                                        {P_WALL, P_FREE, P_BOX, P_FREE, P_FREE, P_BOX, P_FREE, P_FREE, P_WALL},
                                        {P_WALL, P_BOX, P_FREE, P_BOX, P_FREE, P_BOX, P_FREE, P_FREE, P_WALL},
                                        {P_WALL, P_FREE, P_BOX, P_FREE, P_BOX, P_FREE, P_FREE, P_FREE, P_WALL},
                                        {P_WALL, P_BOX, P_FREE, P_BOX, P_FREE, P_BOX, P_FREE, P_PORTAL, P_WALL},
                                        {P_WALL, P_PORTAL, P_BOX, P_FREE, P_BOX, P_BOX, P_FREE, P_TARGET, P_WALL},
                                        {P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL}}},

                            {2, {
                                        {P_OUTER, P_WALL, P_WALL, P_WALL, P_OUTER, P_WALL, P_WALL, P_WALL, P_OUTER},
                                        {P_WALL, P_WALL, P_START, P_WALL, P_WALL, P_WALL, P_TARGET, P_WALL, P_WALL},
                                        {P_WALL, P_FREE, P_FREE, P_FREE, P_FREE, P_FREE, P_FREE, P_FREE, P_WALL},
                                        {P_WALL, P_FREE, P_FREE, P_FREE, P_FREE, P_FREE, P_FREE, P_FREE, P_WALL},
                                        {P_WALL, P_WALL, P_FREE, P_FREE, P_FREE, P_FREE, P_FREE, P_WALL, P_WALL},
                                        {P_OUTER, P_WALL, P_WALL, P_FREE, P_FREE, P_FREE, P_WALL, P_WALL, P_OUTER},
                                        {P_OUTER, P_OUTER, P_WALL, P_WALL, P_FREE, P_WALL, P_WALL, P_OUTER, P_OUTER},
                                        {P_OUTER, P_OUTER, P_OUTER, P_WALL, P_WALL, P_WALL, P_OUTER, P_OUTER, P_OUTER},
                                        {P_OUTER, P_OUTER, P_OUTER, P_OUTER, P_WALL, P_OUTER, P_OUTER, P_OUTER, P_OUTER}}},

                            {2, {
                                        {P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL},
                                        {P_WALL, P_START, P_FREE, P_FREE, P_FREE, P_BOX, P_DOOR, P_BOX, P_WALL},
                                        {P_WALL, P_BOX, P_FREE, P_FREE, P_FREE, P_FREE, P_BOX, P_FREE, P_WALL},
                                        {P_WALL, P_FREE, P_BOX, P_FREE, P_FREE, P_BOX, P_FREE, P_FREE, P_WALL},
                                        {P_WALL, P_BOX, P_FREE, P_BOX, P_FREE, P_BOX, P_PORTAL, P_PORTAL, P_WALL},
                                        {P_WALL, P_FREE, P_BOX, P_FREE, P_BOX, P_BOX, P_FREE, P_FREE, P_WALL},
                                        {P_WALL, P_BOX, P_FREE, P_BOX, P_FREE, P_BOX, P_FREE, P_PORTAL, P_WALL},
                                        {P_WALL, P_FREE, P_BOX, P_FREE, P_BOX, P_BOX, P_PORTAL, P_TARGET, P_WALL},
                                        {P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL}}}}};


// TODO: Broken af
void moveBox(enum e_Direction direction, int y, int x) {

    switch (direction) {
        case dirUp:
            levels[game.levelId]->field[y][x] = P_FREE;
            levels[game.levelId]->field[y - 1][x] = P_BOX;
            break;
        case dirDown:
            // TODO: Moven geht, aber noch weitere abfragen
            levels[game.levelId]->field[y][x] = P_FREE;
            levels[game.levelId]->field[y + 1][x] = P_BOX;
            break;
        case dirLeft:
            break;
        case dirRight:
            break;
    }
}

GLboolean playerMovementAllowed(enum e_Direction direction) {

    int pX = game.playerPosX;
    int pY = game.playerPosY;

    int bottomTile;
    int topTile;

    switch (direction) {
        case dirUp:
            if (pY - 1 >= 0) {
                topTile = levels[game.levelId]->field[pY - 1][pX];
                if (topTile != P_WALL) {

                    if (topTile == P_BOX) {
                        moveBox(dirUp, pY - 1, pX);
                    }

                    return GL_TRUE;
                }
            }
            if (pY - 1 >= 0 && levels[game.levelId]->field[pX][pY - 1] != P_WALL) {
                return GL_TRUE;
            }
            break;
        case dirDown:
            if (pY + 1 <= LEVEL_SIZE - 1) {
                bottomTile = levels[game.levelId]->field[pY + 1][pX];
                if (bottomTile != P_WALL) {

                    if (bottomTile == P_BOX) {
                        moveBox(dirDown, pY + 1, pX);
                    }

                    return GL_TRUE;
                }
            }
            break;
        case dirLeft:
            if (pX - 1 >= 0 && levels[game.levelId]->field[pX - 1][pY] != P_WALL) {
                return GL_TRUE;
            }
            break;
        case dirRight:
            if (pX + 1 <= LEVEL_SIZE - 1 && levels[game.levelId]->field[pX + 1][pY] != P_WALL) {
                return GL_TRUE;
            }
            break;
    }


    return GL_FALSE;
}

/**
 * Setzt den Bewegunsstatus des Spielers.
 * @param direction Bewegungsrichtung deren Status veraendert werden soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Rechteck bewegt sich in Richtung
 * direction, GL_FALSE->Sticks bewegt sich nicht in Richtung direction.
 */
void
setPlayerMovement(enum e_Direction direction) {

    if (playerMovementAllowed(direction)) {
        if (direction == dirLeft) {
            game.playerPosX -= 1;
        } else if (direction == dirRight) {
            game.playerPosX += 1;
        } else if (direction == dirUp) {
            game.playerPosY -= 1;
        } else if (direction == dirDown) {
            game.playerPosY += 1;
        }
    }
}

void generateLevel(int levelId) {
    game.gameStatus = GAME_RUNNING;
    game.levelId = levelId;
    game.playerPosX = 1;
    game.playerPosY = 1;
}

Game *getGame(void) {
    return &game;
}

Levels (*getLevels(void)) {
    return (Levels *) &levels;
}
