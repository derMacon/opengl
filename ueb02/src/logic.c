

#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>
#include <stdio.h>

#endif

#include "variables.h"
#include "types.h"

Game game = {0, 1, 1, GAME_RUNNING};

// TODO: Aendern + P_START entfernen und dafuer Startkoordinaten einfuegen
Levels levels[3] = {{
                            {2, {
                                        {P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL},
                                        {P_WALL, P_START, P_FREE, P_FREE, P_FREE, P_FREE, P_DOOR, P_DOOR_SWITCH, P_WALL},
                                        {P_WALL, P_FREE, P_HOUSE, P_BOX, P_FREE, P_FREE, P_BOX, P_FREE, P_WALL},
                                        {P_WALL, P_FREE, P_BOX, P_WALL, P_FREE, P_BOX, P_FREE, P_FREE, P_WALL},
                                        {P_WALL, P_BOX, P_FREE, P_BOX, P_FREE, P_BOX, P_FREE, P_FREE, P_WALL},
                                        {P_WALL, P_FREE, P_BOX, P_FREE, P_BOX, P_FREE, P_FREE, P_FREE, P_WALL},
                                        {P_WALL, P_BOX, P_FREE, P_BOX, P_PORTAL, P_BOX, P_FREE, P_OBJECT_TRIANGLE, P_WALL},
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

GLboolean playerMovementAllowed(enum e_Direction direction) {
    int pX = game.playerPosX;
    int pY = game.playerPosY;

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
