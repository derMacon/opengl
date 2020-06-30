#include <GL/gl.h>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#endif

#include "logic.h"
#include "scene.h"

/**
 * Zeichnet ein 3D-Viereck
 */
static void drawSquare() {

    int x, y;
    float w = 0.5f;
    int count = 10;

    /* Normalen zeichnen */
    if (getState()->settings.showNormals) {
        glBegin(GL_LINES);
        {
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.3f);
        }
        glEnd();
    }

    /* Viele kleine Squares erstellen */
    for (y = 0; y < count; y++) {
        glBegin(GL_QUAD_STRIP);
        {
            glNormal3f(0.0f, 0.0f, 1.0f);
            for (x = 0; x <= count; x++) {
                glVertex3f(-w + (float) x / (count),
                           w - (float) y / (count),
                           0.0f);

                glVertex3f(-w + (float) x / (count),
                           w - (float) (y + 1) / (count),
                           0.0f);
            }
        }
        glEnd();
    }
}

void
drawCube() {
    glPushMatrix();
    {
        /* Frontflaeche */
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.0f, 0.5f);
            drawSquare();
        }
        glPopMatrix();

        /* rechte Seitenflaeche */
        glPushMatrix();
        {
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, 0.5f);
            drawSquare();
        }
        glPopMatrix();

        /* Rueckseitenflaeche */
        glPushMatrix();
        {
            glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, 0.5f);
            drawSquare();
        }
        glPopMatrix();

        /* linke Seitenflaeche */
        glPushMatrix();
        {
            glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, 0.5f);
            drawSquare();
        }
        glPopMatrix();

        /* Deckelflaeche */
        glPushMatrix();
        {
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, 0.5f);
            drawSquare();
        }
        glPopMatrix();

        /* Bodenflaeche */
        glPushMatrix();
        {
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, 0.5f);
            drawSquare();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawFront() {
    glPushMatrix();
    {
        glTranslatef(0.5f, -0.0001f, 0);
        glRotatef(45, 0, 1, 0);
        glScalef(0.7f, 1, 0.7f);
        drawCube();
    }
    glPopMatrix();
}

void drawDach() {
    glColor3f(0.8f, 0.8f, 0.8f);
    setMaterialLightning(0.8f, 0.8f, 0.8f);
    glPushMatrix();
    {
        glTranslatef(-0.1f, 1.3f, 0);
        glScalef(0.7f, 1.2f, 0.8);
        drawCube();
    }
    glPopMatrix();
}

void drawBoat(GLboolean isFirst) {
    glColor3f(0.600f, 0.240f, 0.100f);
    setMaterialLightning(0.600f, 0.240f, 0.100f);
    glPushMatrix();
    {
        if (isFirst) {
            glTranslatef(1.7, 0, 1);
            glRotatef(110, 0, 1, 0);
        } else {
            glTranslatef(-1.5, 0, -1.7f);
            glRotatef(-20, 0, 1, 0);
        }
        glScalef(0.35f, 0.1, 0.25f);
        drawCube();

        drawFront();
        drawDach();
    }
    glPopMatrix();
}
