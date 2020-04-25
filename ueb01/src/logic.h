
#ifndef __LOGIC_H__
#define __LOGIC_H__

/** Geschwindigkeit (Schritte pro Sekunde) */
#define STEPS_PS 0.5f

/** Anzahl der Aufrufe der Timer-Funktion pro Sekunde */
#define TIMER_CALLS_PS 120

/** Punkt im 2D-Raum */
typedef GLfloat CGPoint2f[2];

/** Datentyp fuer Bewegungsrichtungen. */
typedef enum e_Direction CGDirection;

/** Bewegungsrichtungen. */
enum e_Direction {
    dirLeft, dirRight
};

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Rechtecks.
 * @return Postion (des Mittelpunktes) des Rechtecks.
 */
CGPoint2f *getQuadCenter (void);

void calcPosition (double interval);

void setMovement (CGDirection direction, GLboolean status);

#endif