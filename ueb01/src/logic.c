
#include <GL/gl.h>
#include "logic.h"
#include "variables.h"
#include "math.h"

/** der Mittelpunkt des Sticks */
static CGPoint2f g_stickCenter = {0.0f, -BAR_X_OFFSET};

/** der Mittelpunkt des Sticks */
static CGPoint2f g_ballCenter = {0.0f, 0.0f};

/** Geschwindigkeitsvektor des Quadrats. */
static CGVector2f g_quadSpeed = {X_STEPS_PS, Y_STEPS_PS};

/**
 * Bewegungsstatus der Sticks. Fuer alle zwei Richtungen wird angegeben, ob
 * sich der Stick in die jeweilige Richtung bewegt.
 */
static GLboolean g_movement[4] = {GL_FALSE, GL_FALSE};

/**
 * Reagiert auf Kollisionen des Rechtecks mit dem Rahmen.
 * @param side Rahmenseite, mit der kollidiert wurde.
 */
static void
handleBorderCollision(CGSide side) {
    /* Bewegung in X-Richtung umkehren */
    if (side == sideLeft || side == sideRight) {
        g_quadSpeed[0] *= -1;
    }

    /* Bewegung in Y-Richtung umkehren */
    if (side == sideTop || side == sideBottom) {
        g_quadSpeed[1] *= -1;
    }
}

static CGSide checkBorderCollision(void) {
    CGSide
            res = sideNone;

    float collisionOffset = 0.005f;

    /* Ball fliegt nach rechts und
       die rechte Seite des Ball ueberschreitet den rechten Rand */
    if (g_quadSpeed[0] > 0.0f &&
        g_ballCenter[0] + (BALL_WIDTH / 2) + collisionOffset >= BAR_X_OFFSET - BAR_THICKNESS) {
        res = sideRight;
    }

        /* Quadrat fliegt nach links und
           die linke Seite des Quadrats ueberschreitet den linken Rand */
    else if (g_quadSpeed[0] < 0.0f &&
             g_ballCenter[0] - (BALL_WIDTH / 2) - collisionOffset <= -BAR_X_OFFSET + BAR_THICKNESS) {
        res = sideLeft;
    }

        /* Quadrat fliegt nach oben und
           die obere Seite des Quadrats ueberschreitet den oberen Rand */
    else if (g_quadSpeed[1] > 0.0f &&
             // zusaetzlich Bar-width abziehen, weil ueber die Top-Bar noch der Text steht
             g_ballCenter[1] + BALL_WIDTH / 2 + collisionOffset >= BAR_X_OFFSET - BAR_WIDTH - BAR_THICKNESS) {
        res = sideTop;
    }

        /* Quadrat fliegt nach unten und
           die untere Seite des Quadrats ueberschreitet den unteren Rand */
        // TODO: checken, wo der Ball auftrifft (falls er auftrifft) und dann Richtung bestimmen
    else if (g_quadSpeed[1] < 0.0f &&
             (g_ballCenter[0] >= (g_stickCenter[0] - STICK_WIDTH / 2)) &&
             (g_ballCenter[0] <= (g_stickCenter[0] + STICK_WIDTH / 2) &&
              g_ballCenter[1] < g_stickCenter[1] + BAR_THICKNESS + (2 * collisionOffset))) {

        res = sideBottom;

        // An dieserm Punkt kollidiert der Ball auf der X-Achse mit dem Stick
        float collisionX = g_ballCenter[0] - g_stickCenter[0];

        // Maximumwerte beibehalten
        if (collisionX < -0.15f) {
            collisionX = -0.15f;
        } else if (collisionX > 0.15f) {
            collisionX = 0.15f;
        }



        // Freshe Formel, um die Ausgangswinkel zu berechnen
        // f(x) = -22,222 * x^2 + 1
        float angle = -22.222f * pow(collisionX, 2) + 1;

        // x + y Geschwindigkeit berechnen
        float xySpeed = g_quadSpeed[1] + g_quadSpeed[0];

        // TODO: Muss man nicht wieder was von der Geschwindigkeit wegnehmen?
        // Hier wird jetzt der Ball immer schneller / langsamer, da wird multiplizieren.
        // Aber es darf ja nur eine Seite schneller werden. Die Gesamtgeschwindigkeit darf nicht berührt werden
        // z.B Initialgeschwindigkeit 0.5 und 0.5 bei getroffen müsste z.B 0.7 und 0.3 werden

        // Werte setzen
        g_quadSpeed[0] = (1 - angle) * xySpeed;
        g_quadSpeed[1] = angle * xySpeed;
    }

    return res;
}

void calcBallPosition(double interval) {
    CGSide
            side = checkBorderCollision();

    if (side != 0) {
        handleBorderCollision(side);
    } else {
        g_ballCenter[0] += g_quadSpeed[0] * (float) interval;
        g_ballCenter[1] += g_quadSpeed[1] * (float) interval;
    }

}

/**
 * Berechnet neue Position des Sticks.
 * @param interval Dauer der Bewegung in Sekunden.
 */
void
calcStickPosition(double interval) {
    // Stick nach links bewegen
    if (g_movement[dirLeft]) {
        // Position linker Balken + Breite des Sticks - Breite des linken Balkens
        // ist der Punkt, der den Stick stoppt
        float leftBarPosition = -BAR_X_OFFSET - BAR_WIDTH + STICK_WIDTH;

        if (g_stickCenter[0] > leftBarPosition) {
            g_stickCenter[0] -= STEPS_PS * (float) interval;
        }
    }

    // Stick nach rechts bewegen
    if (g_movement[dirRight]) {
        float rightBarPosition = BAR_X_OFFSET + BAR_WIDTH - STICK_WIDTH;
        if (g_stickCenter[0] < rightBarPosition) {
            g_stickCenter[0] += STEPS_PS * (float) interval;
        }
    }
}

/**
 * Setzt den Bewegunsstatus des Sticks.
 * @param direction Bewegungsrichtung deren Status veraendert werden soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Rechteck bewegt sich in Richtung
 * direction, GL_FALSE->Sticks bewegt sich nicht in Richtung direction.
 */
void
setStickMovement(CGDirection direction, GLboolean status) {
    g_movement[direction] = status;
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Sticks.
 * @return Postion (des Mittelpunktes) des Sticks.
 */
CGPoint2f *
getStickCenter(void) {
    return &g_stickCenter;
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Sticks.
 * @return Postion (des Mittelpunktes) des Sticks.
 */
CGPoint2f *
getBallCenter(void) {
    return &g_ballCenter;
}