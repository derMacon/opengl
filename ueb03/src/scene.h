#ifndef __SCENE_H__
#define __SCENE_H__

#include "variables.h"

/**
 * Zeichnet die gesamte Szene
 */
void drawScene(GLboolean draw3D);


/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int initScene(void);

/**
 * Schaltet den Wireframe-Modus an oder aus
 */
void toggleWireframe();


/**
 * Schaltet den Fullscreen-Modus an oder aus
 */
void toggleFullscreen();

void toggleWorldLight();


#endif