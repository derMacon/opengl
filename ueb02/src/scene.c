
#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include "debug.h"
#include "types.h"
#include "logic.h"

#endif

GLboolean showWireframe = GL_FALSE;
float shrinkVal = 0;
GLboolean isIncreasing = GL_TRUE;

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

static void drawTriangle() {
    glBegin(GL_TRIANGLES);

    float x = 0.7f;
    glColor3f(0.137f, 0.137f, 0.557f);

    // Links unten
    glVertex2f(-x, -x);

    // Rechts unten
    glVertex2f(x, -x);

    // oben
    glVertex2f(0, x);

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

void drawHorizontalDash() {
    glPushMatrix();
    {
        glScalef(1, FUGUE_WIDTH, 1.0f);
        drawSquare();
    }
    glPopMatrix();
}

void drawVerticalDash(float width, float height) {
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
            glTranslatef(0.0f, -FUGUE_HEIGHT * 2, 0.0f);
            // Horizontale Striche
            for (int i = 0; i < 3; i++) {
                glTranslatef(0.0f, FUGUE_HEIGHT, 0.0f);
                drawHorizontalDash();
            }
        }
        glPopMatrix();

        // Vertikal
        glPushMatrix();
        {
            // TODO: KOMMENTARE, kp mehr was hier abgeht :D
            // TODO: Wieso 2.5? :P
            glTranslatef(0.0f, -FUGUE_HEIGHT * 2.5f, 0);
            for (int i = 0; i < 4; i++) {

                glTranslatef(0.0, FUGUE_HEIGHT, 0);
                int max = i % 2 == 0 ? 3 : 2;

                glPushMatrix();
                {
                    glTranslatef(-FUGUE_HEIGHT * 2, 0, 0);
                    for (int j = 0; j < max; ++j) {
                        float xVal = FUGUE_HEIGHT;
                        if (max == 2) {
                            xVal = FUGUE_HEIGHT + 0.1f;
                        }
                        glTranslatef(xVal, 0, 0);
                        drawVerticalDash(FUGUE_WIDTH, FUGUE_HEIGHT);
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

    glPushMatrix();
    {
        glTranslatef(xPos, yPos, 0.0f);
        glScalef(width, BLOCK_SIZE - 0.01f, 1.0f);
        drawSquare();

        // Fugenfarbe
        glColor3f(0.1f, 0.1f, 0.1f);

        // Horizontal
        glPushMatrix();
        {
            glTranslatef(0.0f, bottom + offset, 0.0f);
            // Horizontale Striche
            for (int i = 0; i < 2; i++) {
                drawHorizontalDash();
                glTranslatef(0.0f, -(bottom + offset) * 2, 0.0f);
            }
        }
        glPopMatrix();

        // Vertikal
        glPushMatrix();
        {
            glTranslatef(-0.4f, 0.0f, 0.0f);
            for (int i = 0; i < BOX_NUMBER_OF_COLS; i++) {
                drawVerticalDash(BOX_DASH_WIDTH, BOX_DASH_HEIGHT);
                glTranslatef(1.0f / (float) BOX_NUMBER_OF_COLS - (BOX_DASH_WIDTH / 2), 0.0, 0.0f);
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

void drawDoor(float xPos, float yPos) {
    float width = BLOCK_SIZE - 0.3f;
    drawFreeBlock(xPos, yPos);
    glColor3f(0.600f, 0.240f, 0.100f);

    glPushMatrix();
    {
        glTranslatef(xPos, yPos, 0.0f);
        glScalef(width, BLOCK_SIZE - 0.01f, 1.0f);
        drawSquare();

        // Fugenfarbe
        glColor3f(0.1f, 0.1f, 0.1f);

        // Horizontal
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.0, 0.0f);
            drawHorizontalDash();

        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawTriangleOject(float xPos, float yPos) {
    float width = BLOCK_SIZE - 0.3f;
    drawFreeBlock(xPos, yPos);

    glPushMatrix();
    {
        glTranslatef(xPos, yPos, 0.0f);
        glScalef(width, BLOCK_SIZE / 2, 1.0f);
        glRotatef(90, 0.0f, 0.0f, 1.0f);
        drawTriangle();
    }
    glPopMatrix();
}

void drawFinish(float xPos, float yPos) {
    glColor3f(0.137f, 0.137f, 0.557f);

    glPushMatrix();
    {
        glTranslatef(xPos, yPos, 0.0f);
        glScalef(BLOCK_SIZE - 0.01f, BLOCK_SIZE - 0.01f, 1.0f);
        drawSquare();
    }

    glPopMatrix();
}

void drawDoorSwitchArrow() {
    float width = BLOCK_SIZE - 0.3f;

    double arrowHeadWidth = width / 1.5;
    double arrowHeadHeight = BLOCK_SIZE / 4;
    double arrowBodyWidth = arrowHeadWidth / 2;
    double arrowBodyHeight = arrowHeadHeight / 2;

    glPushMatrix();
    {
        // Arrow Head
        glPushMatrix();
        {
            glTranslatef(0, 0.05f, 0);
            glScalef(arrowHeadWidth, arrowHeadHeight, 1.0f);
            drawTriangle();
        }
        glPopMatrix();

        // Arrow Body
        glPushMatrix();
        {
            glTranslatef(0.0, -0.009f, 0);
            glScalef(arrowBodyWidth, arrowBodyHeight * 1.5f, 1.0f);
            drawSquare();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawDoorSwitch(float xPos, float yPos) {
    drawFreeBlock(xPos, yPos);

    glPushMatrix();
    {
        glTranslatef(xPos + 0.03f, yPos + 0.03f, 0.0f);
        glRotatef(-45, 0, 0, 1);
        drawDoorSwitchArrow();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(xPos - 0.03f, yPos - 0.03f, 0.0f);
        glRotatef(-225, 0, 0, 1);
        drawDoorSwitchArrow();
    }
    glPopMatrix();
}

/**
 * Waehlt anhand einer Zufallszahl eine Farbkombination aus.
 * Wird fuer die Bloecke verwendet, um diese ansehnlicher zu mache
 * @param index Zahl zwischen 1 - 5
 * @return Array mit RGB-Farben
 */
float *selectColor(int index) {

    float *colors = malloc(3);

    switch (index) {
        // Rot
        case 0:
            colors[0] = 1.0f;
            colors[1] = 0;
            colors[2] = 0;
            break;

        case 1:
            // Braun
            colors[0] = 1.0f;
            colors[1] = 0.647f;
            colors[2] = 0;
            break;

            // Gelb
        case 2:
            colors[0] = 0.941f;
            colors[1] = 0.902f;
            colors[2] = 0.549f;
            break;

            // Gruen
        case 3:
            colors[0] = 0.196f;
            colors[1] = 0.804f;
            colors[2] = 0.196f;
            break;

            // Blau
        case 4:
            colors[0] = 0.137f;
            colors[1] = 0.137f;
            colors[2] = 0.557f;
            break;
    }

    return colors;
}

void drawPortals(float xPos, float yPos) {
    drawFreeBlock(xPos, yPos);
    float size = 0.1f;
    float portalSize;
    float shrinkInterval = 15.0f;

    portalSize = 0 + shrinkVal;

    if (isIncreasing) {
        shrinkVal += 0.1f / shrinkInterval;
    } else {
        shrinkVal -= 0.1f / shrinkInterval;
    }

    if (portalSize <= 0) {
        isIncreasing = GL_TRUE;
    } else if (portalSize >= 1) {
        isIncreasing = GL_FALSE;
    }

    glPushMatrix();
    {
        glTranslatef(xPos, yPos, 0.0f);
        glScalef(portalSize, portalSize, 0);

        for (int i = 0; i < 5; ++i) {
            float *colors = selectColor(i);
            glColor3f(colors[0], colors[1], colors[2]);

            glPushMatrix();
            {
                glScalef(size, size, 0);
                drawCircle();
                size -= 0.02f;
            }

            glPopMatrix();
            free(colors);
        }

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
                case (P_START):
                    drawFreeBlock(xPos, yPos);
                    break;

                case (P_WALL):
                    drawWall(xPos, yPos);
                    break;

                case (P_BOX):
                    drawBox(xPos, yPos);
                    break;

                case (P_DOOR):
                    drawDoor(xPos, yPos);
                    break;

                case (P_DOOR_SWITCH):
                    drawDoorSwitch(xPos, yPos);
                    break;

                case (P_FINISH):
                    drawFinish(xPos, yPos);
                    break;

                case (P_OBJECT_TRIANGLE):
                    drawTriangleOject(xPos, yPos);
                    break;
                case (P_PORTAL):
                    drawPortals(xPos, yPos);
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

