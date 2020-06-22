#ifndef __SCENE_H__
#define __SCENE_H__

#include "variables.h"

/**
 * Zeichnet die gesamte Szene
 */
void drawScene();

/**
 *  Initialisiert die Displaylisten
 */
void initDisplayList();

/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int initScene(void);

#endif