#include <GL/gl.h>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#endif

#include <stdio.h>
#include "logic.h"
#include "math.h"
#include "scene.h"
#include "drawWater.h"
#include "texture.h"
#include "debug.h"

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

/** Zeichnet ein 3d-Vierreck */
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

/**
 * Zeichnet das vordere Dreieck eines Bootes
 */
void drawBoatFront() {
    glPushMatrix();
    {
        glTranslatef(0.5f, -0.0001f, 0);
        glRotatef(45, 0, 1, 0);
        glScalef(0.7f, 1, 0.7f);
        drawCube();
    }
    glPopMatrix();
}

/**
 * Zeichnet das "Dach" eines Bootes
 */
void drawBoatRoof() {
    glColor3f(0.8f, 0.8f, 0.8f);
    setMaterialLightning(0.8f, 0.8f, 0.8f);
    glPushMatrix();
    {
        glTranslatef(-0.1f, 1.2f, 0);
        glScalef(0.7f, 1.2f, 0.8);
        drawCube();
    }
    glPopMatrix();
}

/**
 * Zeichnet ein Boot
 * @param isFirst true, wenn es das erste sein soll
 */
void drawBoat(GLboolean isFirst) {
    glColor3f(0.600f, 0.240f, 0.100f);
    setMaterialLightning(0.600f, 0.240f, 0.100f);
    int idx = isFirst ? 0 : getState()->grid.length * getState()->grid.length - 1;
    float x = isFirst ? 2.2f : -2.3f;
    double y = 0.05f + getState()->grid.vertices[idx][Y];
    float z = isFirst ? 2.25f : -2.3f;

    glPushMatrix();
    {
        if (isFirst) {
            glTranslatef(x, y, z);
            glRotatef(110, 0, 1, 0);
        } else {
            glTranslatef(x, y, z);
            glRotatef(-20, 0, 1, 0);
        }
        glScalef(0.35f, 0.1, 0.25f);
        drawCube();

        drawBoatFront();
        drawBoatRoof();
    }
    glPopMatrix();
}

/**
 * Zeichnet einen Zylinder
 */
//static void drawCylinder() {
//}

void drawLighthouseRoof() {
    glColor3f(0.5f, 0.1f, 0.1f);
    setMaterialLightning(0.5f, 0.1f, 0.1f);

    glPushMatrix();
    {
        glTranslatef(0, 1, 0);
        glRotatef(-90, 1, 0, 0);
        GLUquadricObj *qobj = gluNewQuadric();
        if (qobj != 0) {
            gluQuadricNormals(qobj, GLU_SMOOTH);
            qobj = gluNewQuadric();
            gluCylinder(qobj, 0.2f, 0.0f, 0.3f, 32, 32);
            gluDeleteQuadric(qobj);
        } else {
            CG_ERROR(("Could not create Quadric\n"));
        }
    }
    glPopMatrix();
}

/* Loeschen des Quadrics nicht vergessen */

void drawLighthouse() {

    glColor3f(0.9f, 0.9f, 0.9f);
    setMaterialLightning(0.9f, 0.9f, 0.9f);

    glPushMatrix();
    {
        glTranslatef(0, 1, 0);
        glScalef(1, 1, 1);
        glRotatef(90.0f, 1.0f, 0, 0.0f);
        /* Quadric erzuegen */
        GLUquadricObj *qobj = gluNewQuadric();
        if (qobj != 0) {
            gluQuadricNormals(qobj, GLU_SMOOTH);
            gluQuadricTexture(qobj, GL_TRUE);
            bindTexture(texLightHouse);
            gluCylinder(qobj, 0.2f, 0.2f, 1.0f, 32, 32);
            gluDeleteQuadric(qobj);
        } else {
            CG_ERROR(("Could not create Quadric\n"));
        }
    }

    glPopMatrix();
    drawLighthouseRoof();
}

/**
 * Zeichnet die Insel
 * @param isBody
 */
void islandDrawer(GLboolean isBody) {

    int count = 18;
    float size = 0.5f;
    float a = 0;
    for (int i = 0; i <= count; i++) {
        double x = size * cosf(a);
        double z = size * sinf(a);

        if (isBody) {
            glTexCoord2f(0, z);
            glVertex3f(x, -1, z);

            glTexCoord2f(z , x);
            glVertex3f(x, 1, z);
        } else {
            glTexCoord2f(z, x);
            glVertex3f(z, 0, x);
        }

        a += M_PI * 2 / (float) count;
    }
}

/**
 * Zeichnet das Oberteil der Insel
 */
void drawIslandTop() {
    glPushMatrix();
    {

        glTranslatef(0, 0.3, 0);
        glColor3f(0.3, 0, 0);
        glRotatef(10, 0, 1, 0);

        if (getState()->settings.showNormals) {
            glBegin(GL_LINES);
            {
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(0.0f, 1.0f, 0.0f);
            }
            glEnd();
        }

        glBegin(GL_POLYGON);
        {
            glNormal3f(0.0f, 1.0f, 0.0f);
            islandDrawer(GL_FALSE);
        }
        glEnd();
    }
    glPopMatrix();
}


/**
 * Zeichnet den "Körper" der Insel
 */
void drawIslandBody() {
    glPushMatrix();
    {
        glScalef(1, 0.3, 1);
        glBegin(GL_TRIANGLE_STRIP);
        {
            islandDrawer(GL_TRUE);
        }
        glEnd();
    }
    glPopMatrix();
}

/**
 * Laesst die Insel zeichnen
 */
void drawIsland() {

    glPushMatrix();
    {
        float c = 0.1f;
        glColor3f(c, c, c);
        setMaterialLightning(c, c, c);

        bindTexture(texIsland);
        drawIslandTop();
        drawIslandBody();
    }
    glPopMatrix();
}