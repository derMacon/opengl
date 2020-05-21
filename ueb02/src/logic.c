#ifdef __APPLE__

#include <GLUT/glut.h>

#else

#include <GL/glut.h>

#endif

#include "variables.h"
#include <stdio.h>
#include "types.h"
#include "drawObjects.h"
#include "levels.h"

Game game;

pushyFieldType getBlockOfPos(int x, int y) {
    return game.levelSettings.level[y][x];
}

/**
 * Neue Position fuer x oder y bestimmen
 * @param val - x oder y Wert
 * @param direction - Wenn richtung z.B nach oben ist, muss der Y Wert verringert werden
 * @param isX - true, wenn der X-Wert bearbeitet werden soll
 * @return
 */
int newPos(int val, enum e_Direction direction, GLboolean isX) {
    if ((direction == dirLeft && isX) || (direction == dirUp && !isX)) {
        val -= 1;
    } else if ((direction == dirRight && isX) || (direction == dirDown && !isX)) {
        val += 1;
    }

    return val;
}

void checkTriangles() {
    if (game.levelSettings.numberOfTriangles > 0) {
        game.levelSettings.numberOfTriangles--;
    }

    if (game.levelSettings.numberOfTriangles == 0) {
        // MAKE HAUS GRUEN
        changeColor(GL_TRUE);
    }
}

GLboolean checkForDoors(int x, int y, int newX, int newY, pushyFieldType currentType, pushyFieldType nextType,
                        pushyFieldType doorType, GLboolean withNewX) {
    int numberOfDoors = game.levelSettings.numberOfDoors;
    GLboolean hasMoved = GL_FALSE;

    int doorX = withNewX ? newX : x;
    int doorY = withNewX ? newY : y;
    Level level = levels[game.levelId - 1];

    for (int i = 0; i < numberOfDoors; ++i) {
        int levelDoorX = level.doors[i][0];
        int levelDoorY = level.doors[i][1];

        if (level.doorSwitch[i][0] == doorX && level.doorSwitch[i][1] == doorY) {
            game.levelSettings.level[y][x] = currentType;
            game.levelSettings.level[newY][newX] = nextType;
            game.levelSettings.level[levelDoorY][levelDoorX] = doorType;
            hasMoved = GL_TRUE;
        }

        if (currentType == P_DOOR_SWITCH && nextType == P_BOX_DOOR_SWITCH) {
            if (level.doorSwitch[i][0] == newX && level.doorSwitch[i][1] == newY) {
                game.levelSettings.level[levelDoorY][levelDoorX] = P_FREE;
                hasMoved = GL_TRUE;
            }
        }
    }

    return hasMoved;
}

int moveObject(enum e_Direction direction, int x, int y, pushyFieldType currentTileType) {
    int newX = newPos(x, direction, GL_TRUE);
    int newY = newPos(y, direction, GL_FALSE);
    GLboolean hasMoved = GL_FALSE;
    pushyFieldType targetTileType = getBlockOfPos(newX, newY);

    if (targetTileType == P_FREE && currentTileType != P_BOX_DOOR_SWITCH) {
        game.levelSettings.level[y][x] = P_FREE;
        game.levelSettings.level[newY][newX] = currentTileType;
        hasMoved = GL_TRUE;
    } else if (targetTileType == P_DOOR_SWITCH && currentTileType == P_BOX) {
        hasMoved = checkForDoors(x, y, newX, newY, P_FREE, P_BOX_DOOR_SWITCH, P_FREE, GL_TRUE);
    } else if (targetTileType == P_TARGET && currentTileType == P_OBJECT_TRIANGLE) {
        game.levelSettings.level[y][x] = P_FREE;
        checkTriangles();
        hasMoved = GL_TRUE;
    } else if (targetTileType == P_FREE && currentTileType == P_BOX_DOOR_SWITCH) {
        hasMoved = checkForDoors(x, y, newX, newY, P_BOX_DOOR_SWITCH, P_BOX, P_DOOR, GL_FALSE);
    } else if (targetTileType == P_DOOR_SWITCH && currentTileType == P_BOX_DOOR_SWITCH) {
        hasMoved = checkForDoors(x, y, newX, newY, P_DOOR_SWITCH, P_BOX_DOOR_SWITCH, P_DOOR_SWITCH, GL_FALSE);
    }

    return hasMoved;
}

void setPlayerPos(int x, int y) {
    game.levelSettings.playerPosX = x;
    game.levelSettings.playerPosY = y;
}

void teleportPlayer(int portalX, int portalY) {
    int lpX1 = game.levelSettings.portal1PosX;
    int lpY1 = game.levelSettings.portal1PosY;
    int lpX2 = game.levelSettings.portal2PosX;
    int lpY2 = game.levelSettings.portal2PosY;

    // Nur ausfuehren, wenn korrekte Anzahl an Portalen vorhanden
    // -1 wird gesetzt, wenn zuviele Portale vorhanden sind.
    if (lpX1 != -1) {
        if (portalX == lpX1 && portalY == lpY1) {
            setPlayerPos(lpX2, lpY2);
        } else if (portalX == lpX2 && portalY == lpY2) {
            setPlayerPos(lpX1, lpY1);
        }
    }
}

void decreaseTime() {
    if (game.levelSettings.time > 1) {
        game.levelSettings.time--;
    } else {
        game.gameStatus = GAME_LOST;
    }
}

GLboolean playerMovementAllowed(enum e_Direction direction) {
    int pX = game.levelSettings.playerPosX;
    int pY = game.levelSettings.playerPosY;

    int pXNew = newPos(pX, direction, GL_TRUE);
    int pYNew = newPos(pY, direction, GL_FALSE);
    int checkTile;

    if ((direction == dirUp && pY >= 1)
        || (direction == dirDown && pY < LEVEL_SIZE)
        || (direction == dirLeft && pX >= 1)
        || (direction == dirRight && pX < LEVEL_SIZE)) {

        checkTile = getBlockOfPos(pXNew, pYNew);

        switch (checkTile) {
            case P_BOX:
            case P_BOX_DOOR_SWITCH:
            case P_OBJECT_TRIANGLE:
                return moveObject(direction, pXNew, pYNew, checkTile);

            case P_HOUSE:
                if (game.levelSettings.numberOfTriangles == 0) {
                    game.gameStatus = GAME_WON;
                    return GL_TRUE;
                }
                break;

            case P_FREE:
            case P_DOOR_SWITCH:
                return GL_TRUE;

            case P_PORTAL:
                teleportPlayer(pXNew, pYNew);

            case P_WALL:
            case P_TARGET:
            default:
                return GL_FALSE;
        }
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
            game.levelSettings.playerPosX -= 1;
        } else if (direction == dirRight) {
            game.levelSettings.playerPosX += 1;
        } else if (direction == dirUp) {
            game.levelSettings.playerPosY -= 1;
        } else if (direction == dirDown) {
            game.levelSettings.playerPosY += 1;
        }
    }
}

void checkForInvalidPortals(int numberOfPortals) {
    if (numberOfPortals != 2) {
        game.levelSettings.portal1PosX = -1;
        game.levelSettings.portal1PosY = -1;
        game.levelSettings.portal2PosX = -1;
        game.levelSettings.portal2PosY = -1;
    }
}

void checkForInvalidDoor(int numberOfDoors) {
    if (numberOfDoors != 1) {
        game.levelSettings.doorPosX = -1;
        game.levelSettings.doorPosY = -1;
    }
    game.levelSettings.numberOfDoors = numberOfDoors;
}

void setObjectCoords() {

    int alreadyCountedPortals = 0;
    int numberOfTriangles = 0;
    int numberOfDoors = 0;

    for (int y = 0; y < LEVEL_SIZE; y++) {
        for (int x = 0; x < LEVEL_SIZE; x++) {

            // Dreiecke
            if (getBlockOfPos(x, y) == P_OBJECT_TRIANGLE) {
                numberOfTriangles++;
            }

            // Portale
            if (getBlockOfPos(x, y) == P_PORTAL) {

                if (alreadyCountedPortals < 1) {
                    game.levelSettings.portal1PosX = x;
                    game.levelSettings.portal1PosY = y;
                } else {
                    game.levelSettings.portal2PosX = x;
                    game.levelSettings.portal2PosY = y;
                }
                alreadyCountedPortals++;
            }

            // Tuer
            if (getBlockOfPos(x, y) == P_DOOR) {
                game.levelSettings.doorPosX = x;
                game.levelSettings.doorPosY = y;
                numberOfDoors++;
            }

        }
    }

    game.levelSettings.numberOfTriangles = numberOfTriangles;

    if (numberOfTriangles == 0) {
        checkTriangles();
    }

    checkForInvalidPortals(alreadyCountedPortals);
    checkForInvalidDoor(numberOfDoors);
}

void loadLevel(int levelId) {
    pushyFieldType (*tempLevel)[9] = NULL;
    tempLevel = levels[levelId - 1].field;

    for (int y = 0; y < LEVEL_SIZE; ++y) {
        for (int x = 0; x < LEVEL_SIZE; ++x) {
            game.levelSettings.level[y][x] = tempLevel[y][x];
        }
    }
}

void initLevel(int levelId) {
    game.gameStatus = GAME_RUNNING;
    game.levelId = levelId;
    game.levelSettings.time = levels[levelId - 1].time;
    game.levelSettings.playerPosX = levels[levelId - 1].startPos[0];
    game.levelSettings.playerPosY = levels[levelId - 1].startPos[1];
    loadLevel(levelId);
    changeColor(GL_FALSE);

    // Positionen der Portale setzen
    setObjectCoords();
}

Game *getGame(void) {
    return &game;
}
