
#include <GL/gl.h>
#include "logic.h"
#include "variables.h"

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
    // Stick nach links bewegen
    if (g_movement[dirLeft]) {
        // Position linker Balken + Breite des Sticks - Breite des linken Balkens
        // ist der Punkt, der den Stick stoppt
        float leftBarPosition = -BAR_X_OFFSET - BAR_WIDTH + STICK_WIDTH;

        if (g_quadCenter[0] > leftBarPosition) {
            g_quadCenter[0] -= STEPS_PS * (float) interval;
        }
    }

    // Stick nach rechts bewegen
    if (g_movement[dirRight]) {
        float rightBarPosition = BAR_X_OFFSET + BAR_WIDTH - STICK_WIDTH;
        if (g_quadCenter[0] < rightBarPosition) {
            g_quadCenter[0] += STEPS_PS * (float) interval;
        }
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
    g_movement[direction] = status;
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Rechtecks.
 * @return Postion (des Mittelpunktes) des Rechtecks.
 */
CGPoint2f *
getQuadCenter(void) {
    return &g_quadCenter;
}