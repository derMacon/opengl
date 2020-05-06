
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
static CGPoint2f g_ballCenter = {BALL_INITIAL_X_POS, BALL_INITIAL_Y_POS};
static CGPoint2f g_extraCenter = {BALL_INITIAL_X_POS, 0.0f};

/** Geschwindigkeitsvektor des Balls. */
static CGVector2f g_quadSpeed = {BALL_STEPS_X, BALL_STEPS_Y};

float ball_speed = BALL_SPEED_INITIAL;
float extra_speed = EXTRA_SPEED;

static Player player = {INITIAL_LIVES, INITIAL_POINTS};
int extra_points = 0;

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

float randomXValue() {
    float min = -0.5f;
    float max = 0.5f;
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * (max - min);      /* [min, max] */
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
        extra_points = 0;
    } else {
        // Ball neu spawnen
        ball_speed = BALL_SPEED_INITIAL;
        g_ballCenter[0] = g_stickCenter[0];
        g_ballCenter[1] = BALL_INITIAL_Y_POS;

        g_quadSpeed[0] = randomXValue();
        g_quadSpeed[1] = 0.5f;
    }
}

void resetExtraPosition() {
    show_extra = GL_FALSE;
    g_extraCenter[1] = 0;
}

float calculateRadiant(float value) {

    float maxVal = stick_width / 2;

    // Maximumwerte beibehalten
    if (value < -maxVal) {
        value = -maxVal;
    } else if (value > maxVal) {
        value = maxVal;
    }

    float angle = (value * 45) / maxVal;

    // Inititaler Ballspeed ist 0.5, also um 45° korrigieren
    // x und y sind beide iniital 0.5 -> also 45°
    float correctedAngle = angle - 45;
    float radiant = correctedAngle * M_PI / 180;

    // Dreisatz 0.15 entsprechen 100% und 45°
    return radiant;
}

void rotate(float radiant) {

    float x = BALL_STEPS_X, y = BALL_STEPS_Y;
    float tempX = x, tempY = y;
    float cos = cosf(radiant);
    float sin = sinf(radiant);

    x = tempX * cos + tempY * sin;
    y = -tempX * sin + tempY * cos;

    g_quadSpeed[0] = x * ball_speed;
    g_quadSpeed[1] = y * ball_speed;
}

static CGSide checkBorderCollision(void) {
    CGSide res = sideNone;

    float collisionOffset = 0.005f;

    // Ball ist unter dem Stick, also verloren
    if (g_ballCenter[1] <= -1.0f) {
        resetExtraPosition();
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
             (g_ballCenter[0] >= (g_stickCenter[0] - stick_width / 2)) &&
             (g_ballCenter[0] <= (g_stickCenter[0] + stick_width / 2) &&
              g_ballCenter[1] + BALL_WIDTH / 2 < g_stickCenter[1] + BAR_THICKNESS)) {

        // An dieserm Punkt kollidiert der Ball auf der X-Achse mit dem Stick
        float collisionX = g_ballCenter[0] - g_stickCenter[0];

        // Neuen Winkel berechnen
        float radiant = calculateRadiant(collisionX);

        if (radiant == 0) {
            g_quadSpeed[1] *= -1;
        } else {
            rotate(radiant);
        }

        res = sideNone;
    }

    return res;
}

void addExtraPoints() {
    player.points += EXTRA_POINTS;
    extra_points += EXTRA_POINTS;
    printf("Extra: %d Punkte erhalten\n", EXTRA_POINTS);
}

void increaseStickWidth() {
    // Den Stick nicht zu breit machen :)
    float maxWidth = STICK_WIDTH * 3;

    if (stick_width < maxWidth) {
        stick_width *= EXTRA_WIDEN;
        printf("Extra: Stick wurde verbreitert!\n");
    } else {
        addExtraPoints();
    }
}

void decreaseBallSpeed() {
    printf("Extra: Ball wurde verlangsamt!\n");

    float minSpeed = BALL_SPEED_INITIAL;

    if (ball_speed > minSpeed) {
        g_quadSpeed[0] /= ball_speed;
        g_quadSpeed[1] /= ball_speed;

        ball_speed -= BALL_SPEED_MODIFIER;

        g_quadSpeed[0] *= ball_speed;
        g_quadSpeed[1] *= ball_speed;
    } else {
        addExtraPoints();
    }
}

void activateExtra() {

    int random = (rand() % 3) + 1;

    switch (random) {
        case 1:
            increaseStickWidth();
            break;

        case 2:
            decreaseBallSpeed();
            break;

        case 3:
            addExtraPoints();
            break;;
    }
}

void checkExtraCollision() {
    float extra_x = g_extraCenter[0];
    float extra_y = g_extraCenter[1];

    float stick_x = g_stickCenter[0];
    float stick_y = g_stickCenter[1];

    // Wenn Extra auf Schlaeger auftrifft
    if ((extra_x >= stick_x - stick_width / 2) &&
        (extra_x <= stick_x + stick_width / 2)
        && extra_y < stick_y + BAR_THICKNESS) {

        activateExtra();
        resetExtraPosition();
    }
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

void calcExtraPosition(double interval) {
    g_extraCenter[1] -= extra_speed * (float) interval;
    checkExtraCollision();

    if (g_extraCenter[1] <= -1.0f) {
        resetExtraPosition();
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
        float stickPosition = g_stickCenter[0] - (stick_width / 2);
        float leftBarPosition = -BAR_X_OFFSET + BAR_WIDTH / 2;

        if (stickPosition > leftBarPosition) {
            g_stickCenter[0] -= STEPS_PS * (float) interval;
        }
    }

    // Stick nach rechts bewegen
    if (g_movement[dirRight]) {
        float stickPosition = g_stickCenter[0] + (stick_width / 2);
        float rightBarPosition = BAR_X_OFFSET - BAR_WIDTH / 2;

        if (stickPosition < rightBarPosition) {
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

    int points = player.points - extra_points;

    // Alle 20 Punkte ein Leben hinzufuegen
    if (points % 20 == 0) {
        player.lives += 1;
        printf("Leben erhalten!\n");
    }

    // Alle 10 Punkte die Geschwindigkeit erhoehen
    if (points % 10 == 0) {
        // BAll speed entfernen für Inititalgeschwindigkeit
        // plus richtigem Winkel
        g_quadSpeed[0] /= ball_speed;
        g_quadSpeed[1] /= ball_speed;

        // Ballgeschwindigkeit erhoehen
        ball_speed += BALL_SPEED_MODIFIER;
        g_quadSpeed[0] *= ball_speed;
        g_quadSpeed[1] *= ball_speed;
        printf("Ballgeschwindigkeit erhoeht!\n");
    }

    // EXTRAS
    if (!show_extra && propabilityOccured()) {
        show_extra = GL_TRUE;
        g_extraCenter[0] = g_ballCenter[0];
        g_extraCenter[1] = g_ballCenter[1];
    }

    // Ausgabe
    printf("Punkte: %d | Verbleibende Leben: %d\n", player.points, player.lives);

    // Spieler hat gewonnen
    if (points >= NUMBER_OF_BLOCKS) {
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
 * Liefert aktuelle Postion (des Mittelpunktes) des Balls.
 * @return Postion (des Mittelpunktes) des Balls.
 */
CGPoint2f *
getBallCenter(void) {
    return &g_ballCenter;
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Extras.
 * @return Postion (des Mittelpunktes) des Extras.
 */
CGPoint2f *
getExtraCenter(void) {
    return &g_extraCenter;
}