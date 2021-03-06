
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
#include "types.h"

enum e_Direction direction = dirNone;
double cooldown = 0.0f;

/**
 * Aendert die Sicht der Kamera anhand von Winkeln
 * @param radius
 * @param anglehorizontal
 * @param angleVertical - ein nach einer Himmelsrichtung orientierter Horizontalwinkel
 */
void changeCameraView(GLfloat radius, GLfloat anglehorizontal, GLfloat angleVertical) {

    CameraView cam = getGame()->camera;

    cam.radius += radius;
    cam.angleHorizontal += anglehorizontal;
    cam.angleVertical += angleVertical;

    /* Begrenzungen Zoom */
    if (cam.radius < 1) {
        cam.radius = 1;
    }
    if (cam.radius > 4) {
        cam.radius = 4;
    }

    // Vertikale Begrenzung für Sicht von oben
    if (cam.angleVertical < 1.0f) {
        cam.angleVertical = 1.0f;
    }

    // Keine Ansicht von Unten erlauben
    if (cam.angleVertical > 80.0f) {
        cam.angleVertical = 80.0f;
    }

    getGame()->camera = cam;
}

/**
 * Verarbeitung eines Mausereignisses.
 * Durch Bewegung der Maus bei gedrueckter Maustaste kann die aktuelle
 * Zeichenfarbe beeinflusst werden.
 * Falls Debugging aktiviert ist, wird jedes Mausereignis auf stdout
 * ausgegeben.
 * @param x x-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 * @param y y-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 * @param eventType Typ des Ereignisses.
 * @param button ausloesende Maustaste (nur bei Ereignissen vom Typ mouseButton).
 * @param state Status der Maustaste (nur bei Ereignissen vom Typ mouseButton).
 */
void handleMouseEvent(int x, int y, CGMouseEventType eventType,
                      int button, int state) {

    static int mousePosition[2] = {0, 0};
    static int mouseState = GLUT_UP;

    /* Veraenderung der Kameraausrichtung */
    float radius = 0.0f;
    float horizontal = 0.0f;
    float vertical = 0.0f;

    switch (eventType) {
        case mouseButton:
            if (button == 3 && state == GLUT_UP) {
                /* Hochscrollen */
                radius -= 0.6f;
            } else if (button == 4 && state == GLUT_UP) {
                /* Runterscrollen */
                radius += 0.6f;
            } else if (button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON) {
                /* Bei Mausklick die x- und YWerte setzen,
                 * um bei "mousemotion" die Kamera zu bewegen */
                mouseState = state;
                mousePosition[0] = x;
                mousePosition[1] = y;
            }
            break;

        case mouseMotion:
            if (mouseState == GLUT_DOWN) {
                int offsetX = mousePosition[0] - x;
                int offsetY = mousePosition[1] - y;

                /* https://www.glprogramming.com/red/chapter03.html */
                horizontal += ((float) offsetX) * -0.2f;
                vertical += ((float) offsetY) * 0.2f;

                mousePosition[0] = x;
                mousePosition[1] = y;
            }
            break;
        default:
            break;
    }

    changeCameraView(radius, horizontal, vertical);
}

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

    /* perspektivische Projektion */
    gluPerspective(15.0,         /* Oeffnungswinkel */
                   aspect,       /* Seitenverhaeltnis */
                   0.1,          /* nahe Clipping-Ebene */
                   40.0 /* ferne Clipping-Ebene */ );
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
    double interval = (double) (thisCallTime - lastCallTime) / 1000.0f;

    if (cooldown < 0) {
        setPlayerMovement(direction);
        if (direction != dirNone) {
            cooldown = COOLDOWN_TIME;
        }
    } else {
        cooldown -= interval;
    }

    if (getGame()->movementCooldown > 0.0f) {
        getGame()->movementCooldown -= (float) interval;
    }

    /* Wieder als Timer-Funktion registrieren */
    glutTimerFunc(1000 / TIMER_CALLS_PS, cbTimer, thisCallTime);

    /* Neuzeichnen anstossen */
    glutPostRedisplay();
}

/**
 * Wird aufgerufen, um die Zeit fuer das Spiel zu zeichnen
 */
static void
decreaseTimer() {
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
 * Setzt einen Viewport fuer 2-dimensionale Darstellung.
 *
 * @param x, y Position des Viewports im Fenster - (0, 0) ist die untere linke Ecke
 * @param width, height Breite und Hoehe des Viewports
 */
static void set2DViewport(GLint x, GLint y, GLint width, GLint height) {
    /* Seitenverhaeltnis bestimmen */
    double aspect = (double) width / height;

    /* Folge Operationen beeinflussen die Projektionsmatrix */
    glMatrixMode(GL_PROJECTION);

    /* Einheitsmatrix laden */
    glLoadIdentity();

    /* Viewport-Position und -Ausdehnung bestimmen */
    glViewport(x, y, width, height);

    /* Das Koordinatensystem bleibt immer quadratisch */
    if (aspect <= 1) {
        gluOrtho2D(-1, 1,                     /* left, right */
                   -1 / aspect, 1 / aspect); /* bottom, top */
    } else {
        gluOrtho2D(-1 * aspect, 1 * aspect, /* left, right */
                   -1, 1);                    /* bottom, top */
    }

    /* Folge Operationen beeinflussen die Modelviewmatrix */
    glMatrixMode(GL_MODELVIEW);

    /* Einheitsmatrix laden */
    glLoadIdentity();
}

/**
 * Setzt den Viewport fuer die 3D-Darstellung
 * @param x - Position x im Fenster
 * @param y - Position y im Fenster
 * @param width - Breite des Fensters
 * @param height - Hoehe des Fensters
 */
static void set3DViewport(GLint x, GLint y, GLint width, GLint height) {
    double aspect = (double) width / height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80,
                   aspect,
                   0.05,
                   50);

    /* Position und Groesse bestimmen */
    glViewport(x, y, width, height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
 * Zeichen-Callback.
 * Loescht die Buffer, ruft das Zeichnen der Szene auf und tauscht den Front-
 * und Backbuffer.
 */
static void
cbDisplay(void) {
    /* Fensterdimensionen auslesen */
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    /* Buffer zuruecksetzen */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* 3D Ansicht */
    set3DViewport(0, 0, width / 2, height);
    drawScene(GL_TRUE);

    /* 2D Minimap */
    set2DViewport(width / 1.5, 0, width / 4, height);
    drawScene(GL_FALSE);

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

/*
 * Aendert den Spielstatus
 */
void switchGameStatus(Gamestatus status) {
    if (getGame()->gameStatus == GAME_RUNNING) {
        getGame()->gameStatus = status;
    } else if (getGame()->gameStatus == status) {
        getGame()->gameStatus = GAME_RUNNING;
    }
}

/**
 * Mouse-Button-Callback.
 * @param button Taste, die den Callback ausgeloest hat.
 * @param state Status der Taste, die den Callback ausgeloest hat.
 * @param x X-Position des Mauszeigers beim Ausloesen des Callbacks.
 * @param y Y-Position des Mauszeigers beim Ausloesen des Callbacks.
 */
static void cbMouseButton(int button, int state, int x, int y) {
    handleMouseEvent(x, y, mouseButton, button, state);
}

/**
 * Mouse-Motion-Callback.
 * @param x X-Position des Mauszeigers.
 * @param y Y-Position des Mauszeigers.
 */
static void cbMouseMotion(int x, int y) {
    handleMouseEvent(x, y, mouseMotion, 0, 0);
}

/**
 * Mouse-Passive-Motion-Callback.
 * @param x X-Position des Mauszeigers.
 * @param y Y-Position des Mauszeigers.
 */
static void
cbMousePassiveMotion(int x, int y) {
    handleMouseEvent(x, y, mousePassiveMotion, 0, 0);
}

/**
 * Setzt die letzte Richtung des Spielers
 * @param dir - Richtung
 */
void setLastDirection(enum e_Direction dir) {
    direction = dir;
    getGame()->lastDirection = dir;
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
            if (getGame()->levelId < NUMBER_OF_LEVEL) {
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
                    setLastDirection(dirLeft);
                    break;
                case GLUT_KEY_RIGHT:
                    setLastDirection(dirRight);
                    break;
                case GLUT_KEY_UP:
                    setLastDirection(dirUp);
                    break;
                case GLUT_KEY_DOWN:
                    setLastDirection(dirDown);
                    break;
                case GLUT_KEY_F1:
                    toggle(WIREFRAME);
                    break;
                case GLUT_KEY_F2:
                    toggle(NORMALS);
                    break;
                case GLUT_KEY_F3:
                    toggle(WORLDLIGHT);
                    break;
                case GLUT_KEY_F4:
                    toggle(SPOTLIGHT);
                    break;
                case GLUT_KEY_F5:
                    toggle(FULLSCREEN);
                    break;
            }
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

                case 'c':
                case 'C':
                    toggle(FIRSTPERSON);
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

                case '4':
                    initLevel(4);
                    break;

                case 'r':
                case 'R':
                    initLevel(getGame()->levelId);
                    break;

                case 'a':
                case 'A':
                    toggle(ANIMATION);
                    break;

                    /* Programm beenden */
                case 'q':
                case 'Q':
                case ESC:
                    exit(0);
            }
        }
    } else if (status == GLUT_UP) {

        /* Spezialtaste gedrueckt */
        if (isSpecialKey) {
            switch (key) {
                /* Bewegung des Rechtecks in entsprechende Richtung beenden */
                case GLUT_KEY_LEFT:
                    direction = dirNone;
                    break;
                case GLUT_KEY_RIGHT:
                    direction = dirNone;
                    break;
                case GLUT_KEY_UP:
                    direction = dirNone;
                    break;
                case GLUT_KEY_DOWN:
                    direction = dirNone;
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

    /* Mouse-Button-Callback (wird ausgefuehrt, wenn eine Maustaste
 * gedrueckt oder losgelassen wird) */
    glutMouseFunc(cbMouseButton);

    /* Mouse-Motion-Callback (wird ausgefuehrt, wenn die Maus bewegt wird,
     * waehrend eine Maustaste gedrueckt wird) */
    glutMotionFunc(cbMouseMotion);

    /* Mouse-Motion-Callback (wird ausgefuehrt, wenn die Maus bewegt wird,
     * waehrend keine Maustaste gedrueckt wird) */
    glutPassiveMotionFunc(cbMousePassiveMotion);


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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);

    //Tiefentest (in der init()) aktivieren
    glEnable(GL_DEPTH_TEST);

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

/**
 * Getter fuer die Richtung des Spielers
 * @return
 */
enum e_Direction getDirection() {
    return direction;
}