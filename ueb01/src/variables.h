#ifndef UEB01_VARIABLES_H
#define UEB01_VARIABLES_H

/** Groessen der Bars **/
#define BAR_X_OFFSET 0.9f
#define BAR_THICKNESS 0.07f
#define BAR_HEIGHT 1.7f
#define BAR_WIDTH 0.1f

/** Werte des Balles **/
#define INITIAL_BALL_SPEED 1.0f
#define INITIAL_BALL_X_POS 0.0f
#define INITIAL_BALL_Y_POST -0.8f
#define BALL_WIDTH 0.02f
#define X_STEPS_PS 0.2f // X-Geschwindigkeit
#define Y_STEPS_PS 0.3f // Y-Geschwindigkeit
#include "logic.h"

/** Werte der Bloecke **/
#define NUMBER_OF_BLOCKS 56
#define BLOCK_WIDTH  0.15f
#define BLOCK_HEIGHT 0.075f
#define EXTRA_PROPABILITY 100

/** Werte des Sticks **/
#define STICK_WIDTH 0.3f
#define STEPS_PS 1.0f

/** Werte des Spielers **/
#define INITIAL_LIVES 3
#define INITIAL_POINTS 0

/** Globale NULL **/
#define ZERO 0.0f

/** Anzahl der Aufrufe der Timer-Funktion pro Sekunde */
#define TIMER_CALLS_PS 144

#endif
