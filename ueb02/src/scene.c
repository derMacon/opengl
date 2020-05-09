
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

void drawLevel(int levelID) {
    Levels *test = getLevels();

    for (int i = 0; i < LEVELSIZE; ++i) {
        for (int j = 0; j < LEVELSIZE; ++j) {
            printf("x: %d, y: %d :  %d\n", i, j, test[levelID - 1]->field[i][j]);
        }
    }
}

/**
 * Zeichnet die gesamte Szene (Grenzen, Ball, Stick und evtl. Extras
 */
void drawScene(void) {
    drawLevel(getGame()->levelId);
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

    drawScene();

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

