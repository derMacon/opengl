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
#define BALL_SPEED_INITIAL 1.0f
#define BALL_INITIAL_X_POS 0.0f
#define BALL_INITIAL_Y_POS -0.8f
#define BALL_WIDTH 0.02f
#define BALL_SPEED_MODIFIER 0.3f
#define BALL_STEPS_X 0.5f // X-Geschwindigkeit
#define BALL_STEPS_Y 0.5f // Y-Geschwindigkeit

/** Bloecke **/
#define NUMBER_OF_BLOCKS 56
#define BLOCK_WIDTH  0.15f
#define BLOCK_HEIGHT 0.075f

/** Sticks / Paddle **/
#define STICK_WIDTH 0.3f
#define STEPS_PS 1.0f

/** Extras **/
#define EXTRA_SPEED 0.7f
#define EXTRA_PROPABILITY 6 // Wahrscheinlichkeit, dass ein Extra spawnt
#define EXTRA_WIDTH 0.05f
#define EXTRA_HEIGHT 0.05f
#define EXTRA_WIDEN 1.2f
#define EXTRA_POINTS 5

/** Spielers **/
#define INITIAL_LIVES 3
#define INITIAL_POINTS 0

#endif
