
#ifndef __LOGIC_H__
#define __LOGIC_H__

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
CGPoint2f *getStickCenter (void);

CGPoint2f *getBallCenter (void);

void calcStickPosition (double interval);
void calcBallPosition (double interval);

void setStickMovement (CGDirection direction, GLboolean status);

#endif