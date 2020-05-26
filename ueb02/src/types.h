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
    P_OUTER, P_FREE, P_WALL, P_BOX, P_TARGET, P_OBJECT_TRIANGLE,
    P_DOOR, P_DOOR_SWITCH, P_BOX_DOOR_SWITCH, P_PORTAL, P_HOUSE, P_PLAYER
} pushyFieldType;

/* Spielfeld */
typedef pushyFieldType pushyLevel[LEVEL_SIZE][LEVEL_SIZE];

/* Zeiger auf ein Spielfeld */
typedef pushyFieldType (*pushyLevelPointer)[LEVEL_SIZE];

typedef enum {
    GAME_RUNNING,
    GAME_WON,
    GAME_PAUSED,
    GAME_HELP,
    GAME_LOST
} Gamestatus;

/** Bewegungsrichtungen. */
enum e_Direction {
    dirLeft, dirRight, dirUp, dirDown, dirNone
};

typedef struct {
    int portal1PosX, portal1PosY, portal2PosX, portal2PosY;
    int playerPosX, playerPosY;
    int numberOfTriangles;
    int time;
    int numberOfDoors;
    pushyLevel level;
} LevelSettings;

typedef struct {
    int levelId;
    Gamestatus gameStatus;
    LevelSettings levelSettings;
} Game;

typedef struct {
    int time;
    int startPos[2];
    pushyLevel field;
    int doors[MAXIMUM_NUMBER_OF_DOORS][2];
    int doorSwitch[MAXIMUM_NUMBER_OF_DOORS][2];
} Level;

#endif //UEB01_TYPES_H