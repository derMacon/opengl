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
typedef GLfloat rgb_color[3];

struct struct_Block {
    CGPoint2f position;
    rgb_color color;
    int hidden;
};

typedef struct struct_Block Block;


/** Vektor im 2D-Raum */
typedef GLfloat CGVector2f[2];

/* struct zur Verwaltung der Spielballdaten. */
struct struct_Ball {
    GLfloat speed;
    CGPoint2f position;
    GLfloat rotation;
};

/** Datentyp zur Verwaltung der Spielballdaten. */
typedef struct s_Ball Ball;

typedef struct Player {
    int lives;
    int points;
} Player;

GLboolean game_paused;

/** Kollisionsseiten. */
enum e_Side {
    sideNone, sideLeft, sideRight, sideTop, sideBottom
};

/** Datentyp fuer Kollisionsseiten. */
typedef enum e_Side CGSide;

#endif //UEB01_TYPES_H
