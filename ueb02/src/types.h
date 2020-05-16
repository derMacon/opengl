
#ifndef UEB02_TYPES_H
#define UEB02_TYPES_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "variables.h"

GLboolean showWireframe;
GLboolean showFullscreen;

/*
 * Art von Levelfeldern
 */
typedef enum {
    P_START, P_OUTER, P_FREE, P_WALL, P_BOX, P_TARGET, P_OBJECT_TRIANGLE,
    P_TARGET_OBJECT, P_DOOR, P_DOOR_SWITCH, P_BOX_DOOR_SWITCH, P_PORTAL, P_HOUSE, P_PLAYER
} pushyFieldType;

/* Spielfeld */
typedef pushyFieldType pushyLevel[LEVEL_SIZE][LEVEL_SIZE];

/* Zeiger auf ein Spielfeld */
typedef pushyFieldType (*pushyLevelPointer)[LEVEL_SIZE];

typedef enum {
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_WON,
    GAME_LOST
} Gamestatus;

/** Bewegungsrichtungen. */
enum e_Direction {
    dirLeft, dirRight, dirUp, dirDown
};

typedef struct {
    int levelId;
    int playerPosX, playerPosY;
    Gamestatus gameStatus;
} Game;

typedef struct {
    int time;
    pushyLevel field;
} Level;

typedef Level Levels[3];

#endif //UEB01_TYPES_H
