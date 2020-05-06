
#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "scene.h"
#include "types.h"

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Sticks.
 * @return Postion (des Mittelpunktes) des Sticks.
 */
CGPoint2f *getStickCenter(void);

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Balls.
 * @return Postion (des Mittelpunktes) des Balls.
 */
CGPoint2f *getBallCenter(void);

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Extras.
 * @return Postion (des Mittelpunktes) des Extras.
 */
CGPoint2f *getExtraCenter(void);

/**
 * Prueft, ob der uebergebene Block vom Ball getroffen wurde
 * @param block - dieser Block wird geprueft
 * @return true wenn collided, false wenn nicht
 */
GLboolean blockCollided(Block *block);

/**
 * Berechnet neue Position des Sticks, wenn das Spiel nicht pausiert wurde
 * @param interval Dauer der Bewegung in Sekunden.
 */
void calcStickPosition(double interval);

/**
 * Berechnet neue Position des Balls, wenn das Spiel nicht pausiert wurde.
 * @param interval Dauer der Bewegung in Sekunden.
 */
void calcBallPosition(double interval);

/**
 * Berechnet neue Position des Extras
 * @param interval Dauer der Bewegung in Sekunden.
 */
void calcExtraPosition(double interval);

/**
 * Generiert fuer den (neu) gespawnten Ball einen zufaelligen Winkel
 */
void setRandomBallAngle();

/**
 * Setzt den Bewegunsstatus des Sticks.
 * @param direction Bewegungsrichtung deren Status veraendert werden soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Rechteck bewegt sich in Richtung
 * direction, GL_FALSE->Sticks bewegt sich nicht in Richtung direction.
 */
void setStickMovement(CGDirection direction, GLboolean status);

#endif