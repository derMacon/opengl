#ifndef UEB01_VARIABLES_H
#define UEB01_VARIABLES_H
#include "logic.h"

/** Globale Einstellungen **/
#define ZERO 0.0f
#define TIMER_CALLS_PS 144

/** Bars **/
#define BAR_X_OFFSET 0.9f
#define BAR_THICKNESS 0.07f
#define BAR_HEIGHT 1.7f
#define BAR_WIDTH 0.1f

/** Ball **/
#define INITIAL_BALL_SPEED 1.0f
#define INITIAL_BALL_X_POS 0.0f
#define INITIAL_BALL_Y_POST -0.8f
#define BALL_WIDTH 0.02f
#define X_STEPS_PS 0.5f // X-Geschwindigkeit
#define Y_STEPS_PS 0.5f // Y-Geschwindigkeit

/** Bloecke **/
#define NUMBER_OF_BLOCKS 56
#define BLOCK_WIDTH  0.15f
#define BLOCK_HEIGHT 0.075f

/** Sticks / Paddle **/
#define STICK_WIDTH 0.3f
#define STEPS_PS 1.0f

/** Extras **/
#define EXTRA_PROPABILITY 10 // Wahrscheinlichkeit, dass ein Extra spawnt
#define EXTRA_WIDTH 0.1f
#define EXTRA_HEIGHT 0.1f

/** Spielers **/
#define INITIAL_LIVES 3
#define INITIAL_POINTS 0

#endif
