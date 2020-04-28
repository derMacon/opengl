#ifndef __SCENE_H__
#define __SCENE_H__

#include "logic.h"
#include "variables.h"

void drawScene(void);

/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int initScene(void);

/** RGB-Farbwert */
typedef GLfloat rgb_color[3];

struct struct_Block {
    CGPoint2f position;
    rgb_color color;
    int hidden;
};

typedef struct struct_Block Block;

#endif