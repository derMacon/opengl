
#include <GL/gl.h>
#include <GL/glu.h>
#include "scene.h"
#include "debug.h"

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

/**
 * Zeichnet einen Teil der Spielfeldbegrenzung.
 *
 */
static void drawBorder(GLfloat posX, GLfloat posY, int showTop) {
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    {
        glTranslatef(posX, posY, 0.0f);

        if (showTop) {
            // Obere Bar
            glScalef(TOP_BAR, BAR_THICKNESS, 1.0f);
        } else {
            // Wie breit, wie hoch, wie Z-Achse lel
            glScalef(BAR_THICKNESS, BAR_HEIGHT, 1.0f);
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
initScene (void)
{
    /* Setzen der Farbattribute */
    /* Hintergrundfarbe */
    glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
    /* Zeichenfarbe */
    glColor3f (1.0f, 1.0f, 1.0f);

    /* Groese von Punkten */
    glPointSize (4.0f);

    /* Breite von Linien */
    glLineWidth (2.0f);

    /* Alles in Ordnung? */
    return (GLGETERROR == GL_NO_ERROR);
}

void drawScene(void) {

    /* Rahmen zeichnen */
    drawBorder(LEFT_BAR, 0.0f, 0);
    drawBorder(RIGHT_BAR, 0.0f, 0);

    drawBorder(0.0f, (BAR_HEIGHT / 2) - (BAR_THICKNESS / 2), 1);

    glBegin(GL_POLYGON);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.25f, 0.5f);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(-0.25f, 0.5f);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(-0.25f, -0.5f);

        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(0.25f, -0.5f);
    }
    glEnd();
}