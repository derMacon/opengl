
#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>

#endif

#include <time.h>
#include "debug.h"
#include "types.h"
#include "drawWater.h"
#include "io.h"
#include "logic.h"
#include "helper.h"
#include "scene.h"
#include "stringOutput.h"
#include "texture.h"
#include "drawObjects.h"

GLuint g_renderObjects;


/**
 * Setzt die Weltlicht-Position
 */
void setWorldLightPos() {
    float pos[] = {0, 3, 0, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

/**
 * Initialisiert das Weltlicht
 */
static void initWorldLight(void) {

    glEnable(GL_LIGHTING);
    float value = 1.5f;

    /* Globales Licht */
//    float globalAmbient[] = {0.7f, 0.7f, 0.7f, 1.0f};
//    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

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
 * Setzt das Materiallicht fuer die gezeichnetetn Objekte
 * @param r - rot
 * @param g - gruen
 * @param b - blau
 */
void setMaterialLightning(float r, float g, float b) {
    /* Verringert die Saetting der Farben, sodass nur noch x% angzeigt werden */
    float multiplier = 0.15f;

    float matDiffuse[] = {r, g, b, 1};
    float matAmbient[] = {1 * multiplier, 1 * multiplier, 1 * multiplier, 1.0f};
    float matSpecular[] = {r, g, b, 1.0f};
    float matShininess = 20;

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininess);
}

/**
 *  Zeichnen des gesamten Levels
 */
void drawLevel() {

    glPushMatrix();
    {

        drawBoat(GL_TRUE);
        drawBoat(GL_FALSE);

        drawLighthouse();
        glScalef(5, 5, 5);
        if (getState()->settings.showSpheres) {
            drawSpheres();
        }

        glColor3f(1, 1, 1);

        if (getState()->settings.showTextures) {
            glEnable(GL_TEXTURE_2D);
        } else {
            glEnable(GL_COLOR_MATERIAL);
        }

        bindTexture(texWater);
        drawWater();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_COLOR_MATERIAL);
    }
    glPopMatrix();
}

/**
 * Skalierung des Spielfelds und Spiel zeichnen
 */
void drawGame() {

    glPushMatrix();
    {
        setCamera();

        /* Taschenlampe setzen */
        if (getState()->settings.showSpotLight) {
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
    Status gameStatus = getState()->gameStatus;

    if (gameStatus == GAME_HELP) {
        drawHelp();
    } else {
        drawGame();
    }
}

void initMatLighting() {
    float r = 1;
    float g = 1;
    float b = 1;
    float multiplier = 0.15f;
    float matDiffuse[] = {r, g, b, 1};
    float matAmbient[] = {1 * multiplier, 1 * multiplier, 1 * multiplier, 1.0f};
    float matSpecular[] = {r, g, b, 1.0f};
    float matShininess = 20;

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininess);
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
    glEnable(GL_TEXTURE_2D);

    initLight();
    initLevel();
    initMatLighting();


    // Zeit fuer Random einbeziehen, um Zufallsfarben zu generieren
    // srand steht hier, weil es nur einmal aufgerufen werden darf
    srand(time(NULL));

    /* Z-Buffer-Test aktivieren */
    glEnable(GL_DEPTH_TEST);

    /* Polygonrueckseiten nicht anzeigen */
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    /* Alles in Ordnung? */
    return (GLGETERROR == GL_NO_ERROR);
}
