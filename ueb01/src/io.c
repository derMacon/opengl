
#include <GL/glut.h>
#include "io.h"
#include "debug.h"
#include "scene.h"

/**
 * Setzen der Projektionsmatrix.
 * Setzt die Projektionsmatrix unter Berücksichtigung des Seitenverhaeltnisses
 * des Anzeigefensters, sodass das Seitenverhaeltnisse der Szene unveraendert
 * bleibt und gleichzeitig entweder in x- oder y-Richtung der Bereich von -1
 * bis +1 zu sehen ist.
 * @param aspect Seitenverhaeltnis des Anzeigefensters (In).
 */
void
setProjection(GLdouble aspect) {

    /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
    glMatrixMode(GL_PROJECTION);

    /* Matrix zuruecksetzen - Einheitsmatrix laden */
    glLoadIdentity();

    /* Koordinatensystem bleibt quadratisch */
    if (aspect <= 1) {
        gluOrtho2D(-1.0, 1.0,    /* links, rechts */
                   -1.0 / aspect, 1.0 / aspect); /* unten, oben */
    } else {
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect,  /* links, rechts */
                   -1.0, 1.0);   /* unten, oben */
    }
}

/**
 * Zeichen-Callback.
 * Loescht die Buffer, ruft das Zeichnen der Szene auf und tauscht den Front-
 * und Backbuffer.
 */
static void
cbDisplay(void) {
    /* Buffer zuruecksetzen */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Nachfolgende Operationen beeinflussen Modelviewmatrix */
    glMatrixMode(GL_MODELVIEW);
    /* Matrix zuruecksetzen - Einheitsmatrix laden */
    glLoadIdentity();

    /* Szene zeichnen */
    drawScene();

    /* Objekt anzeigen */
    glutSwapBuffers();
}

/**
 * Callback fuer Aenderungen der Fenstergroesse.
 * Initiiert Anpassung der Projektionsmatrix an veränderte Fenstergroesse.
 * @param w Fensterbreite (In).
 * @param h Fensterhoehe (In).
 */
static void
cbReshape(int w, int h) {
    /* Das ganze Fenster ist GL-Anzeigebereich */
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    /* Anpassen der Projektionsmatrix an das Seitenverhältnis des Fensters */
    setProjection((GLdouble) w / (GLdouble) h);
}

/**
 * Registrierung der GLUT-Callback-Routinen.
 */
void registerCallbacks(void) {
    /* Reshape-Callback - wird ausgefuehrt, wenn neu gezeichnet wird (z.B. nach
     * Erzeugen oder Groessenaenderungen des Fensters) */
    glutReshapeFunc(cbReshape);

    /* Display-Callback - wird an mehreren Stellen imlizit (z.B. im Anschluss an
     * Reshape-Callback) oder explizit (durch glutPostRedisplay) angestossen */
    glutDisplayFunc(cbDisplay);
}

/**
 * Initialisiert das Programm (inkl. I/O und OpenGL) und startet die
 * Ereignisbehandlung.
 * @param title Beschriftung des Fensters
 * @param width Breite des Fensters
 * @param height Hoehe des Fensters
 * @return ID des erzeugten Fensters, 0 im Fehlerfall
 */
int
initAndStartIO(char *title, int width, int height) {
    int windowID = 0;

    /* Kommandozeile immitieren */
    int argc = 1;
    char *argv = "cmd";

    /* Glut initialisieren */
    glutInit(&argc, &argv);

    /* DEBUG-Ausgabe */
    INFO (("Erzeuge Fenster...\n"));

    /* Initialisieren des Fensters */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);

    /* Fenster erzeugen */
    windowID = glutCreateWindow(title);

    if (windowID) {

        /* DEBUG-Ausgabe */
        INFO (("...fertig.\n\n"));

        /* DEBUG-Ausgabe */
        INFO (("Initialisiere Szene...\n"));

        if (initScene()) {
            /* DEBUG-Ausgabe */
            INFO (("...fertig.\n\n"));

            /* DEBUG-Ausgabe */
            INFO (("Registriere Callbacks...\n"));

            registerCallbacks();

            /* DEBUG-Ausgabe */
            INFO (("...fertig.\n\n"));

            /* DEBUG-Ausgabe */
            INFO (("Trete in Schleife der Ereignisbehandlung ein...\n"));

            glutMainLoop();
        } else {
            /* DEBUG-Ausgabe */
            INFO (("...fehlgeschlagen.\n\n"));

            glutDestroyWindow(windowID);
            windowID = 0;
        }
    } else {
        /* DEBUG-Ausgabe */
        INFO (("...fehlgeschlagen.\n\n"));
    }

    return windowID;
}