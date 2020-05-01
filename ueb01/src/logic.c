
#include <GL/gl.h>
#include "logic.h"
#include "variables.h"
#include "math.h"
#include "types.h"
#include <stdio.h>
#include <GL/glut.h>
#include <stdarg.h>

/** der Mittelpunkt des Sticks */
static CGPoint2f g_stickCenter = {0.0f, -BAR_X_OFFSET};

/** der Mittelpunkt des Balls */
static CGPoint2f g_ballCenter = {INITIAL_BALL_X_POS, INITIAL_BALL_Y_POST};

/** Geschwindigkeitsvektor des Balls. */
static CGVector2f g_quadSpeed = {X_STEPS_PS, Y_STEPS_PS};

float ball_speed = INITIAL_BALL_SPEED;

static Player player = {INITIAL_LIVES, INITIAL_POINTS};

/**
 * Bewegungsstatus der Sticks. Fuer alle zwei Richtungen wird angegeben, ob
 * sich der Stick in die jeweilige Richtung bewegt.
 */
static GLboolean g_movement[2] = {GL_FALSE, GL_FALSE};

/**
 * Errechnet anhand eines Wahrscheinlichkeitwertes, ob etwas zutrifft oder nicht
 * @param propability int Wert von 0 - 100
 */
GLboolean propabilityOccured() {

    int highestNumber = 100 + 1;
    int calculatedNumber = (rand() % ((highestNumber - 1) + 1));

    return calculatedNumber < EXTRA_PROPABILITY;
}

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

float calculateAngle(float value) {

    float maxVal = 0.15f;

    // Maximumwerte beibehalten
    if (value < -maxVal) {
        value = -maxVal;
    } else if (value > maxVal) {
        value = maxVal;
    }

    // Dreisatz 0.15 entsprechen 100% und 45°
    return (value * 45) / maxVal;
}

/**
 * Spieler hat verloren!
 * Ausgabe von Punkten, Spiel pausuerne
 */
void handleLoss() {

    player.lives -= 1;
    printf("Sie haben ein Leben verloren!\nVerbleibende Leben: %d\n", player.lives);

    if (player.lives == 0) {
        printf("Sie haben verloren und %d Punkte erreicht!\n", player.points);
        fflush(stdout);
        game_paused = GL_TRUE;
    } else {
        // Ball neu spawnen
        g_ballCenter[0] = g_stickCenter[0];
        g_ballCenter[1] = INITIAL_BALL_Y_POST;

        // TODO: Winkel random berechnen zwischen -45° und 45°
        g_quadSpeed[0] = 0.5f;
        g_quadSpeed[1] = 0.5f;
    }

}

static CGSide checkBorderCollision(void) {
    CGSide res = sideNone;

    float collisionOffset = 0.005f;

    // Ball ist unter dem Stick, also verloren
    if (g_ballCenter[1] <= -1.0f) {
        show_extra = GL_FALSE;
        handleLoss();
    }

    if (g_quadSpeed[0] > 0.0f &&
        g_ballCenter[0] + (BALL_WIDTH / 2) + collisionOffset >= BAR_X_OFFSET - BAR_THICKNESS) {
        // Ball fliegt nach rechts und
        // die rechte Seite des Ball ueberschreitet den rechten Rand
        res = sideRight;
    }

        // Quadrat fliegt nach links und
        //  die linke Seite des Quadrats ueberschreitet den linken Rand
    else if (g_quadSpeed[0] < 0.0f &&
             g_ballCenter[0] - (BALL_WIDTH / 2) - collisionOffset <= -BAR_X_OFFSET + BAR_THICKNESS) {
        res = sideLeft;
    }

        // Quadrat fliegt nach oben und
        // die obere Seite des Quadrats ueberschreitet den oberen Rand
    else if (g_quadSpeed[1] > 0.0f &&
             // zusaetzlich Bar-width abziehen, weil ueber die Top-Bar noch der Text steht
             g_ballCenter[1] + BALL_WIDTH / 2 + collisionOffset >= BAR_X_OFFSET - BAR_WIDTH - BAR_THICKNESS) {
        res = sideTop;
    }

        // Ball fliegt nach unten und
        //  die untere Seite des Balls ueberschreitet den unteren Rand
    else if (g_quadSpeed[1] < 0.0f &&
             (g_ballCenter[0] >= (g_stickCenter[0] - STICK_WIDTH / 2)) &&
             (g_ballCenter[0] <= (g_stickCenter[0] + STICK_WIDTH / 2) &&
              g_ballCenter[1] < g_stickCenter[1] + BAR_THICKNESS + (2 * collisionOffset))) {


        // An dieserm Punkt kollidiert der Ball auf der X-Achse mit dem Stick
        float collisionX = g_ballCenter[0] - g_stickCenter[0];

        // Neuen Winkel berechnen
        float newAngle = calculateAngle(collisionX);

        /*  printf("BallCenterX Alt: %f\n", g_ballCenter[0]);
          printf("Collisionx: %f\n", collisionX * 100);
          printf("New angle %f\n", newAngle); */

        if (newAngle == 0) {
            g_quadSpeed[1] *= -1;
        } else {
            g_quadSpeed[0] = ball_speed * cosf(newAngle);

            // y
            g_quadSpeed[1] = ball_speed * sinf(newAngle);
/*
            printf("Speed X: %f\n", ball_speed * cosf(newAngle));
            printf("Speed Y: %f\n", -ball_speed * -sinf(newAngle));*/
        }

        res = sideNone;
    }

    return res;
}

void calcBallPosition(double interval) {

    if (!game_paused) {
        CGSide side = checkBorderCollision();

        if (side != sideNone) {
            handleBorderCollision(side);
        } else {
            g_ballCenter[0] += g_quadSpeed[0] * (float) interval;
            g_ballCenter[1] += g_quadSpeed[1] * (float) interval;
        }
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
 * Wenn die Bloecke getroffen wurden, Punkte hinzufuegen
 * und ggf. Leben und Geschwindigkeit erhoehen
 */
void handleHits() {
    player.points += 1;

    // Alle 20 Punkte ein Leben hinzufuegen
    if (player.points % 20 == 0) {
        player.lives += 1;
    }

    // Alle 10 Punkte die Geschwindigkeit erhoehen
    if (player.points % 10 == 0) {
        // TODO: Nach korrekter Winkelberechnung besseren Wert finden fuer ballspeed
        ball_speed += 0.1f;
    }

    // EXTRAS
    if (propabilityOccured()){
        printf("EXTRA\n");
        show_extra = GL_TRUE;
    }

    // Ausgabe
    printf("Punkte: %d | Verbleibende Leben: %d\n", player.points, player.lives);

    // Spieler hat gewonnen
    if (player.points >= NUMBER_OF_BLOCKS) {
        // Gewonnen
        printf("Sie haben gewonnen!");

        // Sofortige Ausgabe
        fflush(stdout);

        game_paused = GL_TRUE;
    }
}

int checkBlockCollision(Block *block) {

    // Blockposition
    GLfloat blockX = block->position[0];
    GLfloat blockY = block->position[1];

    // Ballposition
    GLfloat ballX = g_ballCenter[0];
    GLfloat ballY = g_ballCenter[1];

    // Trefferpunkte
    GLfloat blockRight = (blockX + BLOCK_WIDTH / 2) + BALL_WIDTH;
    GLfloat blockLeft = (blockX - BLOCK_WIDTH / 2) - BALL_WIDTH;

    GLfloat blockTop = (blockY + BLOCK_HEIGHT / 2) + BALL_WIDTH;
    GLfloat blockBottom = (blockY - BLOCK_HEIGHT / 2) - BALL_WIDTH;

    // Pruefen, ob getroffen wurde
    if ((ballY <= blockTop && ballY >= blockBottom)
        && (ballX >= blockLeft && ballX <= blockRight)) {
        // Ausblenden
        block->hidden = 1;

        // Punkte und Geschwindigkeiten ggf. erhoehen
        handleHits();

        // Werte, um zu pruefen, wo der Block genau getroffen wurde
        // links
        float xCheckLeft = fabsf(ballX - blockLeft);

        // rechts
        float xCheckRight = fabsf(ballX - blockRight);

        // unten
        float xCheckBottom = fabsf(ballY - blockBottom);

        // oben
        float xCheckTop = fabsf(ballY - blockTop);

        // Beim kleinsten Wert wurde der Block getroffen
        float val = xCheckLeft;
        CGSide side = sideLeft;

        // Rechts
        if (xCheckRight < val) {
            val = xCheckRight;
            side = sideRight;
        }

        // Unten
        if (xCheckBottom < val) {
            val = xCheckBottom;
            side = sideBottom;
        }

        // Oben
        if (xCheckTop < val) {
            side = sideTop;
        }

        // neuer Winkel fuer den Ball
        handleBorderCollision(side);

        // Kollision
        return 0;
    }

    return 1;
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