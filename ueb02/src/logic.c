

#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>
#include <stdio.h>

#endif

#include "variables.h"
#include "types.h"

Game game;

// TODO: Aendern + P_START entfernen und dafuer Startkoordinaten einfuegen
Levels levels[3] = {{
                            {2, {
                                        {P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL},

                                        {P_WALL, P_START, P_WALL, P_FREE, P_FREE, P_WALL, P_FREE, P_TARGET, P_WALL},
                                        {P_WALL, P_FREE, P_WALL, P_FREE, P_DOOR_SWITCH, P_WALL, P_FREE, P_FREE, P_WALL},
                                        {P_WALL, P_BOX, P_WALL, P_FREE, P_BOX, P_DOOR, P_FREE, P_FREE, P_WALL},
                                        {P_WALL, P_FREE, P_FREE, P_FREE, P_FREE, P_WALL, P_FREE, P_FREE, P_WALL},
                                        {P_WALL, P_FREE, P_FREE, P_OBJECT_TRIANGLE, P_FREE, P_WALL, P_WALL, P_WALL, P_WALL},
                                        {P_WALL, P_FREE, P_FREE, P_FREE, P_WALL, P_FREE, P_FREE, P_FREE, P_WALL},
                                        {P_WALL, P_BOX, P_FREE, P_PORTAL, P_WALL, P_PORTAL, P_FREE, P_HOUSE, P_WALL},

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

int moveObject(enum e_Direction direction, int x, int y, pushyFieldType fieldType) {
    int newX = newPos(x, direction, GL_TRUE);
    int newY = newPos(y, direction, GL_FALSE);
    pushyFieldType blockOfPos = getBlockOfPos(newX, newY);

    if (blockOfPos == P_FREE || (blockOfPos == P_DOOR_SWITCH && fieldType == P_BOX) ||
        (blockOfPos == P_TARGET && fieldType == P_OBJECT_TRIANGLE)) {
        //TODO P_BOX_DOOR_SWITCH einbauen

        if (fieldType != P_OBJECT_TRIANGLE || blockOfPos != P_TARGET) {
            levels[game.levelId]->field[newY][newX] = fieldType;
        }
        levels[game.levelId]->field[y][x] = P_FREE;
        return GL_TRUE;
    }
    return GL_FALSE;
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

    if (portalX == lpX1 && portalY == lpY1) {
        setPlayerPos(lpX2, lpY2);
    } else if (portalX == lpX2 && portalY == lpY2) {
        setPlayerPos(lpX1, lpY1);
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
            case P_OBJECT_TRIANGLE:
                return moveObject(direction, pXNew, pYNew, checkTile);

            case P_HOUSE:
                break;

            case P_FREE:
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

void setObjectCoords(GLboolean setPortals) {

    GLboolean foundPortal = GL_FALSE;

    for (int y = 0; y < LEVEL_SIZE; y++) {
        for (int x = 0; x < LEVEL_SIZE; x++) {

            if (setPortals && getBlockOfPos(x, y) == P_PORTAL) {

                if (!foundPortal) {
                    foundPortal = GL_TRUE;
                    game.levelSettings.portal1PosX = x;
                    game.levelSettings.portal1PosY = y;
                } else {
                    game.levelSettings.portal2PosX = x;
                    game.levelSettings.portal2PosY = y;
                    return;
                }
            } else if (!setPortals && getBlockOfPos(x, y) == P_DOOR) {
                game.levelSettings.doorPosX = x;
                game.levelSettings.doorPosY = y;
                return;
            }

        }
    }

    if (setPortals) {
        // Keine Portale vorhanden
        game.levelSettings.portal1PosX = -1;
        game.levelSettings.portal1PosY = -1;
        game.levelSettings.portal2PosX = -1;
        game.levelSettings.portal2PosY = -1;
    } else {
        game.levelSettings.doorPosX = -1;
        game.levelSettings.doorPosY = -1;
    }
}

void initLevel(int levelId) {
    game.gameStatus = GAME_RUNNING;
    game.levelId = levelId;
    game.levelSettings.playerPosX = 1;
    game.levelSettings.playerPosY = 1;

    // Positionen der Portale setzen
    setObjectCoords(GL_TRUE);

    // Postion der Tuer setzen
    setObjectCoords(GL_FALSE);
}

Game *getGame(void) {
    return &game;
}

Levels (*getLevels(void)) {
    return (Levels *) &levels;
}
