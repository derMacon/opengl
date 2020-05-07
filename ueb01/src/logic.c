
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "logic.h"
#include "variables.h"
#include "math.h"
#include "types.h"
#include "helper.h"
#include <stdio.h>

/* Initial-Werte setzen */
static CGPoint2f stickCenter = {0.0f, -BAR_X_OFFSET};
static CGPoint2f ballCenter = {BALL_INITIAL_X_POS, BALL_INITIAL_Y_POS};
static CGPoint2f extraCenter = {BALL_INITIAL_X_POS, 0.0f};
static CGVector2f ballSpeedVector; // Wird initial gesetzt durch setRandomBallAngle()

float ballSpeed = BALL_SPEED_INITIAL;
float extraSpeed = EXTRA_SPEED;
static Player player = {PLAYER_INITIAL_LIVES, PLAYER_INITIAL_POINTS};

int extraPoints = 0;

/**
 * Bewegungsstatus der Sticks. Fuer alle zwei Richtungen wird angegeben, ob
 * sich der Stick in die jeweilige Richtung bewegt.
 */
static GLboolean stickMovementDirection[2] = {GL_FALSE, GL_FALSE};

/**
 * Errechnet anhand eines Wahrscheinlichkeitwertes, ob etwas zutrifft oder nicht
 * @param propability int Wert von 0 - 100
 */
GLboolean propabilityOccured() {
    return genRandomNumber(100, 1) < EXTRA_PROPABILITY;
}

/**
 * Reagiert auf Kollisionen des Rechtecks mit dem Rahmen.
 * @param side Rahmenseite, mit der kollidiert wurde.
 */
static void
handleBorderCollision(CGSide side) {
    /* Bewegung in X-Richtung umkehren */
    if (side == sideLeft || side == sideRight) {
        ballSpeedVector[0] *= -1;
    }

    /* Bewegung in Y-Richtung umkehren */
    if (side == sideTop || side == sideBottom) {
        ballSpeedVector[1] *= -1;
    }
}

/**
 * Generiert fuer den (neu) gespawnten Ball einen zufaelligen Winkel
 */
void setRandomBallAngle() {
    ballSpeedVector[0] = randomBallXValue();
    ballSpeedVector[1] = 0.5f;
}

/**
 * Setzt die Y-Position des aktuellen Extras zurueck und blendet es aus
 * x-Position wird bei erneutem Spawn bei Kollision des Balls mit einem Blocks gesetzt
 */
void resetExtraPosition() {
    showExtra = GL_FALSE;
    extraCenter[1] = 0;
}

/**
 * Zieht dem Spieler ein Leben ab.
 * Bei keinen verbleibenden Leben ist das Spiel beendet.
 * Bei uebrigen Leben wird ein neuer Ball gespawnt
 * Gibt zudem Info ueber evtl. verbleibende Leben aus
 */
void handleLoss() {

    player.lives -= 1;
    printf("Sie haben ein Leben verloren!\nVerbleibende Leben: %d\n", player.lives);

    if (player.lives == 0) {
        printf("Sie haben verloren und %d Punkte erreicht!\n", player.points);
        fflush(stdout);
        gamePaused = GL_TRUE;
        extraPoints = 0;
    } else {
        // Ball neu spawnen
        ballSpeed = BALL_SPEED_INITIAL;
        ballCenter[0] = stickCenter[0];
        ballCenter[1] = BALL_INITIAL_Y_POS;

        setRandomBallAngle();
    }
}

/**
 * Wird fuer die Berechnung des Ausgangswinkels des Balls verwendet.
 * @param collisionX - An dieser X-Stelle kollidiert der Ball mit dem Stick
 * @return Winkelmaß
 */
float calculateRadiant(float collisionX) {

    float maxVal = stickWidth / 2;
    int maxAngle = 45;

    // Maximumwerte beibehalten
    if (collisionX < -maxVal) {
        collisionX = -maxVal;
    } else if (collisionX > maxVal) {
        collisionX = maxVal;
    }

    float angle = (collisionX * maxAngle) / maxVal;

    // Inititaler Ballspeed ist 0.5, also um 45° korrigieren
    // x und y sind beide iniital 0.5 -> also 45°
    float correctedAngle = angle - maxAngle;
    return correctedAngle * M_PI / 180;
}

/**
 * Rotiert den Ball in einen angegebenen Winkelmaß
 * @param radiant - Winkelmaß fuer die Rotation
 */
void rotate(float radiant) {

    float x = BALL_STEPS_X, y = BALL_STEPS_Y;
    // Temp x, um den X-Wert fuer die Formel nicht vorher zu ueberschreiben
    float tempX = x, tempY = y;
    float cos = cosf(radiant);
    float sin = sinf(radiant);

    // Neuer X-Wert: x * cos + y * sin
    // Neuer Y-Wert: -y * sin + y * cos
    x = tempX * cos + tempY * sin;
    y = -tempX * sin + tempY * cos;

    // Werte setzen
    ballSpeedVector[0] = x * ballSpeed;
    ballSpeedVector[1] = y * ballSpeed;
}

/**
 * Prueft, ob der Ball mit einem der Raender kollidiert
 * @return gibt die Seite der Kollision aus
 */
static CGSide checkBorderCollision(void) {
    CGSide res = sideNone;

    // Fuer bessere Uebersicht lokale Variablen erstellen
    float ballX = ballCenter[0], ballY = ballCenter[1], stickX = stickCenter[0], stickY = stickCenter[1];
    float collisionOffset = 0.005f;

    // Ball ist unter dem Stick, also (Punkt) verloren
    if (ballY <= -1.0f) {
        resetExtraPosition();
        handleLoss();
    }

    // Ball fliegt nach rechts
    if (ballSpeedVector[0] > 0.0f &&
        ballX + (BALL_WIDTH / 2) + collisionOffset >= BAR_X_OFFSET - BAR_THICKNESS) {
        res = sideRight;
    }

        // Ball fliegt nach links
    else if (ballSpeedVector[0] < 0.0f &&
             ballX - (BALL_WIDTH / 2) - collisionOffset <= -BAR_X_OFFSET + BAR_THICKNESS) {
        res = sideLeft;
    }

        // Ball fliegt nach oben und
    else if (ballSpeedVector[1] > 0.0f &&
             // zusaetzlich Bar-Breite abziehen, weil ueber die Top-Bar noch der Text steht
             ballY + BALL_WIDTH / 2 + collisionOffset >= BAR_X_OFFSET - BAR_WIDTH - BAR_THICKNESS) {
        res = sideTop;
    }

        // Ball fliegt nach unten und
    else if (ballSpeedVector[1] < 0.0f &&
             (ballX >= (stickX - stickWidth / 2)) &&
             (ballX <= (stickX + stickWidth / 2) &&
              ballY + BALL_WIDTH / 2 < stickY + BAR_THICKNESS)) {

        // An dieserm Punkt kollidiert der Ball auf der X-Achse mit dem Stick
        float collisionX = ballX - stickX;

        // Neues Winkelmaß berechnen
        float radiant = calculateRadiant(collisionX);

        // Wenn Ball in der Mitte auftritt, Ball umkehren,
        // sonst mit winkelmaß rotieren
        if (radiant == 0) {
            ballSpeedVector[1] *= -1;
        } else {
            rotate(radiant);
        }

        // Side none, damit nicht erneut Ball umgedreht wird
        // (in handleBorderCollision())
        res = sideNone;
    }

    return res;
}

/**
 * Extra Punkte dem Spieler hinzufuegen und eine Info ausgeben
 */
void addExtraPoints() {
    player.points += EXTRA_POINTS;
    extraPoints += EXTRA_POINTS;
    printf("Extra: %d Punkte erhalten\n", EXTRA_POINTS);
}

/**
 * Stick vergroessern (bis zu einer maximalen Breite)
 * Falls Stick maximale Breite erreicht hat, stattdessen Extrapunkte vergeben
 */
void increaseStickWidth() {
    // Den Stick nicht zu breit machen :)
    float maxWidth = STICK_WIDTH * 3;

    if (stickWidth < maxWidth) {
        stickWidth *= EXTRA_WIDEN;
        printf("Extra: Stick wurde verbreitert!\n");
    } else {
        addExtraPoints();
    }
}

/**
 * Ball geschwindigkeit verringern
 * Falls minimale Geschwindigkeit erreicht wurde, stattdessen Extrapunkte vergeben
 */
void decreaseBallSpeed() {
    printf("Extra: Ball wurde verlangsamt!\n");

    float minSpeed = BALL_SPEED_INITIAL;

    if (ballSpeed > minSpeed) {
        ballSpeedVector[0] /= ballSpeed;
        ballSpeedVector[1] /= ballSpeed;

        ballSpeed -= BALL_SPEED_MODIFIER;

        ballSpeedVector[0] *= ballSpeed;
        ballSpeedVector[1] *= ballSpeed;
    } else {
        addExtraPoints();
    }
}

/**
 * Zufaellig eines der drei Extras auswaehlen
 * (Stick vergroessern, Ballgeschwindigkeit verringern oder Extrapunkte hinzufuegen)
 */
void chooseExtra() {

    int random = genRandomNumber(3, 1);

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

/**
 * Prueft, ob der Ball das Extra getroffen hat
 * Falls er es getroffen hat, Extra anzeigen
 */
void checkExtraCollision() {
    float extraX = extraCenter[0];
    float extraY = extraCenter[1];

    float stickX = stickCenter[0];
    float stickY = stickCenter[1];

    // Wenn Extra auf Schlaeger auftrifft
    if ((extraX >= stickX - stickWidth / 2) &&
        (extraX <= stickX + stickWidth / 2)
        && extraY < stickY + BAR_THICKNESS) {
        chooseExtra();
        resetExtraPosition();
    }
}

/**
 * Berechnet neue Position des Balls, wenn das Spiel nicht pausiert wurde.
 * @param interval Dauer der Bewegung in Sekunden.
 */
void calcBallPosition(double interval) {

    if (!gamePaused) {
        CGSide side = checkBorderCollision();

        if (side != sideNone) {
            handleBorderCollision(side);
        } else {
            ballCenter[0] += ballSpeedVector[0] * (float) interval;
            ballCenter[1] += ballSpeedVector[1] * (float) interval;
        }
    }
}

/**
 * Berechnet neue Position des Extras
 * @param interval Dauer der Bewegung in Sekunden.
 */
void calcExtraPosition(double interval) {
    // Extra nach unten bewegen
    extraCenter[1] -= extraSpeed * (float) interval;
    checkExtraCollision();

    // Wenn Extra nicht getroffen wurde und zu tief ist,
    // soll die Position fuer das naechste Extra zurueckgesetzt werden
    if (extraCenter[1] <= -1.0f) {
        resetExtraPosition();
    }
}

/**
 * Berechnet neue Position des Sticks, wenn das Spiel nicht pausiert wurde
 * @param interval Dauer der Bewegung in Sekunden.
 */
void
calcStickPosition(double interval) {

    // Stick nach links bewegen
    if (!gamePaused) {
        if (stickMovementDirection[dirLeft]) {
            // Position linker Balken + Breite des Sticks - Breite des linken Balkens
            // ist der Punkt, der den Stick stoppt
            float stickPosition = stickCenter[0] - (stickWidth / 2);
            float leftBarPosition = -BAR_X_OFFSET + BAR_WIDTH / 2;

            if (stickPosition > leftBarPosition) {
                stickCenter[0] -= STICK_SPEED * (float) interval;
            }
        }

        // Stick nach rechts bewegen
        if (stickMovementDirection[dirRight]) {
            float stickPosition = stickCenter[0] + (stickWidth / 2);
            float rightBarPosition = BAR_X_OFFSET - BAR_WIDTH / 2;

            if (stickPosition < rightBarPosition) {
                stickCenter[0] += STICK_SPEED * (float) interval;
            }
        }
    }
}

/**
 * Wenn die Bloecke getroffen wurden, Punkte hinzufuegen
 * und ggf. Leben und Geschwindigkeit erhoehen
 */
void handleHits() {
    player.points += 1;

    int points = player.points - extraPoints;

    // Alle 20 Punkte ein Leben hinzufuegen
    if (points % 20 == 0) {
        player.lives += 1;
        printf("Leben erhalten!\n");
    }

    // Alle 10 Punkte die Geschwindigkeit erhoehen
    if (points % 10 == 0) {
        // Ball speed entfernen für Inititalgeschwindigkeit
        // plus richtigem Winkel
        ballSpeedVector[0] /= ballSpeed;
        ballSpeedVector[1] /= ballSpeed;

        // Ballgeschwindigkeit erhoehen
        ballSpeed += BALL_SPEED_MODIFIER;
        ballSpeedVector[0] *= ballSpeed;
        ballSpeedVector[1] *= ballSpeed;
        printf("Ballgeschwindigkeit erhoeht!\n");
    }

    // EXTRAS
    if (!showExtra && propabilityOccured()) {
        showExtra = GL_TRUE;
        extraCenter[0] = ballCenter[0];
        extraCenter[1] = ballCenter[1];
    }

    // Ausgabe
    printf("Punkte: %d | Verbleibende Leben: %d\n", player.points, player.lives);

    // Spieler hat gewonnen
    if (points >= BLOCKS_COUNT) {
        // Gewonnen
        printf("Sie haben gewonnen!");

        // Sofortige Ausgabe
        fflush(stdout);

        gamePaused = GL_TRUE;
    }
}

/**
 * Prueft, ob der uebergebene Block vom Ball getroffen wurde
 * @param block - dieser Block wird geprueft
 * @return true wenn collided, false wenn nicht
 */
GLboolean blockCollided(Block *block) {

    // Blockposition
    GLfloat blockX = block->position[0], blockY = block->position[1];

    // Ballposition
    GLfloat ballX = ballCenter[0], ballY = ballCenter[1];

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
        //
        float xCheckLeft = fabsf(ballX - blockLeft);
        float xCheckRight = fabsf(ballX - blockRight);
        float xCheckBottom = fabsf(ballY - blockBottom);
        float xCheckTop = fabsf(ballY - blockTop);

        // Beim kleinsten Wert wurde der Block getroffen
        float val = xCheckLeft;
        CGSide side = sideLeft;

        if (xCheckRight < val) {
            val = xCheckRight;
            side = sideRight;
        }

        if (xCheckBottom < val) {
            val = xCheckBottom;
            side = sideBottom;
        }

        if (xCheckTop < val) {
            side = sideTop;
        }

        // neuer Winkel fuer den Ball
        handleBorderCollision(side);

        // Kollision
        return GL_TRUE;
    }

    return GL_FALSE;
}

/**
 * Setzt den Bewegunsstatus des Sticks.
 * @param direction Bewegungsrichtung deren Status veraendert werden soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Rechteck bewegt sich in Richtung
 * direction, GL_FALSE->Sticks bewegt sich nicht in Richtung direction.
 */
void
setStickMovement(CGDirection direction, GLboolean status) {
    stickMovementDirection[direction] = status;
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Sticks.
 * @return Postion (des Mittelpunktes) des Sticks.
 */
CGPoint2f *
getStickCenter(void) {
    return &stickCenter;
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Balls.
 * @return Postion (des Mittelpunktes) des Balls.
 */
CGPoint2f *
getBallCenter(void) {
    return &ballCenter;
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Extras.
 * @return Postion (des Mittelpunktes) des Extras.
 */
CGPoint2f *
getExtraCenter(void) {
    return &extraCenter;
}