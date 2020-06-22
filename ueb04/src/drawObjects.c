#include "debug.h"
#include "types.h"
#include "logic.h"

GLfloat houseColors[3] = {1.0f, 0.0f, 1.0f};

/**
 * Setzt das Materiallicht fuer die gezeichnetetn Objekte
 * @param r - rot
 * @param g - gruen
 * @param b - blau
 */
void setMaterialLightning(float r, float g, float b) {
    /* Verringert die Saetting der Farben, sodass nur noch x% angzeigt werden */
    float multiplier = 0.15f;

    float matDiffuse[] = {r, g, b, 1};
    float matAmbient[] = {r * multiplier, g * multiplier, b * multiplier, 1.0f};
    float matSpecular[] = {r, g, b, 1.0f};
    float matShininess = 20;

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininess);
}

/**
 * Zeichnet ein 3D-Viereck
 */
static void drawSquare() {

    int x, y;
    float w = 0.5f;

    /* Normalen zeichnen */
    if (getGame()->settings.showNormals) {
        glBegin(GL_LINES);
        {
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.3f);
        }
        glEnd();
    }

    /* Viele kleine Squares erstellen */
    for (y = 0; y < SQUARE_REFINEMENT_COUNT; y++) {
        glBegin(GL_QUAD_STRIP);
        {
            glNormal3f(0.0f, 0.0f, 1.0f);
            for (x = 0; x <= SQUARE_REFINEMENT_COUNT; x++) {
                glVertex3f(-w + (float) x / (SQUARE_REFINEMENT_COUNT),
                           w - (float) y / (SQUARE_REFINEMENT_COUNT),
                           0.0f);

                glVertex3f(-w + (float) x / (SQUARE_REFINEMENT_COUNT),
                           w - (float) (y + 1) / (SQUARE_REFINEMENT_COUNT),
                           0.0f);
            }
        }
        glEnd();
    }
}

/**
 * Zeichnet ggf. die Normalen an der angegebenen Stelle
 * @param x - Wert
 * @param y - Wert
 * @param z - Wert
 */
void drawNormals(float x, float y, float z) {
    if (getGame()->settings.showNormals) {
        glBegin(GL_LINES);
        {
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(x, y, z);
        }
        glEnd();
    }
}

/**
 * Zeichnet eine Pyramide
 */
static void drawPyramid() {
    drawNormals(0.5f, 0, 1.0f);
    drawNormals(1.0f, 0.0f, 0.5f);
    drawNormals(0.0f, 1, -1);
    drawNormals(-1, 0.5f, 0.5f);

    glBegin(GL_TRIANGLES);

    // Vorne
    glNormal3f(0.5f, 0.1f, 1.0);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);


    // Rechts
    glNormal3f(1.0f, 0.0f, 0.5f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // Hinten
    glNormal3f(0.0, 1.0, -1.0);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    // Links
    glNormal3f(-1.0, 0.5, 0.5);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();
}

/**
 * Zeichnet gerade Striche (z.B. fuer die Mauern)
 * @param width  - Breite (wird ignoriert bei isHorizontal == true)
 * @param heigth - Hoehe (wird ignoriert bei isHorizontal == true)
 * @param isHorizontal - True, wenn horizontal gezeichnet werden soll
 */
void drawDash(float width, float heigth, GLboolean isHorizontal) {

    float w = width;
    float h = heigth;

    if (isHorizontal) {
        w = w < 0.00001f ? 1 : w;
        h = h < 0.00001f ? FUGUE_WIDTH : h;
    }

    glPushMatrix();
    {
        glScalef(w, h, 1.0f);
        drawSquare();
    }
    glPopMatrix();
}

/**
 * Zeichnet den freien Block
 */
void drawFreeBlock() {
    glColor3f(0.663f, 0.663f, 0.663f);
    setMaterialLightning(0.663f, 0.663f, 0.663f);

    glPushMatrix();
    {
        glRotatef(-90, 1, 0, 0);
        glScalef(BLOCK_SIZE, BLOCK_SIZE, 1.0f);
        drawSquare();
    }

    glPopMatrix();
}

/**
 * Zeichnet die Tuer
 */
void drawDoor() {
    glColor3f(0.600f, 0.240f, 0.100f);
    setMaterialLightning(0.600f, 0.240f, 0.100f);

    glPushMatrix();
    {
        drawSquare();

        // Braun
        glColor3f(0.1f, 0.1f, 0.1f);
        setMaterialLightning(0.1f, 0.1f, 0.1f);

        // Horizontal
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.0, 0.001f);
            drawDash(0, 0, GL_TRUE);
        }
        glPopMatrix();
    }
    glPopMatrix();
}

/**
 * Zeichnet die Tür und die Fenster des Hauses
 */
void drawHouseFront() {
    glColor3f(houseColors[0], houseColors[1], houseColors[2]);
    setMaterialLightning(houseColors[0], houseColors[1], houseColors[2]);

    // Tür
    glPushMatrix();
    {
        glTranslatef(0, -0.34, 0.03f);
        glScalef(0.18f, 0.3f, 0.03f);
        drawSquare();
        glColor3f(1, 1, 1);
        setMaterialLightning(1, 1, 1);
    }
    glPopMatrix();

    // Fenster
    for (int i = 0; i < 2; ++i) {
        float x = i == 0 ? -0.3f : 0.3f;
        glPushMatrix();
        {
            glColor3f(0.529f, 0.808f, 0.922f);
            setMaterialLightning(0.529f, 0.808f, 0.922f);

            glTranslatef(x, 0.16, 0.003);
            glScalef(0.24f, 0.24f, 0.015f);

            drawSquare();
        }
        glPopMatrix();
    }
    glColor3f(1, 1, 1);
    setMaterialLightning(1, 1, 1);
}

/**
 * Zeichnen der einzelnen Seiten des Quaders
 * @param type Typ des fieldtypes
 * @param specialCase Sonderbehandlung, z.B. Tür an Hauswand
 */
void paintCube(const pushyFieldType *type, GLboolean specialCase) {

   if ((*type) == P_DOOR) {

        /* Tuer drehen, damit Schlitz an der richtigen Stelle ist */
        if (specialCase == 1) {
            glRotatef(90, 0, 0, 1);
        }

        drawDoor();
    } else if ((*type) == P_HOUSE) {

        if (specialCase == 1) {
            drawHouseFront();
        }
        drawSquare();
    } else {
        drawSquare();
    }
}

/**
 * Zeichnet einen Cube
 * @param type - Typ des Feldes (z.B. P_WALL)
 */
void
drawCube(pushyFieldType type) {

    if (type == P_DOOR) {
        drawFreeBlock();
    }

    glPushMatrix();
    {
        glTranslatef(0, 0.11f, 0);

        if (type == P_DOOR) {
            glScalef(0.1f, BLOCK_SIZE, BLOCK_SIZE);
        } else {
            glScalef(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
        }

        /* Frontflaeche */
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.0f, 0.5f);
            paintCube(&type, GL_FALSE);
        }
        glPopMatrix();

        /* rechte Seitenflaeche */
        glPushMatrix();
        {
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, 0.5f);

            paintCube(&type, GL_TRUE);
        }
        glPopMatrix();

        /* Rueckseitenflaeche */
        glPushMatrix();
        {
            glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, 0.5f);
            paintCube(&type, GL_FALSE);
        }
        glPopMatrix();

        /* linke Seitenflaeche */
        glPushMatrix();
        {
            glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, 0.5f);
            paintCube(&type, GL_TRUE);
        }
        glPopMatrix();

        /* Deckelflaeche */
        glPushMatrix();
        {
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, 0.5f);
            paintCube(&type, GL_FALSE);
        }
        glPopMatrix();

        /* Bodenflaeche */
        glPushMatrix();
        {
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, 0.5f);
            paintCube(&type, GL_FALSE);
        }
        glPopMatrix();
    }
    glPopMatrix();
}

/**
 * Zeichnet das Haus
 */
void drawHouse() {
    drawFreeBlock();
    glColor3f(1.0f, 1.0f, 1.0f);
    setMaterialLightning(1, 1, 1);

    glPushMatrix();
    {
        glPushMatrix();
        {
            // Hauskoerper
            glScalef(0.8f, 1, 0.8);
            drawCube(P_HOUSE);
        }
        glPopMatrix();

        // Dach
        glPushMatrix();
        {
            glTranslatef(0, 0.305, 0.0f);
            glScalef(0.18f, 0.18f, 0.18f);
            glColor3f(houseColors[0], houseColors[1], houseColors[2]);
            setMaterialLightning(houseColors[0], houseColors[1], houseColors[2]);
            drawPyramid();
        }
        glPopMatrix();

    }
    glPopMatrix();
}
