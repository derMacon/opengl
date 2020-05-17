

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

int moveBox(enum e_Direction direction, int x, int y) {
    int newX = x;
    int newY = y;

    //todo auslagern?
    switch (direction) {
        case dirLeft:
            newX = x - 1;
            break;
        case dirRight:
            newX = x + 1;
            break;
        case dirUp:
            newY = y - 1;
            break;
        case dirDown:
            newY = y + 1;
            break;
    }

    if (y > 0
        && (getBlockOfPos(newX, newY) == P_FREE
            || getBlockOfPos(newX, newY) == P_DOOR_SWITCH)) {
        //TODO P_BOX_DOOR_SWITCH einbauen
        levels[game.levelId]->field[y][x] = P_FREE;
        levels[game.levelId]->field[newY][newX] = P_BOX;
        return GL_TRUE;
    }
    return GL_FALSE;
}

GLboolean playerMovementAllowed(enum e_Direction direction) {

    int pX = game.playerPosX;
    int pY = game.playerPosY;
    int pXNew = pX;
    int pYNew = pY;

    //todo auslagern?
    switch (direction) {
        case dirLeft:
            pXNew = pX - 1;
            break;
        case dirRight:
            pXNew = pX + 1;
            break;
        case dirUp:
            pYNew = pY - 1;
            break;
        case dirDown:
            pYNew = pY + 1;
            break;
    }
    int checkTile;
    // TODO positions in Variable und zusammenfassen 4x das gleiche unnötig

    if(direction == dirUp && pY >= 1
    || direction == dirDown && pY < LEVEL_SIZE
    ||direction == dirLeft && pX >= 1
    ||direction == dirRight && pX < LEVEL_SIZE ){

        checkTile = getBlockOfPos(pXNew, pYNew);

        if (checkTile == P_BOX) {
            return moveBox(direction, pXNew, pYNew);
        } else if (checkTile == P_OBJECT_TRIANGLE) {

        } else if (checkTile == P_HOUSE) {

        } else if (checkTile == P_WALL) {
            return GL_FALSE;
        }
        return GL_TRUE;
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
