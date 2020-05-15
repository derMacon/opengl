
#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>
#include <time.h>
#include "debug.h"
#include "types.h"
#include "logic.h"

#endif

GLboolean showWireframe = GL_FALSE;


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

void drawHorizontalFuge() {
    glPushMatrix();
    {
        glScalef(1, FUGE_WIDTH, 1.0f);
        drawSquare();
    }
    glPopMatrix();
}

void drawVerticaLFuge(float width, float height) {
    glPushMatrix();
    {
        glScalef(width, height, 1.0f);
        drawSquare();
    }
    glPopMatrix();
}

void drawWall(float xPos, float yPos) {

    glColor3f(0.412f, 0.412f, 0.412f);

    float width = BLOCK_SIZE - 0.01f;

    glPushMatrix();
    {
        glTranslatef(xPos, yPos, 0.0f);
        glScalef(width, BLOCK_SIZE - 0.01f, 1.0f);
        drawSquare();

        // Fugenfarbe
        glColor3f(0.663f, 0.663f, 0.663f);

        // Horizontal
        glPushMatrix();
        {
            glTranslatef(0.0f, -FUGE_HEIGHT * 2, 0.0f);
            // Horizontale Striche
            for (int i = 0; i < 3; i++) {
                glTranslatef(0.0f, FUGE_HEIGHT, 0.0f);
                drawHorizontalFuge();
            }
        }
        glPopMatrix();

        // Vertikal
        glPushMatrix();
        {
            // TODO: KOMMENTARE, kp mehr was hier abgeht :D
            // TODO: Wieso 2.5? :P
            glTranslatef(0.0f, -FUGE_HEIGHT * 2.5f, 0);
            for (int i = 0; i < 4; i++) {

                glTranslatef(0.0, FUGE_HEIGHT, 0);
                int max = i % 2 == 0 ? 3 : 2;

                glPushMatrix();
                {
                    glTranslatef(-FUGE_HEIGHT * 2, 0, 0);
                    for (int j = 0; j < max; ++j) {
                        float xVal = FUGE_HEIGHT;
                        if (max == 2) {
                            xVal = FUGE_HEIGHT + 0.1f;
                        }
                        glTranslatef(xVal, 0, 0);
                        drawVerticaLFuge(FUGE_WIDTH, FUGE_HEIGHT);
                    }
                }
                glPopMatrix();
            }
        }
        glPopMatrix();
    }

    glPopMatrix();
}

void drawBox(float xPos, float yPos) {
    glColor3f(0.600f, 0.240f, 0.100f);
    float width = BLOCK_SIZE - 0.01f;

    float bottom = -0.5f;
    float offset = 0.15f;
    float dashHeight = 0.7f;
    float dashWidth = 0.03f;
    int numberOfDashes = 7;

    glPushMatrix();
    {
        glTranslatef(xPos, yPos, 0.0f);
        glScalef(width, BLOCK_SIZE - 0.01f, 1.0f);
        drawSquare();

        // Fugenfarbe
        glColor3f(0.0f, 0.0f, 0.0f);

        // Horizontal
        glPushMatrix();
        {
            glTranslatef(0.0f, bottom + offset, 0.0f);
            // Horizontale Striche
            for (int i = 0; i < 2; i++) {
                drawHorizontalFuge();
                glTranslatef(0.0f, -(bottom + offset) * 2, 0.0f);
            }
        }
        glPopMatrix();

        // Vertikal
        glPushMatrix();
        {
            glTranslatef(-0.4f, 0.0f, 0.0f);
            // Horizontale Striche
            for (int i = 0; i < numberOfDashes; i++) {
                drawVerticaLFuge(dashWidth, dashHeight);
                glTranslatef(1.0f / (float) numberOfDashes - (dashWidth / 2), 0.0, 0.0f);
            }
        }
        glPopMatrix();

    }
    glPopMatrix();

}

void drawFreeBlock(float xPos, float yPos) {
    glColor3f(0.663f, 0.663f, 0.663f);

    glPushMatrix();
    {
        glTranslatef(xPos, yPos, 0.0f);
        glScalef(BLOCK_SIZE - 0.01f, BLOCK_SIZE - 0.01f, 1.0f);
        drawSquare();
    }

    glPopMatrix();
}

void drawLevel(int levelID) {
    Levels *levels = getLevels();

    for (int y = 0; y < LEVEL_SIZE; y++) {
        for (int x = 0; x < LEVEL_SIZE; x++) {

            // Spielfeld faengt bei -1 (x) und 1 (y) an
            float correctX = -1.0f;
            float correctY = 1.0f;

            float xPos = (correctX + (float) x * BLOCK_SIZE) + BLOCK_SIZE / 2;
            float yPos = correctY - ((float) y * BLOCK_SIZE);

            int level = levels[levelID]->field[y][x];

            switch (level) {
                case (P_FREE):
                    drawFreeBlock(xPos, yPos);
                    break;

                case (P_WALL):
                    drawWall(xPos, yPos);
                    break;

                case (P_BOX):
                    drawBox(xPos, yPos);
                    break;

                default:
                    break;
            }
        }
    }
}

void drawGame(int levelId) {
    glPushMatrix();
    {
        // Spielfeld ist ein wenig zu groß, also um 20% kleiner machen
        glScalef(0.8f, 0.8f, 1.0f);

        // Spielfeld ist ein wenig zu hoch, also bisschen tiefer setzen
        glTranslatef(0.0f, -0.1f, 0.0f);

        // Uebergebenes Level zeichnen
        drawLevel(levelId);
    }
    glPopMatrix();
}


/**
 * Zeichnet die gesamte Szene (Grenzen, Ball, Stick und evtl. Extras
 */
void drawScene(void) {
    drawGame(getGame()->levelId);
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
 * Schaltet den Fullscreen an oder aus
 */
void toggleFullscreen() {
    showFullscreen = !showFullscreen;

    if (showFullscreen) {
        glutFullScreen();
    } else {
        glutPositionWindow(glutGet(GLUT_SCREEN_WIDTH) / 2, glutGet(GLUT_SCREEN_HEIGHT) / 2);
    }
}

