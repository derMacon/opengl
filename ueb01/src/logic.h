
#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "scene.h"
#include "types.h"

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Rechtecks.
 * @return Postion (des Mittelpunktes) des Rechtecks.
 */
CGPoint2f *getStickCenter (void);

CGPoint2f *getBallCenter (void);

int checkBlockCollision(Block *block);

void calcStickPosition (double interval);
void calcBallPosition (double interval);

void setStickMovement (CGDirection direction, GLboolean status);

#endif