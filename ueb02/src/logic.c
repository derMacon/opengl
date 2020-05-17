

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
                                        {P_WALL, P_START, P_BOX, P_BOX, P_OBJECT_TRIANGLE, P_FREE, P_DOOR, P_DOOR_SWITCH, P_WALL},
                                        {P_WALL, P_FREE, P_HOUSE, P_BOX, P_FREE, P_FREE, P_BOX, P_FREE, P_WALL},
                                        {P_WALL, P_FREE, P_BOX, P_WALL, P_FREE, P_BOX, P_FREE, P_FREE, P_WALL},
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


pushyFieldType getBlockOfPos(int x, int y) {
    return levels[game.levelId]->field[y][x];
}

// TODO: 4x das gleiche in 1 machen
int moveBox(enum e_Direction direction, int x, int y) {

    switch (direction) {
        case dirUp:
            if (y > 0
                && (getBlockOfPos(x, y - 1) == P_FREE
                    || getBlockOfPos(x, y - 1) == P_DOOR_SWITCH)) {
                //TODO P_BOX_DOOR_SWITCH einbauen
                levels[game.levelId]->field[y][x] = P_FREE;
                levels[game.levelId]->field[y - 1][x] = P_BOX;
                return GL_TRUE;
            }
            break;
        case dirDown:
            if (y < LEVEL_SIZE
                && (getBlockOfPos(x, y + 1) == P_FREE
                    || getBlockOfPos(x, y + 1) == P_DOOR_SWITCH)) {
                //TODO P_BOX_DOOR_SWITCH einbauen
                levels[game.levelId]->field[y][x] = P_FREE;
                levels[game.levelId]->field[y + 1][x] = P_BOX;
                return GL_TRUE;
            }
            break;
        case dirLeft:
            if (x > 0
                && (getBlockOfPos(x - 1, y) == P_FREE
                    || getBlockOfPos(x - 1, y ) == P_DOOR_SWITCH)) {
                //TODO P_BOX_DOOR_SWITCH einbauen
                levels[game.levelId]->field[y][x] = P_FREE;
                levels[game.levelId]->field[y][x - 1] = P_BOX;
                return GL_TRUE;
            }
            break;
        case dirRight:
            if (x < LEVEL_SIZE
                && (getBlockOfPos(x + 1, y) == P_FREE
                    || getBlockOfPos(x + 1, y) == P_DOOR_SWITCH)) {
                //TODO P_BOX_DOOR_SWITCH einbauen
                levels[game.levelId]->field[y][x] = P_FREE;
                levels[game.levelId]->field[y][x + 1] = P_BOX;
                return GL_TRUE;
            }
            break;
    }
    return GL_FALSE;
}

GLboolean playerMovementAllowed(enum e_Direction direction) {

    int pX = game.playerPosX;
    int pY = game.playerPosY;

    int bottomTile;
    int topTile;
    // TODO positions in Variable und zusammenfassen 4x das gleiche unnötig
    switch (direction) {
        case dirUp:
            if (pY >= 1) {
                topTile = getBlockOfPos(pX, pY - 1);

                if (topTile == P_BOX) {
                    return moveBox(direction, pX, pY - 1);
                } else if (topTile == P_OBJECT_TRIANGLE) {

                } else if (topTile == P_HOUSE) {

                } else if (topTile == P_WALL) {
                    return GL_FALSE;
                }
                return GL_TRUE;
            }

            break;
        case dirDown:
            if (pY < LEVEL_SIZE) {
                topTile = getBlockOfPos(pX, pY + 1);

                if (topTile == P_BOX) {
                    return moveBox(direction, pX, pY + 1);
                } else if (topTile == P_OBJECT_TRIANGLE) {

                } else if (topTile == P_HOUSE) {

                } else if (topTile == P_WALL) {
                    return GL_FALSE;
                }
                return GL_TRUE;
            }
            break;
        case dirLeft:
            if (pX >= 1) {
                topTile = getBlockOfPos(pX - 1, pY);

                if (topTile == P_BOX) {
                    return moveBox(direction, pX - 1, pY);
                } else if (topTile == P_OBJECT_TRIANGLE) {

                } else if (topTile == P_HOUSE) {

                } else if (topTile == P_WALL) {
                    return GL_FALSE;
                }
                return GL_TRUE;
            }
            break;
        case dirRight:
            if (pX < LEVEL_SIZE) {
                topTile = getBlockOfPos(pX + 1, pY);

                if (topTile == P_BOX) {
                    return moveBox(direction, pX + 1, pY);
                } else if (topTile == P_OBJECT_TRIANGLE) {

                } else if (topTile == P_HOUSE) {

                } else if (topTile == P_WALL) {
                    return GL_FALSE;
                }
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
