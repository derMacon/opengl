
#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>

#endif

#include "io.h"
#include "scene.h"
#include "logic.h"
#include "variables.h"
#include "helper.h"

GLboolean showFullscreen = GL_FALSE;

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

    /* Wieder als Timer-Funktion registrieren */
    glutTimerFunc(1000 / TIMER_CALLS_PS, cbTimer, thisCallTime);

    /* Neuzeichnen anstossen */
    glutPostRedisplay();
}

static void
decreaseTimer(int lastCallTime) {
    /* Seit dem Programmstart vergangene Zeit in Millisekunden */
    int thisCallTime = glutGet(GLUT_ELAPSED_TIME);

    /* Wieder als Timer-Funktion registrieren */
    glutTimerFunc(1000, decreaseTimer, thisCallTime);

    if (getGame()->gameStatus == GAME_RUNNING) {
        decreaseTime();
    }

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

void switchGameStatus(Gamestatus status) {
    if (getGame()->gameStatus == GAME_RUNNING) {
        getGame()->gameStatus = status;
    } else if (getGame()->gameStatus == status) {
        getGame()->gameStatus = GAME_RUNNING;
    }
}

/**
 * Verarbeitung eines Tasturereignisses.
 * Pfeiltasten steuern die Position des angezeigten Rechtecks.
 * F1-Taste (de-)aktiviert Wireframemodus.
 * f-Taste schaltet zwischen Fenster und Vollbilddarstellung um.
 * ESC-Taste und q, Q beenden das Programm.
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
#define ESC 27

    /* Taste gedrueckt */
    if (status == GLUT_DOWN) {

        Gamestatus s = getGame()->gameStatus;
        if (s == GAME_WON) {
            // Naechstes Level
            if (getGame()->levelId < 3) {
                initLevel(++getGame()->levelId);
            } else {
                initLevel(1);
            }
        } else if (s == GAME_LOST) {
            initLevel(getGame()->levelId);
        }

        /* Spezialtaste gedrueckt */
        if (isSpecialKey) {
            switch (key) {

                /* Bewegung des Rechtecks in entsprechende Richtung beenden */
                case GLUT_KEY_LEFT:
                    setPlayerMovement(dirLeft);
                    break;
                case GLUT_KEY_RIGHT:
                    setPlayerMovement(dirRight);
                    break;
                case GLUT_KEY_UP:
                    setPlayerMovement(dirUp);
                    break;
                case GLUT_KEY_DOWN:
                    setPlayerMovement(dirDown);
                    break;
                case GLUT_KEY_F1:
                    toggleWireframe();
                    break;
                case GLUT_KEY_F2:
                    toggleFullscreen();
                    break;}
        }
            /* normale Taste gedrueckt */
        else {
            switch (key) {
                case 'h':
                case 'H':
                    switchGameStatus(GAME_HELP);
                    break;

                case 'p':
                case 'P':
                    switchGameStatus(GAME_PAUSED);
                    break;

                case '1':
                    initLevel(1);
                    break;

                case '2':
                    initLevel(2);
                    break;

                case '3':
                    initLevel(3);
                    break;

                case 'r':
                case 'R':
                    initLevel(getGame()->levelId);
                    break;

                case 'a':
                case 'A':
                    setPlayerMovement(dirLeft);
                    break;
                case 'd':
                case 'D':
                    setPlayerMovement(dirRight);
                    break;
                case 'w':
                case 'W':
                    setPlayerMovement(dirUp);
                    break;

                case 's':
                case 'S':
                    setPlayerMovement(dirDown);
                    break;
                    /* Programm beenden */
                case 'q':
                case 'Q':
                case ESC:
                    exit(0);
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

    glutTimerFunc(1000, decreaseTimer, glutGet(GLUT_ELAPSED_TIME));

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
    int windowID;

    /* Kommandozeile immitieren */
    int argc = 1;
    char *argv = "cmd";

    /* Glut initialisieren */
    glutInit(&argc, &argv);

    /* Initialisieren des Fensters */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);

    // Auf dem aktuellen Bildschirm anzeigen
    glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH) / 2, glutGet(GLUT_SCREEN_HEIGHT) / 2);

    /* Fenster erzeugen */
    windowID = glutCreateWindow(title);

    if (windowID) {
        if (initScene()) {
            registerCallbacks();
            glutMainLoop();
        } else {
            glutDestroyWindow(windowID);
            windowID = 0;
        }
    }

    return windowID;
}