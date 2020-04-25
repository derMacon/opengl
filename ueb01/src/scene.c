
#include <GL/gl.h>
#include "scene.h"
#include "debug.h"
#include "logic.h"
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
    CGPoint2f *quadCenter = getQuadCenter();

    /* Rahmen zeichnen */
    drawBorder(BAR_X_OFFSET, ZERO, 1);
    drawBorder(-BAR_X_OFFSET, ZERO, 1);
    drawBorder(ZERO, BAR_X_OFFSET - BAR_WIDTH, 0);

    drawStick(*quadCenter);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}