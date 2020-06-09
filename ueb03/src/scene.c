
#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>

#endif

#include <time.h>
#include "debug.h"
#include "types.h"
#include "io.h"
#include "logic.h"
#include "helper.h"
#include "drawObjects.h"
#include "scene.h"
#include "levels.h"
#include "stringOutput.h"
#include <math.h>

GLuint g_renderObjects;

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
        if (getGame()->settings.showAnimation) {
            glTranslatef(0, 0, (float) ((getGame()->movementCooldown / 10) / COOLDOWN_TIME));
        }
        glCallList(g_renderObjects + P_PLAYER);
    }
}

/**
 * Initialisiert das Weltlicht
 */
static void initWorldLight(void) {

    float one = 1.0f;

    /* Globales Licht */
    float globalAmbient[] = {one, one, one, one};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    /* Weltlicht */
    float pos[] = {2.0f, 0.5f, 0.0f, one};
    float colorDiffus[] = {one, one, one, one};
    float colorSpecular[] = {one, one, one, one};

    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, colorDiffus);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, colorDiffus);
    glLightfv(GL_LIGHT0, GL_SPECULAR, colorSpecular);
    glEnable(GL_LIGHT0);
}

/**
 * Initialisiert das Taschenlampen-Licht
 */
static void initSpotLight(void) {
    float one = 1.0f;

    /* Tachenlampe (Spotlight) */
    float colorDiffus[] = {one, one, 0.9f, one};
    float colorSpecular[] = {one, one, one, one};

    glLightfv(GL_LIGHT1, GL_SPECULAR, colorSpecular);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, colorDiffus);
    glLightfv(GL_LIGHT1, GL_AMBIENT, colorDiffus);
    glLighti(GL_LIGHT1, GL_SPOT_CUTOFF, 40);
    glLighti(GL_LIGHT1, GL_SPOT_EXPONENT, 15);
}

/**
 * Laesst das Welt- und Taschenlampen-Licht initialisieren
 */
static void initLight(void) {
    initWorldLight();
    initSpotLight();
}

/**
 * Dreht den Spieler in die gewaehlte Richtung
 */
void changePlayerDirection() {

    switch (getGame()->lastDirection) {
        case dirUp:
            glRotatef(180, 0, 1, 0);
            break;
        case dirDown:
        case dirNone:
            break;
        case dirLeft:
            glRotatef(-90, 0, 1, 0);
            break;
        case dirRight:
            glRotatef(90, 0, 1, 0);
            break;
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

        if (!draw3D) {
            /* Minimap ist initial spiegelverkehr, also nochmal drehen */
            glRotatef(180, 0, 1, 0);
        }

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
                    glTranslatef(-xPos, 0.0f, yPos);

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

                    /* Spieler zeichnen */
                    changePlayerDirection();
                    showPlayer(x, y);
                }
                glPopMatrix();
            }
        }
    }
    glPopMatrix();
}

/**
 * Hilft dabei, die Sichtrichtung in der Firstperson-Ansicht zu aendern
 * @param isX - links oder rechts
 * @return die (evtl) veraenderte Variable
 */
float setFirstPersonView(GLboolean isX) {

    enum e_Direction lastDir = getGame()->lastDirection;

    if (lastDir == dirNone) {
        return 0;
    }

    float val = 0;

    if (isX) {
        if (lastDir == dirLeft) {
            val = 1;
        } else if (lastDir == dirRight) {
            val = -1;
        }
    } else {
        if (lastDir == dirUp) {
            val = 1;
        } else if (lastDir == dirDown) {
            val = -1;
        }
    }

    return val;
}

/**
 * Skalierung des Spielfelds und Spiel zeichnen
 */
void drawGame(GLboolean draw3D) {

    float playerX = (float) getGame()->levelSettings.playerPosX;
    float playerY = (float) getGame()->levelSettings.playerPosY;

    /* Firstperson */
    float playerXOffset = (float) (LEVEL_SIZE - playerX) / LEVEL_SIZE;
    float playerYOffset = (float) (LEVEL_SIZE - playerY) / LEVEL_SIZE;

    playerX = playerXOffset - ((float) (getGame()->levelSettings.playerPosX) / LEVEL_SIZE);
    playerY = playerYOffset - ((float) (getGame()->levelSettings.playerPosY) / LEVEL_SIZE);

    glPushMatrix();
    {
        if (draw3D) {

            if (getGame()->settings.showFirstPerson) {

                // Kamera einstellen
                GLfloat eyeX = playerX - 0.12f;
                GLfloat eyeY = 0.25f;
                GLfloat eyeZ = playerY;

                if (getDirection() != dirNone) {
                    getGame()->lastDirection = getDirection();
                }

                gluLookAt(eyeX, eyeY, eyeZ,
                          eyeX + setFirstPersonView(GL_TRUE), 0, eyeZ + setFirstPersonView(GL_FALSE),
                          0.0f, 1.0f, 0.0f);
            } else {

                GLfloat radius = getGame()->camera.radius;
                GLfloat polar = TO_RADIANS(getGame()->camera.anglePolar);
                GLfloat azimuth = TO_RADIANS(getGame()->camera.angleAzimuth);

                // Kamera einstellen
                GLfloat eyeX = radius * sinf(azimuth) * cosf(polar);
                GLfloat eyeY = radius * cosf(azimuth);
                GLfloat eyeZ = radius * sinf(azimuth) * sinf(polar);

                gluLookAt(eyeX, eyeY, eyeZ,
                          0.0, 0.0, 0.0,
                          0.0, 1.0, 0.0);
            }

            /* Taschenlampe setzen */
            // playerx bei oben und unten -> versetzt links und rechts
            // playery bei links rechts -> versetzt oben und unten
            // y wert = Hoehe
            float spotLightPos[] = {playerX - 0.1f, 0.2f, playerY, 1};
            float spotlightDirection[] = {setFirstPersonView(GL_TRUE), 0, setFirstPersonView(GL_FALSE)};

            glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotlightDirection);
            glLightfv(GL_LIGHT1, GL_POSITION, spotLightPos);

        } else {
            glRotatef(90, 1, 0, 0);
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

    drawString(0.02f, 0.8, textColor, "Level %d | Dreiecke: %d | Verbleibende Zeit: %d",
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

    drawString(0.45f, 0.25f, textColor, "H I L F E ");

    drawString(0.2f, 0.35f, textColor, "q, Q, ESC                              Spiel beenden");
    drawString(0.2f, 0.40f, textColor, "r, R                                         Level neustarten");
    drawString(0.2f, 0.45f, textColor, "h, H                                        Hilfe anzeigen");
    drawString(0.2f, 0.50f, textColor, "F1                                          Wireframe an/aus");
    drawString(0.2f, 0.55f, textColor, "F2                                          Normalen an/aus");
    drawString(0.2f, 0.60f, textColor, "F3                                          Weltlich an/aus");
    drawString(0.2f, 0.65f, textColor, "F4                                          Taschenlampe an/aus");
    drawString(0.2f, 0.70f, textColor, "F5                                          Vollbild an/aus");
    drawString(0.2f, 0.75f, textColor, "p, P                                        Pause an/aus");
    drawString(0.2f, 0.80f, textColor, "1, 2, 3, 4                                 Direkter Levelzugriff");
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

        /* Weltlicht an- oder ausschalten */
        if (draw3D) {
            glEnable(GL_LIGHTING);
        } else {
            glDisable(GL_LIGHTING);
        }

        drawGame(draw3D);

        /* Infos, wie Zeit verbleibend nur beim 3D-Viewport anzeigen */
        if (!draw3D) {
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

    glEnable(GL_NORMALIZE);

    initLight();
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
