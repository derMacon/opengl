
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
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


static void drawStick(GLfloat x, GLfloat y) {
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    {
        glTranslatef(x, y, 0.0f);
        glScalef(0.5f, BAR_THICKNESS, 1.0f);
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
            // Obere Bar
            glTranslatef(posX, posY, 0.0f);
            glScalef((posY + BAR_THICKNESS) * 2 + (BAR_THICKNESS * 2) - OFFSET, BAR_THICKNESS, 1.0f);
        } else {
            glTranslatef(posX, -(2 * OFFSET), 0.0f);

            glScalef(BAR_THICKNESS, BAR_HEIGHT - (BAR_THICKNESS * 2) + OFFSET, 1.0f);
            printf("a");
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

    /* Rahmen zeichnen */
    drawBorder(0.0f, TOP_BAR, 1);
    drawBorder(LEFT_BAR, TOP_BAR / 10, 0);
    drawBorder(RIGHT_BAR, TOP_BAR / 10, 0);
    drawStick(0.0f, -0.9f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}