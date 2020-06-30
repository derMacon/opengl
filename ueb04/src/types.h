#ifndef UEB04_TYPES_H
#define UEB04_TYPES_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>

#endif

#include "variables.h"

#define TAU (acos(-1.0) * 2.f)
#define TO_RADIANS(x) ((x) * TAU / 360.0f)

/** Spielstatus  */
typedef enum {
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_HELP,
} Status;

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
    GLboolean showWorldLight;
    GLboolean showSpotLight;
    GLboolean showSpheres;
    GLboolean showNormals;
    GLboolean showTextures;
    GLboolean showWireframe;
    GLboolean showFullScreen;
} Settings;

typedef enum {
    X, Y, Z, R, G, B, NX, NY, NZ, TX, TY
} vertexIndices;

typedef GLdouble Vertex[11];

typedef struct {
    Vertex *vertices;
    GLuint *indices;
    GLuint length;
    double *velocities;
} Grid;

typedef struct {
    float posX;
    float posY;
    GLboolean xIncreasing;
    GLboolean yIncreasing;
} Spotlight;

/** Spiel */
typedef struct {
    Status gameStatus;
    Settings settings;
    Grid grid;
    CameraView camera;
    Spotlight spotlight;
} State;

/** Wird verwendet, um u.g. Einstellungen zu togglen */
enum e_ToggleTypes {
    FULLSCREEN,
    NORMALS,
    WORLDLIGHT,
    TEXTURES,
    SPOTLIGHT,
    SPHERES,
    WIREFRAME
};


#endif //UEB04_TYPES_H
