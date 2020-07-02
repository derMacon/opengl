#ifndef __SCENE_H__
#define __SCENE_H__

#include "variables.h"

/**
 * Zeichnet die gesamte Szene
 */
void drawScene();

/**
 * Setzt das Materiallicht fuer die gezeichnetetn Objekte
 * @param r - rot
 * @param g - gruen
 * @param b - blau
 */
void setMaterialLightning(float r, float g, float b);

/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int initScene(void);

#endif