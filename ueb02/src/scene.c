
#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>

#endif

#include <time.h>
#include "debug.h"
#include "types.h"
#include "logic.h"
#include "helper.h"
#include "drawObjects.h"
#include "scene.h"

GLuint g_renderObjects;
GLboolean showWireframe = GL_FALSE;

void initDisplayList() {
    // Alle benoetigten Listen auf einmal erzeugen
    // TODO: 15 dynamisch
    g_renderObjects = glGenLists((GLsizei) 15);
    if (g_renderObjects != 0) {
        for (GLuint index = 0; index < 15; index++) {
            // Displayliste bereit machen
            glNewList(g_renderObjects + index, GL_COMPILE);

            switch (index) {
                case (P_FREE):
                case (P_START):
                    drawFreeBlock();
                    break;

                case (P_WALL):
                    drawWall();
                    break;

                case (P_BOX):
                    drawBox();
                    break;

                case (P_DOOR):
                    drawDoor();
                    break;

                case (P_DOOR_SWITCH):
                    drawDoorSwitch();
                    break;

                case (P_TARGET):
                    drawFinish();
                    break;

                case (P_OBJECT_TRIANGLE):
                    drawTriangleOject();
                    break;

                case (P_HOUSE):
                    drawHouse();
                    break;
                case (P_PLAYER):
                    drawPlayer();
                    break;

                default:
                    break;
            }
            // Displayliste abschliessen
            glEndList();
        }
    } else {
        CG_ERROR(("Konnte Displaylisten nicht erzeugen\n"));
    }
}

void showPlayer(int x, int y) {
    if (getGame()->levelSettings.playerPosX == x && getGame()->levelSettings.playerPosY == y) {
        glCallList(g_renderObjects + P_PLAYER);
    }
}

void drawLevel(int levelID) {
    Levels *levels = getLevels();
    glPushMatrix();
    {
        /* Skalierung und Positionierung des Spielfelds. */
        glTranslatef(0.0f, 0.1f, 0.0f);
        glScalef(0.9f, 0.9f, 1.0f);

        for (int y = 0; y < LEVEL_SIZE; y++) {
            for (int x = 0; x < LEVEL_SIZE; x++) {

                glPushMatrix();
                {
                    // Spielfeld faengt bei -1 (x) und 1 (y) an
                    float correctX = -1.0f;
                    float correctY = 1.0f;

                    float xPos = (correctX + (float) x * BLOCK_SIZE) + BLOCK_SIZE / 2;
                    float yPos = correctY - ((float) y * BLOCK_SIZE);

                    int level = levels[levelID]->field[y][x];

                    /* Kachel an Position x,y zeichnen. */
                    glTranslatef(xPos, yPos, 0.0f);

                    switch (level) {
                        case (P_FREE):
                        case (P_START):
                            glCallList(g_renderObjects + P_FREE);
                            break;

                        case (P_WALL):
                            glCallList(g_renderObjects + P_WALL);
                            break;

                        case (P_BOX):
                            glCallList(g_renderObjects + P_BOX);
                            break;

                        case (P_DOOR):
                            glCallList(g_renderObjects + P_DOOR);
                            break;

                        case (P_DOOR_SWITCH):
                            glCallList(g_renderObjects + P_DOOR_SWITCH);
                            break;

                        case (P_TARGET):
                            glCallList(g_renderObjects + P_TARGET);
                            break;

                        case (P_OBJECT_TRIANGLE):
                            glCallList(g_renderObjects + P_OBJECT_TRIANGLE);
                            break;
                        case (P_PORTAL):
                            drawPortals();
                            break;

                        case (P_HOUSE):
                            glCallList(g_renderObjects + P_HOUSE);
                            break;

                        default:
                            break;
                    }

                    showPlayer(x, y);
                }
                glPopMatrix();
            }
        }
    }
    glPopMatrix();
}

void drawGame(int levelId) {
    glPushMatrix();
    {
        // Spielfeld ist ein wenig zu groß, also um 20% kleiner machen
        // TODO: WIEDER NORMAL
        // glScalef(3.8f, 3.8f, 1.0f);
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

    initDisplayList();

    initLevel(0);

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

