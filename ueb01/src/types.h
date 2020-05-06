//
// Created by mike on 29.04.20.
//

#ifndef UEB01_TYPES_H
#define UEB01_TYPES_H

#include <GL/gl.h>

/** Punkt im 2D-Raum */
typedef GLfloat CGPoint2f[2];

/** Datentyp fuer Bewegungsrichtungen. */
typedef enum e_Direction CGDirection;

/** Bewegungsrichtungen. */
enum e_Direction {
    dirLeft, dirRight
};

/** RGB-Farbwert */
typedef GLfloat rgbColor[3];

struct struct_Block {
    CGPoint2f position;
    rgbColor color;
    int hidden;
};

typedef struct struct_Block Block;


/** Vektor im 2D-Raum */
typedef GLfloat CGVector2f[2];

typedef struct Player {
    int lives;
    int points;
} Player;

GLboolean gamePaused;
GLboolean showWireframe;
GLboolean showExtra;
float stickWidth;

/** Kollisionsseiten. */
enum collisionSide {
    sideNone, sideLeft, sideRight, sideTop, sideBottom
};

/** Datentyp fuer Kollisionsseiten. */
typedef enum collisionSide CGSide;

#endif //UEB01_TYPES_H
