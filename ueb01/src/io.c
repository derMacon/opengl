
#include <GL/glut.h>
#include "io.h"
#include "debug.h"
#include "scene.h"
#include "logic.h"
#include "variables.h"
#include "types.h"

GLboolean game_paused = GL_FALSE;
GLboolean show_extra = GL_FALSE;


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
 * Timer-Callback.
 * Initiiert Berechnung der aktuellen Position und anschliessendes Neuzeichnen,
 * setzt sich selbst erneut als Timer-Callback.
 * @param lastCallTime Zeitpunkt, zu dem die Funktion als Timer-Funktion
 *   registriert wurde (In).
 */
static void
cbTimer(int lastCallTime) {
    /* Seit dem Programmstart vergangene Zeit in Millisekunden */
    int thisCallTime = glutGet(GLUT_ELAPSED_TIME);

    /* Seit dem letzten Funktionsaufruf vergangene Zeit in Sekunden */
    double interval = (double) (thisCallTime - lastCallTime) / 1000.0f;

    if (game_paused) {
        interval = 0.0f;
    }

    /* neue Position berechnen (zeitgesteuert) */
    calcStickPosition(interval);

    calcBallPosition(interval);

    /* Wieder als Timer-Funktion registrieren */
    glutTimerFunc(1000 / TIMER_CALLS_PS, cbTimer, thisCallTime);

    /* Neuzeichnen anstossen */
    glutPostRedisplay();
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
 * Verarbeitung eines Tasturereignisses.
 * Pfeiltasten steuern die Position des angezeigten Rechtecks.
 * F1-Taste (de-)aktiviert Wireframemodus.
 * F2-Taste schaltet zwischen Fenster und Vollbilddarstellung um.
 * ESC-Taste und q, Q beenden das Programm.
 * Falls Debugging aktiviert ist, wird jedes Tastaturereignis auf stdout
 * ausgegeben.
 * @param key Taste, die das Ereignis ausgeloest hat. (ASCII-Wert oder WERT des
 *        GLUT_KEY_<SPECIAL>.
 * @param status Status der Taste, GL_TRUE=gedrueckt, GL_FALSE=losgelassen.
 * @param isSpecialKey ist die Taste eine Spezialtaste?
 * @param x x-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 * @param y y-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 */
static void
handleKeyboardEvent(int key, int status, GLboolean isSpecialKey, int x,
                    int y) {
    /** Keycode der ESC-Taste */
#define ESC 27

    /* Debugausgabe */
    // debugPrintKeyboardEvent (key, status, isSpecialKey, x, y);

    /* Taste gedrueckt */
    if (status == GLUT_DOWN) {
        /* Spezialtaste gedrueckt */
        if (isSpecialKey) {
            switch (key) {
                /* Bewegung des Rechtecks in entsprechende Richtung starten */
                case GLUT_KEY_LEFT:
                    setStickMovement(dirLeft, GL_TRUE);
                    break;
                case GLUT_KEY_RIGHT:
                    setStickMovement(dirRight, GL_TRUE);
                    break;
            }
        }
            /* normale Taste gedrueckt */
        else {
            switch (key) {
                case 'p':
                    game_paused = !game_paused;
                    break;

                    /* Programm beenden */
                case 'q':
                case 'Q':
                case ESC:
                    exit(0);
            }
        }
    }
        /* Taste losgelassen */
    else {
        /* Spezialtaste losgelassen */
        if (isSpecialKey) {
            switch (key) {
                /* Bewegung des Rechtecks in entsprechende Richtung beenden */
                case GLUT_KEY_LEFT:
                    setStickMovement(dirLeft, GL_FALSE);
                    break;
                case GLUT_KEY_RIGHT:
                    setStickMovement(dirRight, GL_FALSE);
                    break;
                case GLUT_KEY_F1:
                    toggleWireframe();
                    break;
            }
        }
    }

}

/**
 * Callback fuer Tastendruck.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Tastendrucks (In).
 * @param y y-Position der Maus zur Zeit des Tastendrucks (In).
 */
static void
cbKeyboard(unsigned char key, int x, int y) {
    handleKeyboardEvent(key, GLUT_DOWN, GL_FALSE, x, y);
}

/**
 * Callback fuer Tastenloslassen.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Loslassens (In).
 * @param y y-Position der Maus zur Zeit des Loslassens (In).
 */
static void
cbKeyboardUp(unsigned char key, int x, int y) {
    handleKeyboardEvent(key, GLUT_UP, GL_FALSE, x, y);
}


/**
 * Callback fuer Druck auf Spezialtasten.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Tastendrucks (In).
 * @param y y-Position der Maus zur Zeit des Tastendrucks (In).
 */
static void
cbSpecial(int key, int x, int y) {
    handleKeyboardEvent(key, GLUT_DOWN, GL_TRUE, x, y);
}

/**
 * Callback fuer Loslassen von Spezialtasten.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Loslassens (In).
 * @param y y-Position der Maus zur Zeit des Loslassens (In).
 */
static void
cbSpecialUp(int key, int x, int y) {
    handleKeyboardEvent(key, GLUT_UP, GL_TRUE, x, y);
}

/**
 * Registrierung der GLUT-Callback-Routinen.
 */
void registerCallbacks(void) {

    /* Tasten-Druck-Callback - wird ausgefuehrt, wenn eine Taste gedrueckt wird */
    glutKeyboardFunc(cbKeyboard);

    /* Tasten-Loslass-Callback - wird ausgefuehrt, wenn eine Taste losgelassen wird */
    glutKeyboardUpFunc(cbKeyboardUp);

    /* Spezialtasten-Druck-Callback - wird ausgefuehrt, wenn Spezialtaste
     * (F1 - F12, Links, Rechts, Oben, Unten, Bild-Auf, Bild-Ab, Pos1, Ende oder Einfuegen)
     * gedrueckt wird
     * */
    glutSpecialFunc(cbSpecial);

    /* Spezialtasten-Loslass-Callback - wird ausgefuehrt, wenn eine Spezialtaste losgelassen wird */
    glutSpecialUpFunc(cbSpecialUp);

    /* Automat. Tastendruckwiederholung ignorieren */
    glutIgnoreKeyRepeat(1);

    /* Timer-Callback - wird einmalig nach msescs Millisekunden ausgefuehrt */
    glutTimerFunc(1000 / TIMER_CALLS_PS,
                  cbTimer,
                  glutGet(GLUT_ELAPSED_TIME));


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