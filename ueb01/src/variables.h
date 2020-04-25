#ifndef UEB01_VARIABLES_H
#define UEB01_VARIABLES_H

/** Groessen der Bars **/
#define BAR_X_OFFSET 0.9f
#define BAR_THICKNESS 0.07f
#define BAR_HEIGHT 1.7f
#define BAR_WIDTH 0.1f

/** Werte des Balles **/
#define BALL_WIDTH 0.04f

/** Geschwindigkeit (Schritte in X-Richtung pro Sekunde) */
#define X_STEPS_PS     0.35f

/** Geschwindigkeit (Schritte in Y-Richtung pro Sekunde) */
#define Y_STEPS_PS     0.5f

/** Werte des Sticks **/
#define STICK_WIDTH 0.3f

/** Globale NULL **/
#define ZERO 0.0f

/** Geschwindigkeit (Schritte pro Sekunde) */
#define STEPS_PS 0.5f

/** Anzahl der Aufrufe der Timer-Funktion pro Sekunde */
#define TIMER_CALLS_PS 144


/** Vektor im 2D-Raum */
typedef GLfloat CGVector2f[2];

/* struct zur Verwaltung der Spielballdaten. */
struct struct_Ball {
    GLfloat speed;
    CGPoint2f position;
    GLfloat rotation;
};

/** Datentyp zur Verwaltung der Spielballdaten. */
typedef struct s_Ball Ball;

/** Kollisionsseiten. */
enum e_Side {
    sideNone, sideLeft, sideRight, sideTop, sideBottom
};

/** Datentyp fuer Kollisionsseiten. */
typedef enum e_Side CGSide;

#endif
