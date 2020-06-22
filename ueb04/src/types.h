#ifndef UEB02_TYPES_H
#define UEB02_TYPES_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "variables.h"

#define TAU (acos(-1.0) * 2.f)
#define TO_RADIANS(x) ((x) * TAU / 360.0f)

/** Art von Levelfeldern */
typedef enum {
    P_OUTER, P_FREE, P_WALL, P_BOX, P_TARGET, P_OBJECT_TRIANGLE,
    P_DOOR, P_DOOR_SWITCH, P_BOX_DOOR_SWITCH, P_PORTAL, P_HOUSE, P_PLAYER
} pushyFieldType;

/** Spielstatus  */
typedef enum {
    GAME_RUNNING,
    GAME_WON,
    GAME_PAUSED,
    GAME_HELP,
    GAME_LOST
} Gamestatus;

/** Mausereignisse. */
typedef enum {
    mouseButton,
    mouseMotion,
    mousePassiveMotion
} CGMouseEventType;

/** Kamerasicht (wird verwendet, um mit der Maus die Ansicht zu aendern) */
typedef struct {
    GLfloat radius;
    GLfloat angleVertical;
    GLfloat angleHorizontal;
} CameraView;

/** Spieleinstellungen */
typedef struct {
    GLboolean showFirstPerson;
    GLboolean showWorldLight;
    GLboolean showSpotLight;
    GLboolean showAnimation;
    GLboolean showNormals;
    GLboolean showWireframe;
    GLboolean showFullScreen;
} GameSettings;

/** Spiel */
typedef struct {
    Gamestatus gameStatus;
    GameSettings settings;
    CameraView camera;
} Game;


/** Wird verwendet, um u.g. Einstellungen zu togglen */
enum e_ToggleTypes {
    FULLSCREEN,
    NORMALS,
    FIRSTPERSON,
    WORLDLIGHT,
    SPOTLIGHT,
    ANIMATION,
    WIREFRAME
};

#endif //UEB01_TYPES_H