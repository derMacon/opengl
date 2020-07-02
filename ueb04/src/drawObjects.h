#ifndef drawObjects_h
#define drawObjects_h

#include <GL/glu.h>
#include <GL/gl.h>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#endif

/**
 * Zeichnet ein Boot
 * @param isFirst true, wenn es das erste sein soll
 */
void drawBoat(GLboolean isFirst);

/**
 * Zeichnet den Leuchtturm
 */
void drawLighthouse();

/**
 * Laesst die Insel zeichnen
 */
void drawIsland();

/**
 * Laesst alle Spheren zeichnen
 */
void drawSpheres();

/**
 * Faerbt die Kugeln / Wasser anhand der Hoehe
 * @param index - Richtige Kugel awehlen
 * @param height - Die neue Hoehe, die die Kugel hat
 */
void changeColors(int index, float height);
#endif