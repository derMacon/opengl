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

/** Art von Levelfeldern */
typedef enum {
    P_OUTER, P_FREE, P_WALL, P_BOX, P_TARGET, P_OBJECT_TRIANGLE,
    P_DOOR, P_DOOR_SWITCH, P_BOX_DOOR_SWITCH, P_PORTAL, P_HOUSE, P_PLAYER
} pushyFieldType;

/** Spielfeld */
typedef pushyFieldType pushyLevel[LEVEL_SIZE][LEVEL_SIZE];

/** Spielstatus  */
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

/** Leveleinstellungen des Spiels */
typedef struct {
    int
            portal1PosX, portal1PosY, portal2PosX, portal2PosY,
            playerPosX, playerPosY,
            numberOfTriangles,
            numberOfDoors,
            time;
    pushyLevel
            level;
} LevelSettings;


// TODO: Neuschreiben
#define TAU (acos(-1.0) * 2.f)
#define TO_RADIANS(x) ((x) * TAU / 360.0f)

// TODO: Neuschreiben
typedef struct {
    GLfloat radius;
    GLfloat azimuthAngle;
    GLfloat polarAngle;
} CameraOrientation;

/** Spieleinstellungen */
typedef struct {
    int levelId;
    Gamestatus gameStatus;
    LevelSettings levelSettings;
    GLboolean firstPerson;
    GLboolean showWorldLight;
    GLboolean showAnimation;
    enum e_Direction lastDirection;
    float movementCooldown;

    // TODO: Umbenennen
    CameraOrientation camera;
} Game;

/** Level des Spiels */
typedef struct {
    int
            time,
            startPos[2],
            doors[MAXIMUM_NUMBER_OF_DOORS][2],
            doorSwitch[MAXIMUM_NUMBER_OF_DOORS][2];
    pushyLevel
            field;
} Level;

#endif //UEB01_TYPES_H