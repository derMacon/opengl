
#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>
#include "variables.h"
#include "types.h"

Game game = {0, 0, 0, GAME_RUNNING};

// TODO: Aendern
Levels levels[3] = {{
                            {0, 2, {
                                           {P_OUTER, P_WALL, P_WALL, P_WALL, P_OUTER, P_WALL, P_WALL, P_WALL, P_OUTER},
                                           {P_WALL, P_WALL, P_START, P_WALL, P_WALL, P_WALL, P_TARGET, P_WALL, P_WALL},
                                           {P_WALL, P_FREE, P_FREE, P_FREE, P_BOX, P_FREE, P_DOOR, P_FREE, P_WALL},
                                           {P_WALL, P_FREE, P_FREE, P_FREE, P_BOX, P_FREE, P_DOOR, P_FREE, P_WALL},
                                           {P_WALL, P_WALL, P_FREE, P_FREE, P_BOX, P_FREE, P_FREE, P_WALL, P_WALL},
                                           {P_OUTER, P_WALL, P_WALL, P_FREE, P_BOX, P_FREE, P_WALL, P_WALL, P_OUTER},
                                           {P_OUTER, P_OUTER, P_WALL, P_WALL, P_BOX, P_WALL, P_WALL, P_OUTER, P_OUTER},
                                           {P_OUTER, P_OUTER, P_OUTER, P_WALL, P_WALL, P_WALL, P_OUTER, P_OUTER, P_OUTER},
                                           {P_OUTER, P_OUTER, P_OUTER, P_OUTER, P_WALL, P_OUTER, P_OUTER, P_OUTER, P_OUTER}}},

                            {1, 2, {
                                           {P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL},
                                           {P_WALL, P_FREE, P_START, P_FREE, P_WALL, P_FREE, P_FREE, P_FREE, P_WALL},
                                           {P_WALL, P_BOX, P_FREE, P_FREE, P_WALL, P_FREE, P_FREE, P_FREE, P_WALL},
                                           {P_WALL, P_FREE, P_BOX, P_FREE, P_WALL, P_FREE, P_FREE, P_FREE, P_WALL},
                                           {P_WALL, P_BOX, P_FREE, P_BOX, P_WALL, P_FREE, P_FREE, P_DOOR, P_WALL},
                                           {P_WALL, P_FREE, P_BOX, P_FREE, P_WALL, P_FREE, P_FREE, P_FREE, P_WALL},
                                           {P_WALL, P_BOX, P_FREE, P_BOX, P_FREE, P_PORTAL, P_FREE, P_FREE, P_WALL},
                                           {P_WALL, P_FREE, P_BOX, P_FREE, P_TARGET, P_FREE, P_PORTAL, P_FREE, P_WALL},
                                           {P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL}}},

                            {2, 2, {
                                           {P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL},
                                           {P_WALL, P_START, P_FREE, P_FREE, P_FREE, P_BOX, P_DOOR, P_BOX, P_WALL},
                                           {P_WALL, P_BOX, P_FREE, P_FREE, P_FREE, P_FREE, P_BOX, P_FREE, P_WALL},
                                           {P_WALL, P_FREE, P_BOX, P_FREE, P_FREE, P_BOX, P_FREE, P_FREE, P_WALL},
                                           {P_WALL, P_BOX, P_FREE, P_BOX, P_FREE, P_BOX, P_PORTAL, P_PORTAL, P_WALL},
                                           {P_WALL, P_FREE, P_BOX, P_FREE, P_BOX, P_BOX, P_FREE, P_FREE, P_WALL},
                                           {P_WALL, P_BOX, P_FREE, P_BOX, P_FREE, P_BOX, P_FREE, P_PORTAL, P_WALL},
                                           {P_WALL, P_FREE, P_BOX, P_FREE, P_BOX, P_BOX, P_PORTAL, P_TARGET, P_WALL},
                                           {P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL, P_WALL}}}}};


/**
 * Setzt den Bewegunsstatus des Sticks.
 * @param direction Bewegungsrichtung deren Status veraendert werden soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Rechteck bewegt sich in Richtung
 * direction, GL_FALSE->Sticks bewegt sich nicht in Richtung direction.
 */
void
setPlayerMovement() {
}

void generateLevel(int levelId) {
    game.gameStatus = GAME_RUNNING;
    game.levelId = levelId;
    game.playerPosX = 0;
    game.playerPosY = 0;
}

Game *getGame(void) {
    return &game;
}

Levels (*getLevels(void)) {
    return (Levels *) &levels;
}

#endif