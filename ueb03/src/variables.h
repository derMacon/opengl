#ifndef UEB02_VARIABLES_H
#define UEB02_VARIABLES_H

/* Globale Einstellungen */
#define TIMER_CALLS_PS 144
#define MOUSE_SCROLL_SPEED (0.6f)
#define SQUARE_REFINEMENT_COUNT 40

/* Kamera */
#define CAMERA_RADIUS_MIN ( 0.5f)
#define CAMERA_RADIUS_MAX (5.0f)
#define FACTOR_POLAR (-0.25f)
#define FACTOR_AZIMUTH (0.25f)
#define EMPTY_CAMERA_ORIENTATION {2.5f, 45.0f, -90}

/* Hoehe/Breite des (quadratischen) Levels */
#define LEVEL_SIZE (9)
#define BLOCK_WIDTH (2.f / LEVEL_SIZE)
#define BLOCK_OFFSET  0.005f
#define BLOCK_SIZE BLOCK_WIDTH - BLOCK_OFFSET

#define FUGUE_HEIGHT 0.25f
#define FUGUE_WIDTH 0.05f

/* BOXES */
#define BOX_NUMBER_OF_COLS 7
#define BOX_DASH_WIDTH 0.03f
#define BOX_DASH_HEIGHT 0.7f

#define MAXIMUM_NUMBER_OF_DOORS 3
#define NUMBER_OF_LEVEL 4

/* Cooldown der Spielerbewegung */
#define COOLDOWN_TIME 0.3
#endif //UEB02_VARIABLES_H
