#include "drawObjects.h"
#include "io.h"
#include "types.h"

/* ---- System Header einbinden ---- */
#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>

#endif

#include "logic.h"
#include "math.h"
#include "scene.h"
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

    int idx = isFirst
              ? 0

              : getState()->grid.length * getState()->grid.length - 1;
    float x = isFirst
              ? 2.2f
              : -2.3f;
    double y = 0.05f + getState()->grid.vertices[idx][Y];
    float z = isFirst
              ? 2.25f
              : -2.3f;

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
 * Zeichnet das "Dach" des Leuchtturmes
 */
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

/**
 * Zeichnet den Leuchtturm
 */
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
 * - Reduziert Codeverdopplung
 * @param isBody wenn True, werden die Werte fuer den
 * Inselboden angepasst
 */
void islandDrawer(GLboolean isBody) {

    int count = 18;
    float size = 0.5f;
    float a = 0;
    for (int i = 0; i <= count; i++) {
        double x = size * cosf(a);
        double z = size * sinf(a);

        if (isBody) {
            glNormal3f(x, 0.0f, 0);
            glTexCoord2f(0, z);
            glVertex3f(x, -1, z);

            glTexCoord2f(z, x);
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

/**
 * Hilfsfunktion, um die Farben der Baelle und des Wasser zu setzen
 * @param index - an der Stelle
 * @param r - R Wert
 * @param g - G Wert
 * @param b - B Wert
 * @param colorBalls - True, wenn die Baelle gefaerbt werden sollen
 */
void setColors(int index, float r, float g, float b, GLboolean colorBalls) {
    if (colorBalls) {
        setMaterialLightning(1, 1, 1);
        glColor3f(1, 1, 1);
    } else {
        setMaterialLightning(r, g, b);
        glColor3f(r, g, b);
    }

    if (!getState()->settings.showTextures) {
        getState()->grid.vertices[index][R] = r;
        getState()->grid.vertices[index][G] = g;
        getState()->grid.vertices[index][B] = b;
    }
}

/**
 * Faerbt die Kugeln / Wasser anhand der Hoehe
 * @param index - Richtige Kugel awehlen
 * @param height - Die neue Hoehe, die die Kugel hat
 */
void changeColors(int index, float height) {

    if (height >= COLOR_HEIGHT_1 && height < COLOR_HEIGHT_2) {
        // Gold
        setColors(index, 1.000f, 0.843f, 0.000f, GL_FALSE);
    } else if (height >= COLOR_HEIGHT_2 && height < COLOR_HEIGHT_3) {
        // Gruen
        setColors(index, 0.180f, 0.545f, 0.341f, GL_FALSE);
    } else if (height >= COLOR_HEIGHT_3) {
        // Pink
        setColors(index, 1.000f, 0.078f, 0.576f, GL_FALSE);
    } else if (height < COLOR_HEIGHT_1) {
        // Initial Blau
        setColors(index, 0, 0.3f, 0.8f, GL_TRUE);
    }
}


/**
 * Zeichnet eine Sphere
 * @param index
 */
void drawSphere(int index) {


    glPushName(index);
    {
        glPushMatrix();
        {
            float height = getState()->grid.vertices[index][Y];
            glTranslatef(
                    getState()->grid.vertices[index][X],
                    height,
                    getState()->grid.vertices[index][Z]
            );

            /* Normalen zeichnen */
            if (getState()->settings.showNormals) {
                glBegin(GL_LINES);
                {
                    glVertex3f(0, 0,
                               0);
                    glVertex3f(getState()->grid.vertices[index][NX] * 0.1f, 0.1,
                               getState()->grid.vertices[index][NZ] * 0.1f);
                }
                glEnd();
            }
            changeColors(index, height);

            /* Quadric erzuegen */
            GLUquadricObj *qobj = gluNewQuadric();

            if (qobj != 0) {
                /* Normalen fuer Quadrics berechnen lassen */
                gluQuadricNormals(qobj, GLU_SMOOTH);

                /* Kugel zeichen */
                float radius = 1.0f / (getState()->grid.length * (float) 5);
                gluSphere(qobj, radius, 20, 20);

                /* Loeschen des Quadrics nicht vergessen */
                gluDeleteQuadric(qobj);
            } else {
                CG_ERROR (("Could not create Quadric\n"));
            }
        }
        glPopMatrix();
    }
    glPopName();
}

/**
 * Laesst alle Spheren zeichnen
 */
void drawSpheres() {

    int length = getState()->grid.length;

    for (int i = 0; i < length * length; i++) {
        drawSphere(i);
    }
}

/**
 * Zeichnet die Spheren, die gepickt werden koennen
 * (sind theoretisch unsichtbar und dienen nur zum Picken)
 */
void drawPickingSpheres() {
    glPushMatrix();
    {
        glScalef(5, 5, 5);
        drawSpheres();
    }
    glPopMatrix();
}
