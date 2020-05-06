
#include <GL/gl.h>
#include <GL/glut.h>
#include "scene.h"
#include "logic.h"
#include "math.h"
#include "variables.h"
#include "types.h"
#include "debug.h"
#include "helper.h"
#include <time.h>

GLboolean showWireframe = GL_FALSE;
static Block bloecke[BLOCKS_COUNT];
float stickWidth = STICK_WIDTH;

// Winkel fuer das Extra
int tAngle = 0;

/**
 * Zeichnet ein Dreieck (fuer das Extra)
 */
static void drawTriangle() {
    glBegin(GL_TRIANGLES);

    float x = 0.7f;
    glColor3f(1, 0.843, 0);

    // Links unten
    glVertex2f(-x, -x);

    // Rechts unten
    glVertex2f(x, -x);

    // oben
    glVertex2f(0, x);

    glEnd();
}

/**
 * Zeichnet das Extra
 * @param coords an diesen Koordinaten wird das Extra gezeichnet
 */
void drawExtra(const CGPoint2f coords) {
    GLfloat x = coords[0];
    GLfloat y = coords[1];
    glColor3f(0.0f, 1.0f, 0.0f);

    glPushMatrix();
    {
        glTranslatef(x, y, ZERO);
        glRotatef(tAngle, 0, 0, 1);

        glScalef(EXTRA_WIDTH, EXTRA_HEIGHT, 2.0f);
        drawTriangle();

        if (!gamePaused) {
            // Extra drehen
            tAngle += 2;
            if (tAngle > 360) {
                tAngle = 0;
            }
        }
    }

    glPopMatrix();
}

/**
 * Zeichnet ein Rechteck mit der Breite und Hoehe 1.
 */
static void drawSquare() {

    float length = 0.5f;

    glBegin(GL_QUADS);
    {
        // Links unten
        glVertex2f(-length, -length);

        // Rechts unten
        glVertex2f(length, -length);

        // Links oben
        glVertex2f(length, length);

        // Rechts oben
        glVertex2f(-length, length);
    }

    glEnd();
}

/**
 * Zeichnet einen Kreis (fuer den Ball)
 */
static void drawCircle() {

    float radius = 1.0f;

    // Um diesen Wert, den Winkel erhoehen, um irgendwann auf 360 zu kommen
    // je kleiner der Wert, desto runder der Ball
    float increaseValue = 0.01f;

    glBegin(GL_POLYGON);

    // Winkel immer minimal erhoehen und somit den Kreis zeichnen
    // 2 * PI entsprechen 360°
    for (float angle = 0; angle < 2 * M_PI; angle += increaseValue) {
        glVertex3f(radius * cosf(angle), radius * sinf(angle), 0);
    }

    glEnd();
}

/**
 * Zeichnet den Ball anhand angegebener Koordinaten
 * @param coords - An dieser Stelle soll der Ball erscheinen
 */
static void drawBall(const CGPoint2f coords) {
    GLfloat x = coords[0];
    GLfloat y = coords[1];
    glColor3f(0.827f, 0.827f, 0.827f);

    glPushMatrix();
    {
        glTranslatef(x, y, ZERO);
        glScalef(BALL_WIDTH, BALL_WIDTH, 1.0f);
        drawCircle();
    }

    glPopMatrix();
}

/**
 * Zeichnet einen Block
 * @param block - enthaelt alle Informationen zu diesem Block (Koordinatien, Farben)
 */
static void drawBlock(const Block block) {
    GLfloat x = block.position[0];
    GLfloat y = block.position[1];
    glColor3f(block.color[0], block.color[1], block.color[2]);

    glPushMatrix();
    {
        glTranslatef(x, y, ZERO);
        glScalef(BLOCK_WIDTH, BLOCK_HEIGHT, 1.0f);
        drawSquare();
    }

    glPopMatrix();
}

/**
 * Zeichnet den Stick an uebergebenen Koordinaten
 * @param coords - Koordnaten des Sticks
 */
static void drawStick(const CGPoint2f coords) {

    GLfloat x = coords[0];
    GLfloat y = coords[1];
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    {
        glTranslatef(x, y, ZERO);
        glScalef(stickWidth, BAR_THICKNESS, 1.0f);
        drawSquare();
    }

    glPopMatrix();
}

/**
 * Zeichnet einen Teil der Spielfeldbegrenzung.
 *
 */
static void drawBorder(GLfloat posX, GLfloat posY, int showTop) {
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    {
        if (showTop) {
            glTranslatef(posX, posY, ZERO);
            glScalef(BAR_WIDTH, BAR_HEIGHT, 1.0f);
        } else {
            glTranslatef(posX, posY, ZERO);
            glScalef(BAR_HEIGHT, BAR_WIDTH, 1.0f);
        }

        drawSquare();
    }
    glPopMatrix();
}

/**
 * Alle Bloecke zeichnen, wenn sie nicht versteckt sind und
 * der Ball gerade nicht kollidiert
 */
void drawBlocks() {
    for (int i = 0; i < BLOCKS_COUNT; i++) {
        if (!bloecke[i].hidden && !blockCollided(&bloecke[i])) {
            drawBlock(bloecke[i]);
        }
    }
}

/**
 * Generiert alle Bloecke mit Position und Farben
 * und speichert sie in das uebergebene Blockarray
 * @param block Blockarray, das gefuellt wird
 */
void generateBlocks(Block *block) {
    int count = 0;
    float width = 0.65f;
    float height = 0.7f;
    float gap = 0.2f;

    // Spielfeld-Koordinaten - Erst von links nach rechts, dann von oben nach unten
    for (float j = width; j > -width; j -= gap) {
        for (float i = -height; i <= height; i += gap) {

            // Farben fuer den Block zufaellig generieren
            float *colors = selectColor(genRandomNumber(5, 1));

            block[count].color[0] = colors[0];
            block[count].color[1] = colors[1];
            block[count].color[2] = colors[2];

            block[count].hidden = 0;
            block[count].position[0] = i;
            block[count].position[1] = j;

            count++;

            free(colors);
        }
    }
}

/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int
initScene(void) {
    /* Setzen der Farbattribute */
    /* Hintergrundfarbe */
    glClearColor(0.1f, 0.1f, 0.11f, 1.0f);

    /* Zeichenfarbe */
    glColor3f(1.0f, 1.0f, 1.0f);

    /* Groese von Punkten */
    glPointSize(4.0f);

    /* Breite von Linien */
    glLineWidth(2.0f);

    // Zeit fuer Random einbeziehen, um Zufallsfarben zu generieren
    // srand steht hier, weil es nur einmal aufgerufen werden darf
    srand(time(NULL));

    // Ball soll in zufaellige Richtung fliegen
    setRandomBallAngle();

    /* Bloecke generieren */
    generateBlocks(bloecke);

    /* Alles in Ordnung? */
    return (GLGETERROR == GL_NO_ERROR);
}

/**
 * Schaltet den Wireframe-Modus an oder aus
 */
void toggleWireframe() {
    showWireframe = !showWireframe;
    if (showWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

/**
 * Zeichnet die gesamte Szene (Grenzen, Ball, Stick und evtl. Extras
 */
void drawScene(void) {

    /* In der Logik berechnet Position beziehen */
    CGPoint2f *stickCenter = getStickCenter();
    CGPoint2f *ballCenter = getBallCenter();
    CGPoint2f *extraCenter = getExtraCenter();

    /* Rahmen zeichnen */
    drawBorder(BAR_X_OFFSET, ZERO, 1);
    drawBorder(-BAR_X_OFFSET, ZERO, 1);
    drawBorder(ZERO, BAR_X_OFFSET - BAR_WIDTH, 0);

    drawStick(*stickCenter);
    drawBall(*ballCenter);

    if (showExtra) {
        drawExtra(*extraCenter);
    }

    drawBlocks();
}
