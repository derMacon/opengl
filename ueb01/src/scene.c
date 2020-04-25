
#include <GL/gl.h>
#include <GL/glut.h>
#include "scene.h"
#include "debug.h"
#include "logic.h"
#include "math.h"
#include "variables.h"

/**
 * Zeichnet ein Rechteck mit der Breite und Hoehe 1.
 */
static void drawSquare(void) {
    glBegin(GL_QUADS);
    {
        // Links unten
        glVertex2f(-0.5, -0.5);

        // Rechts unten
        glVertex2f(0.5, -0.5);

        // Links oben
        glVertex2f(0.5, 0.5);

        // Rechts oben
        glVertex2f(-0.5, 0.5);
    }
    glEnd();
}

static void drawRound(void) {

    float radius = 1.0f;

    // TODO: WTF is delta theta
    float delta_theta = 0.01f;

    glBegin(GL_POLYGON);

    // TODO: Erklaerung hierfuer finden xD
    for (float angle = 0; angle < 2 * M_PI; angle += delta_theta) {
        glVertex3f(radius * cos(angle), radius * sin(angle), 0);
    }

    glEnd();
}

static void drawBall(const CGPoint2f coords) {
    GLfloat x = coords[0];
    GLfloat y = coords[1];

    glColor3f(1.0f, 0.0f, .0f);

    glPushMatrix();
    {
        glTranslatef(x, y, ZERO);
        glScalef(BALL_WIDTH, BALL_WIDTH, 1.0f);
        drawRound();
    }

    glPopMatrix();
}

static void drawStick(const CGPoint2f coords) {

    GLfloat x = coords[0];
    GLfloat y = coords[1];

    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    {
        glTranslatef(x, y, ZERO);
        glScalef(STICK_WIDTH, BAR_THICKNESS, 1.0f);
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
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int
initScene(void) {
    /* Setzen der Farbattribute */
    /* Hintergrundfarbe */
    glClearColor(ZERO, ZERO, ZERO, ZERO);
    /* Zeichenfarbe */
    glColor3f(1.0f, 1.0f, 1.0f);

    /* Groese von Punkten */
    glPointSize(4.0f);

    /* Breite von Linien */
    glLineWidth(2.0f);

    /* Alles in Ordnung? */
    return (GLGETERROR == GL_NO_ERROR);
}

void drawScene(void) {

    /* In der Logik berechnet Position beziehen */
    CGPoint2f *stickCenter = getStickCenter();
    CGPoint2f *ballCenter = getBallCenter();

    /* Rahmen zeichnen */
    drawBorder(BAR_X_OFFSET, ZERO, 1);
    drawBorder(-BAR_X_OFFSET, ZERO, 1);
    drawBorder(ZERO, BAR_X_OFFSET - BAR_WIDTH, 0);

    drawStick(*stickCenter);
    drawBall(*ballCenter);

   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}