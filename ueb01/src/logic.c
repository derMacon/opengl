
#include <GL/gl.h>
#include "logic.h"
#include "scene.h"

/** der Mittelpunkt des Quadrats */
static CGPoint2f g_quadCenter = {0.0f, -BAR_X_OFFSET};

/**
 * Bewegungsstatus der Rechtecks. Fuer alle vier Richtungen wird angegeben, ob
 * sich das Rechteck in die jeweilige Richtung bewegt.
 */
static GLboolean g_movement[4] = {GL_FALSE, GL_FALSE};

/**
 * Berechnet neue Position des Rechtecks.
 * @param interval Dauer der Bewegung in Sekunden.
 */
void
calcPosition(double interval) {
    if (g_movement[dirLeft]) {
        g_quadCenter[0] -= STEPS_PS * (float) interval;
    }
    if (g_movement[dirRight]) {
        g_quadCenter[0] += STEPS_PS * (float) interval;
    }
}

/**
 * Setzt den Bewegunsstatus des Rechtecks.
 * @param direction Bewegungsrichtung deren Status veraendert werden soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Rechteck bewegt sich in Richtung
 * direction, GL_FALSE->Rechteck bewegt sich nicht in Richtung direction.
 */
void
setMovement(CGDirection direction, GLboolean status) {
    //  if (direction <= dirDown)
    // {
    g_movement[direction] = status;
    // }
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Rechtecks.
 * @return Postion (des Mittelpunktes) des Rechtecks.
 */
CGPoint2f *
getQuadCenter(void) {
    return &g_quadCenter;
}