
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
#include "levels.h"
#include "stringOutput.h"
#include <math.h>

GLuint g_renderObjects;
GLboolean showWireframe = GL_FALSE;

/**
 *  Initialisiert die Displaylisten
 */
void initDisplayList() {

    int numberOfTypes = (P_PLAYER - P_OUTER) + 1;

    // Alle benoetigten Listen auf einmal erzeugen
    g_renderObjects = glGenLists((GLsizei) numberOfTypes);
    if (g_renderObjects != 0) {
        for (GLuint index = 0; index < numberOfTypes; index++) {
            // Displayliste bereit machen
            glNewList(g_renderObjects + index, GL_COMPILE);

            switch (index) {
                case (P_FREE):
                    drawFreeBlock();
                    break;

                case (P_WALL):
                    drawCube(P_WALL);
                    break;

                case (P_BOX):
                    drawCube(P_BOX);
                    break;

                case (P_DOOR):
                    //drawDoor();
                    drawCube(P_DOOR);
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

/**
 *  Spieler an Position anzeigen
 * @param x x Pos des Spielers
 * @param y y Pos des Spielers
 */
void showPlayer(int x, int y) {
    if (getGame()->levelSettings.playerPosX == x && getGame()->levelSettings.playerPosY == y) {
        glCallList(g_renderObjects + P_PLAYER);
    }
}

/**
 *  Zeichnen des gesamten Levels
 */
void drawLevel(GLboolean draw3D) {
    pushyFieldType (*level)[LEVEL_SIZE] = getGame()->levelSettings.level;

    glPushMatrix();
    {
        /* Skalierung und Positionierung des Spielfelds. */
        glTranslatef(0.0f, 0.1f, 0.0f);
        //todo maybe ändern
//        glScalef(0.9f,1.0f, 0.9f);

        for (int y = 0; y < LEVEL_SIZE; y++) {
            for (int x = 0; x < LEVEL_SIZE; x++) {

                glPushMatrix();
                {
                    // Spielfeld faengt bei -1 (x) und 1 (y) an
                    float correctX = -1.0f;
                    float correctY = 1.0f;

                    float xPos = (correctX + (float) x * BLOCK_WIDTH) + BLOCK_WIDTH / 2;
                    float yPos = correctY - ((float) y * BLOCK_WIDTH);

                    int levelField = level[y][x];

                    /* Kachel an Position x,y zeichnen. */
//                    if (draw3D) {
                        glTranslatef(-xPos, 0.0f, yPos);
//                    } else {
//                        glTranslatef(xPos, yPos, 0);
//                    }

                    switch (levelField) {
                        case (P_FREE):
                        case (P_WALL):
                        case (P_DOOR):
                        case (P_DOOR_SWITCH):
                        case (P_OBJECT_TRIANGLE):
                        case (P_TARGET):
                        case (P_BOX):
                            glCallList(g_renderObjects + levelField);
                            break;

                        case (P_BOX_DOOR_SWITCH):
                            glCallList(g_renderObjects + P_BOX);
                            break;
                        case (P_HOUSE):
                            drawHouse();
                            break;

                        case (P_PORTAL):
                            drawPortals();
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

/**
 * Skalierung des Spielfelds und Spiel zeichnen
 */
void drawGame(GLboolean draw3D) {

    glPushMatrix();
    {
        //todo maybe ändern

        // Spielfeld ist ein wenig zu groß, also um 20% kleiner machen
        //glScalef(1.0f, 1.0f,1.0f);

        if (draw3D) {
            // Spielfeld ist ein wenig zu hoch, also bisschen tiefer setzen
            glTranslatef(0.0f, -0.1f, 0.0f);
        } else {
            // TODO mal sehen was hier geht
            glRotatef(90,1,0,0);
        }

        GLfloat radius = getGame()->camera.radius;
        GLfloat polar = TO_RADIANS(getGame()->camera.polarAngle);
        GLfloat azimuth = TO_RADIANS(getGame()->camera.azimuthAngle);

        GLfloat eyeX = radius * sinf(azimuth) * cosf(polar);
        GLfloat eyeY = radius * cosf(azimuth);
        GLfloat eyeZ = radius * sinf(azimuth) * sinf(polar);

        if (draw3D) {
            gluLookAt(eyeX, eyeY, eyeZ,
                      0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0);
        }

        // Uebergebenes Level zeichnen
        drawLevel(draw3D);
    }
    glPopMatrix();

}

/**
 * Zeichnen der Info des Spiels
 */
void drawGameInfo() {
    int timeLeft = getGame()->levelSettings.time;
    int trianglesLeft = getGame()->levelSettings.numberOfTriangles;
    GLfloat textColor[3] = {1, 1, 1};

    // Langsam rot faerben
    if (timeLeft <= 10) {
        textColor[0] = 1;
        textColor[1] = (float) timeLeft / 10.0f;
        textColor[2] = (float) timeLeft / 10.0f;
    }

    drawString(0.14, 0.87, textColor, "Level %d | Verbleibende Dreiecke: %d | Verbleibende Zeit: %d",
               getGame()->levelId, trianglesLeft,
               timeLeft);
}

/**
 * Zeichnen des Lost Screens
 */
void drawLost() {
    GLfloat textColor[3] = {1, 1, 1};
    drawString(0.25f, 0.5f, textColor, "Du hast verloren. Dir fehlten noch %d Dreiecke.",
               getGame()->levelSettings.numberOfTriangles);
}

/**
 * Hilfe zeichnen
 */
void drawHelp() {
    GLfloat textColor[3] = {1, 1, 1};
    drawString(0.45f, 0.35f, textColor, "H I L F E ");

    drawString(0.2f, 0.45f, textColor, "q, Q, ESC                              Spiel beenden");
    drawString(0.2f, 0.50f, textColor, "r, R                                         Level neustarten");
    drawString(0.2f, 0.55f, textColor, "h, H                                        Hilfe anzeigen");
    drawString(0.2f, 0.60f, textColor, "F1                                          Wireframe an/aus");
    drawString(0.2f, 0.65f, textColor, "F2                                          Vollbildmodus an/aus");
    drawString(0.2f, 0.70f, textColor, "p, P                                        Pause an/aus");
    drawString(0.2f, 0.75f, textColor, "1, 2, 3                                     Direkter Levelzugriff");
}

/**
 * Spielpause zeichnen
 */
void drawPause() {
    GLfloat textColor[3] = {1, 1, 1};
    drawString(0.45f, 0.4f, textColor, "P A U S E");
    drawString(0.2f, 0.45f, textColor, "Dir fehlen noch %d Dreiecke und du hast noch %d Sekunden Zeit.",
               getGame()->levelSettings.numberOfTriangles, getGame()->levelSettings.time);
}

/**
 * Zeichnen des Winning screens
 */
void drawWon() {
    GLfloat textColor[3] = {1, 1, 1};
    int timeNeeded = levels[getGame()->levelId - 1].time - getGame()->levelSettings.time;

    drawString(0.25f, 0.5f, textColor, "Du hast gewonnen und %d Sekunden gebraucht", timeNeeded);

    if (getGame()->levelId == NUMBER_OF_LEVEL) {
        drawString(0.25f, 0.55f, textColor, "Beliebe Taste zum Neustart des Spiels druecken.");
    }
}

/**
 * Zeichnet die gesamte Szene
 */
void drawScene(GLboolean draw3D) {
    Gamestatus gameStatus = getGame()->gameStatus;

    if (gameStatus == GAME_RUNNING) {
        drawGame(draw3D);

        if (draw3D) {
            drawGameInfo();
        }
    } else if (draw3D && gameStatus == GAME_LOST) {
        drawLost();
    } else if (draw3D && gameStatus == GAME_WON) {
        drawWon();
    } else if (draw3D && gameStatus == GAME_PAUSED) {
        drawPause();
    } else if (draw3D && gameStatus == GAME_HELP) {
        drawHelp();
    }
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

    initLevel(1);

    // Zeit fuer Random einbeziehen, um Zufallsfarben zu generieren
    // srand steht hier, weil es nur einmal aufgerufen werden darf
    srand(time(NULL));


    /* Z-Buffer-Test aktivieren */
    glEnable(GL_DEPTH_TEST);

    /* Polygonrueckseiten nicht anzeigen */
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

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

