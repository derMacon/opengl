
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
 * Setzt die Weltlicht-Position
 */
void setWorldLightPos() {
    float pos[] = {0, 4, 0, 2};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

/**
 * Initialisiert das Weltlicht
 */
static void initWorldLight(void) {

    float value = 0.5f;

    /* Globales Licht */
    float globalAmbient[] = {0.3f, 0.2f, 0.4f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    /* Weltlicht */
    float colorDiffus[] = {value, value, value, value};
    float colorSpecular[] = {value, value, value, value};

    setWorldLightPos();

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
 *  Zeichnen des gesamten Levels
 */
void drawLevel() {

    glPushMatrix();
    {
        /* Skalierung und Positionierung des Spielfelds. */
        glTranslatef(0.0f, 0.1f, 0.0f);
        drawHouse();
    }
    glPopMatrix();
}

/**
 * Skalierung des Spielfelds und Spiel zeichnen
 */
void drawGame() {

    glPushMatrix();
    {

        /* "Third" Person Ansicht */
        GLfloat radius = getGame()->camera.radius;
        GLfloat angleHorizontal = TO_RADIANS(getGame()->camera.angleHorizontal);
        GLfloat angleVertical = TO_RADIANS(getGame()->camera.angleVertical);

        // Kamera einstellen
        GLfloat camX = radius * sinf(angleVertical) * cosf(angleHorizontal);
        GLfloat camY = radius * cosf(angleVertical);
        GLfloat camZ = radius * sinf(angleVertical) * sinf(angleHorizontal);

        gluLookAt(camX, camY, camZ,
                  0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0);


        /* Taschenlampe setzen */
        if (getGame()->settings.showSpotLight) {
            /* Taschenlampe setzen */
            // playerx bei oben und unten -> versetzt links und rechts
            // playery bei links rechts -> versetzt oben und unten
            // y wert = Hoehe
            // float spotLightPos[] = {4 - 0.1f, 0.2f, 4, 1}; //todo leuchtturm
            //float spotlightDirection[] = {setFirstPersonView(GL_TRUE), 0, setFirstPersonView(GL_FALSE)};

            // glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotlightDirection);
            // glLightfv(GL_LIGHT1, GL_POSITION, spotLightPos);
        }

        /* Weltlicht Pos erneut setzen,
         * sonst wuerde die Kameradrehung die Position falsch setzen */
        setWorldLightPos();
        drawLevel();
    }
    glPopMatrix();
}

/**
 * Zeichnen der Info des Spiels
 */
void drawGameInfo() {
    GLfloat textColor[3] = {1, 1, 1};

    drawString(0.4f, 0.8, textColor, "Level %d | Dreiecke: %d | Verbleibende Zeit: %d", 3, 3, 3);

}

/**
 * Hilfe zeichnen
 */
void drawHelp() {
    GLfloat textColor[3] = {1, 1, 1};

    drawString(0.4f, 0.10f, textColor, "H I L F E ");

    drawString(0.2f, 0.20f, textColor, "q, Q, ESC                              Spiel beenden");
    drawString(0.2f, 0.25f, textColor, "r, R                                         Level neustarten");
    drawString(0.2f, 0.30f, textColor, "h, H                                        Hilfe anzeigen");
    drawString(0.2f, 0.35f, textColor, "c, A                                        FirstPerson an/aus");
    drawString(0.2f, 0.40f, textColor, "p, P                                        Pause an/aus");
    drawString(0.2f, 0.45f, textColor, "a, A                                        Animation an/aus");
    drawString(0.2f, 0.50f, textColor, "F1                                          Wireframe an/aus");
    drawString(0.2f, 0.55f, textColor, "F2                                          Normalen an/aus");
    drawString(0.2f, 0.60f, textColor, "F3                                          Weltlich an/aus");
    drawString(0.2f, 0.65f, textColor, "F4                                          Taschenlampe an/aus");
    drawString(0.2f, 0.70f, textColor, "F5                                          Vollbild an/aus");
    drawString(0.2f, 0.75f, textColor, "1, 2, 3, 4                                Direkter Levelzugriff");
}


/**
 * Zeichnet die gesamte Szene
 */
void drawScene() {
    Gamestatus gameStatus = getGame()->gameStatus;

    if (gameStatus == GAME_RUNNING) {
        drawGame();
        /* Infos, wie Zeit verbleibend nur beim 3D-Viewport anzeigen */
        drawGameInfo();

    } else if (gameStatus == GAME_HELP) {
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

    /* Normalen */
    glEnable(GL_NORMALIZE);

    initLight();
    initDisplayList();

    initLevel();

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
