#include "utility.h"
#include <GL/freeglut.h>
#include "variables.h"
#include "main.h"
#include "debugGL.h"

/**
 * Gibt die Hilfe in der Console aus
 */
void showHelp() {
    printf("%s\n", "--- Hilfe ---");
    printf("%s\n", "q,Q,ESC:    Beenden");
    printf("%s\n", "F1:         Wireframe an/aus");
    printf("%s\n", "F2:         Normals an/aus");
    printf("%s\n", "F3:         Phong an/aus");
    printf("%s\n", "F4:         Texturen an/aus");
    printf("%s\n", "F5:         Fullscreen an/aus");
    printf("%s\n", "1:          Sepia an");
    printf("%s\n", "2:          Graustufen an");
    printf("%s\n", "3:          Normale Farben an");
    printf("%s\n", "b,B:        Pause an/aus");
    printf("%s\n", "----------");
}

/**
 * Timer-Callback.
 * Initiiert Berechnung der aktuellen Position und Farben und anschliessendes
 * Neuzeichnen, setzt sich selbst erneut als Timer-Callback.
 * @param lastCallTime Zeitpunkt, zu dem die Funktion als Timer-Funktion
 *   registriert wurde (In).
 */
static void
cbTimer(int lastCallTime) {
    (void) lastCallTime;
    /* Seit dem Programmstart vergangene Zeit in Millisekunden */
    int thisCallTime = glutGet(GLUT_ELAPSED_TIME);

    /* Wieder als Timer-Funktion registrieren */
    glutTimerFunc(1000 / TIMER_CALLS_PS, cbTimer, thisCallTime);

    /* Neuzeichnen anstossen */
    glutPostRedisplay();
}

/**
 * Zeichen-Callback.
 * Loescht die Buffer, ruft das Zeichnen der Szene auf und erzwingt die
 * Darstellung durch OpenGL.
 */
static void
cbDisplay(void) {
    /* Buffer zuruecksetzen */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Szene zeichnen */
    drawScene();

    /* Objekt anzeigen */
    glutSwapBuffers();
}

/**
 * Aendert die Farben im PHONG-Modus
 * @param color
 */
void changeColor(enum ColorType color) {
    getSettings()->color = color;
}

/**
 * Togglet verschiedene Dinge (z.B. FirstPerson oder Weltlich)
 * @param type - Das angegebene soll getoggled werden
 */
void toggle(enum e_ToggleTypes type) {
    switch (type) {
        case NORMALS:
            getSettings()->showNormals = !getSettings()->showNormals;
            break;
        case TEXTURES:
            getSettings()->showTextures = !getSettings()->showTextures;
            break;
        case FULLSCREEN:
            getSettings()->showFullscreen = !getSettings()->showFullscreen;
            if (getSettings()->showFullscreen) {
                glutFullScreen();
            } else {
                glutPositionWindow(glutGet(GLUT_SCREEN_WIDTH) / 2, glutGet(GLUT_SCREEN_HEIGHT) / 2);
            }
            break;
        case PHONG:
            getSettings()->showPhong = !getSettings()->showPhong;
            break;
        case WIREFRAME:
            getSettings()->showWireframe = !getSettings()->showWireframe;
            if (getSettings()->showWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            break;
        case BREAK:
            getSettings()->showBreak = !getSettings()->showBreak;
            break;
        default:
            break;
    }
}

static void
handleKeyboardEvent(int key, int status, GLboolean isSpecialKey, int x,
                    int y) {
#define ESC 27

    /* Taste gedrueckt */
    if (status == GLUT_DOWN) {

        /* Spezialtaste gedrueckt */
        if (isSpecialKey) {
            switch (key) {
                /* Bewegung des Rechtecks in entsprechende Richtung beenden */
                case GLUT_KEY_F1:
                    toggle(WIREFRAME);
                    break;
                case GLUT_KEY_F2:
                    toggle(NORMALS);
                    break;
                case GLUT_KEY_F3:
                    toggle(PHONG);
                    break;
                case GLUT_KEY_F4:
                    toggle(TEXTURES);
                    break;
                case GLUT_KEY_F5:
                    toggle(FULLSCREEN);
                    break;
            }
        } else {
            switch (key) {
                case '1':
                    changeColor(COLOR_SEPIA);
                    break;
                case '2':
                    changeColor(COLOR_GRAYSCALE);
                    break;
                case '3':
                    changeColor(COLOR_NORMAL);
                    break;
                case 'h':
                case 'H':
                    showHelp();
                    break;
                case 'b':
                case 'B':
                    toggle(BREAK);
                    break;
                case '+':
                    if (*getElevation() < MAX_ELEVATION && !getSettings()->showBreak) {
                        *getElevation() += 0.01f;
                    }
                    printf("Elevation: %f\n", *getElevation());
                    break;
                case '-':
                    if (*getElevation() > MIN_ELEVATION && !getSettings()->showBreak) {
                        *getElevation() -= 0.01f;
                    }
                    printf("Elevation: %f\n", *getElevation());
                    break;
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
 * Registrierung der GLUT-Callback-Routinen.
 */
void registerCallbacks(void) {
    /* DEBUG-Ausgabe */
    INFO(("Registriere Callbacks...\n"));

    /* Ob die extension ARB_debug_output bereit steht hängt vom Treiber
     * ab. Deswegen muss zur Laufzeit geprüft werden, ob das Callback
     * registriert werden kann. */
    if (GLEW_ARB_debug_output) {
        /* Verhindert, dass die OpenGL Implementierung das Callback
         * asynchron aufruft. Damit ist sichergestellt, dass der Callstack
         * im Falle eines Fehlers bereit steht.  */
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        /* In den Rechenzentren ist im Sommersemester 2016 eine veraltete
         * GLEW-Version installiert. Die dort erwartete Funktions-
         * signatur des Callback-Handlers stimmt nicht mit der hier
         * implementierten überein. Wenn ihr (Studenten) auf euren
         * Laptops arbeitet, ist es jedoch durchaus möglich, dass dieser
         * Code fehlerfrei kompiliert. Definiert einfach mal das Makro
         * und testet das. */
#ifdef GLEW_CORRECT
        /* Wenn dieses Callback gesetzt ist, wird die übergebene Funktion
         * immer dann aufgerufen, wenn Fehler auftreten oder weitere
         * nennenswerte Informationen ausgegeben werden müssen. Ihr
         * müsst dann nicht immer glGetError aufrufen und bekommt
         * zusätzlich auch noch einige Zusatzinformationen, die das
         * Debuggen vereinfachen. */
        glDebugMessageCallback(cbDebugOutput, NULL);
#endif

        /* Aktivieren der Extension */
        glEnable(GL_DEBUG_OUTPUT);
    }

    /* Display-Callback (wird ausgefuehrt, wenn neu gezeichnet wird
     * z.B. nach Erzeugen oder Groessenaenderungen des Fensters) */
    glutDisplayFunc(cbDisplay);

    /* Tasten-Druck-Callback - wird ausgefuehrt, wenn eine Taste gedrueckt wird */
    glutKeyboardFunc(cbKeyboard);

    /* Spezialtasten-Druck-Callback - wird ausgefuehrt, wenn Spezialtaste
    * (F1 - F12, Links, Rechts, Oben, Unten, Bild-Auf, Bild-Ab, Pos1, Ende oder Einfuegen)
    * gedrueckt wird
    * */
    glutSpecialFunc(cbSpecial);

    /* Timer-Callback - wird einmalig nach msescs Millisekunden ausgefuehrt */
    glutTimerFunc(1000 / TIMER_CALLS_PS,         /* msecs - bis Aufruf von func */
                  cbTimer,                       /* func  - wird aufgerufen    */
                  glutGet(GLUT_ELAPSED_TIME));  /* value - Parameter, mit dem
                                                   func aufgerufen wird */

    /* DEBUG-Ausgabe */
    INFO(("...fertig.\n\n"));
}
