/* ---- System Header einbinden ---- */
#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>

#endif

#include "io.h"
#include <stdio.h>
#include "scene.h"
#include "logic.h"
#include "variables.h"
#include "types.h"
#include "math.h"
#include "stringOutput.h"
#include "drawWater.h"
#include "texture.h"
#include "drawObjects.h"

double idleInterval = 0;
double lastCallTimeIdle = 0;

/**
 * Stellt die Kamera ein
 */
void setCamera() {
    GLfloat radius = getState()->camera.radius;
    GLfloat angleHorizontal = TO_RADIANS(getState()->camera.angleHorizontal);
    GLfloat angleVertical = TO_RADIANS(getState()->camera.angleVertical);

    // Kamera einstellen
    GLfloat camX = radius * sinf(angleVertical) * cosf(angleHorizontal);
    GLfloat camY = radius * cosf(angleVertical);
    GLfloat camZ = radius * sinf(angleVertical) * sinf(angleHorizontal);

    gluLookAt(camX, camY, camZ,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
}

/**
 * Verarbeitung der Picking-Ergebnisse.
 * Findet den Treffer, der dem Betrachter am naechsten liegt und gibt die
 * Namen dieses Treffers aus.
 * @param numHits Anzahl der getroffenen Objekte (In).
 * @param buffer Buffer, in dem die Treffer gespeichert sind (In).
 */
static void
processHits(GLint numHits, GLuint buffer[], GLboolean isLeftMouse) {
    /* Anzahl der Namen des Treffers, der am naechsten zum Betrachter ist */
    GLint numOfClosestNames = 0;
    /* Anzahl der Namen des Treffers, der am naechsten zum Betrachter ist */
    GLuint *ptrClosestNames = NULL;

    /* Laufvariable - Nummer des akt. verarbeiteten Treffers */
    GLint i;
    /* Laufvariable - Zeiger auf den Beginn des Hit-Records */
    GLuint *ptr = (GLuint *) buffer;

    /* DepthBuffer - Wert */
    GLuint minZ = 0xffffffff;

    if (numHits > 0) {
        /* Alle Treffer verarbeiten */
        for (i = 0; i < numHits; i++) {
            /* Ist der Treffer naeher dran, als der bisher naechste? */
            if (*(ptr + 1) < minZ) {
                /* Anzahl der Namen merken */
                numOfClosestNames = *ptr;
                /* Zeiger auf den ersten Namen merken */
                ptrClosestNames = ptr + 3;
                /* als neuen minimalen Tiefenwert merken */
                minZ = *(ptr + 1);
            }
            /* zum Beginn des naechsten Hit-Records springen */
            /* Schrittweite: Anzahl-Feld + minZ-Feld + maxZ-Feld + Namen-Felder */
            ptr += 3 + *ptr;
        }

        /* Alle Namen des Treffers ausgeben, der am naechsten zum Betrachter liegt */
        for (i = 0; i < numOfClosestNames; i++, ptrClosestNames++) {
            int idx = *ptrClosestNames;
            if (idx >= 0) {
                if (isLeftMouse) {
                    getState()->grid.vertices[idx][Y] += WATER_INCREASE_VALUE;
                } else {
                    getState()->grid.vertices[idx][Y] -= WATER_INCREASE_VALUE;
                }
            }
        }
    }
}

/**
 * Setzt den Viewport fuer die 3D-Darstellung
 * @param x - Position x im Fenster
 * @param y - Position y im Fenster
 * @param width - Breite des Fensters
 * @param height - Hoehe des Fensters
 */
static void setViewport(GLint x, GLint y, GLint width, GLint height) {
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
 * Picking. Auswahl von Szenenobjekten durch Klicken mit der Maus.
 */
static void
pick(int x, int y, GLboolean isLeftMouse) {
    /* Groesse des Buffers fuer Picking Ergebnisse */
#define SELECTBUFSIZE 512

    /* Fensterdimensionen auslesen */
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    /* Puffer fuer Picking-Ergebnis */
    GLuint buffer[SELECTBUFSIZE];

    /* Puffer festlegen, Name-Stack leeren und Rendermode wechseln */
    glSelectBuffer(SELECTBUFSIZE, buffer);
    glInitNames();
    glRenderMode(GL_SELECT);

    /* Folge Operationen beeinflussen die Projektionsmatrix */
    glMatrixMode(GL_PROJECTION);

    /* Einheitsmatrix laden */
    glLoadIdentity();

    gluPickMatrix(x, height - y, 10, 10, viewport);

    /* Perspektivische Darstellung */
    gluPerspective(80,                           /* Oeffnungswinkel */
                   (double) width / height,    /* Seitenverhaeltnis */
                   0.05,                        /* nahe Clipping-Ebene */
                   50);                         /* ferne Clipping-Ebene */

    /* Folge Operationen beeinflussen die Modelviewmatrix */
    glMatrixMode(GL_MODELVIEW);

    /* Einheitsmatrix laden */
    glLoadIdentity();

    /* Kamera richten */
    setCamera();

    /* Spheren zeichnen */
    drawPickingSpheres();

    /* Zeichnen beenden und auswerten */
    glFlush();
    processHits(glRenderMode(GL_RENDER), buffer, isLeftMouse);
}

/**
 * Berechnung der Frames pro Sekunde.
 * @return aktuelle Framerate.
 */
float frameRate(void) {
    /* Zeitpunkt der letzten Neuberechnung */
    static int timebase = 0;
    /* Anzahl der Aufrufe seit letzter Neuberechnung */
    static int frameCount = 0;
    /* Zuletzt berechneter FPS-Wert */
    static float fps = 0;
    /* aktuelle "Zeit" */
    static int time = 0;

    /* Diesen Aufruf hinzuzaehlen */
    frameCount++;

    /* seit dem Start von GLUT vergangene Zeit ermitteln */
    time = glutGet(GLUT_ELAPSED_TIME);

    /* Eine Sekunde ist vergangen */
    if (time - timebase > 1000) {
        /* FPS-Wert neu berechnen */
        fps = frameCount * 1000.0f / (time - timebase);

        /* Zureuecksetzen */
        timebase = time;
        frameCount = 0;
    }

    /* Aktuellen FPS-Wert zurueckgeben */
    return fps;
}

/**
 * Aendert die Sicht der Kamera anhand von Winkeln
 * @param radius
 * @param anglehorizontal
 * @param angleVertical - ein nach einer Himmelsrichtung orientierter Horizontalwinkel
 */
void changeCameraView(GLfloat radius, GLfloat anglehorizontal, GLfloat angleVertical) {

    CameraView cam = getState()->camera;

    cam.radius += radius;
    cam.angleHorizontal += anglehorizontal;
    cam.angleVertical += angleVertical;

    /* Begrenzungen Zoom */
    if (cam.radius < CAMERA_MIN_RADIUS) {
        cam.radius = CAMERA_MIN_RADIUS;
    }
    if (cam.radius > CAMERA_MAX_RADIUS) {
        cam.radius = CAMERA_MAX_RADIUS;
    }

    // Vertikale Begrenzung für Sicht von oben
    if (cam.angleVertical < 1.0f) {
        cam.angleVertical = 1.0f;
    }

    // Keine Ansicht von Unten erlauben
    if (cam.angleVertical > 80.0f) {
        cam.angleVertical = 80.0f;
    }

    getState()->camera = cam;
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

                if (state == GLUT_UP) {
                    pick(x, y, button == GLUT_LEFT_BUTTON);
                }

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
 * Funktion, um Codeverdopplung zu sparen.
 * Andert die Richtung des Spotlights
 * Links: 1,0
 * Oben: 0, 1
 * Rechts: -1,  0
 * Unten: 0, -1
 * @param isX True, x Wert wird geaendert
 * @param val Der Wert, der entweder erhoeht oder verringert wird
 */
void changeSpotLightVal(GLboolean isX, float val) {
    Spotlight s = getState()->spotlight;
    GLboolean isIncreasing = isX ? s.xIncreasing : s.yIncreasing;
    float incVal = 0.02f;

    if (isIncreasing) {
        val += incVal;
        if (val >= 1) {
            isIncreasing = GL_FALSE;
        }
    } else {
        val -= incVal;
        if (val <= -1) {
            isIncreasing = GL_TRUE;
        }
    }

    if (isX) {
        getState()->spotlight.xIncreasing = isIncreasing;
        getState()->spotlight.posX = val;
    } else {
        getState()->spotlight.yIncreasing = isIncreasing;
        getState()->spotlight.posY = val;
    }
}

/**
 * Wird aufgerufen, um die Zeit fuer das Spiel zu zeichnen
 */
static void
spotLightTimer() {
    /* Seit dem Programmstart vergangene Zeit in Millisekunden */
    int thisCallTime = glutGet(GLUT_ELAPSED_TIME);

    /* Wieder als Timer-Funktion registrieren */
    glutTimerFunc(50, spotLightTimer, thisCallTime);

    if (getState()->settings.showSpotLight) {
        float x = getState()->spotlight.posX;
        float y = getState()->spotlight.posY;

        changeSpotLightVal(GL_TRUE, x);
        changeSpotLightVal(GL_FALSE, y);
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
    /* Frames pro Sekunde */
    static GLfloat fps = 0.0f;

    /* Fensterdimensionen auslesen */
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    /* Buffer zuruecksetzen */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* 3D Ansicht */
    setViewport(0, 0, width, height);
    drawScene();

    /* Objekt anzeigen */
    glutSwapBuffers();

    /* Framerate berechnen */
    fps = frameRate();

    char title[100];
    int len = getState()->grid.length;

    sprintf((char *) title, "Wasser-Animation | Verticies: %d x %d | FPS: %.2f", len, len, (double) fps);
    glutSetWindowTitle(title);
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
void switchGameStatus(Status status) {
    if (getState()->gameStatus == GAME_RUNNING) {
        getState()->gameStatus = status;
    } else if (getState()->gameStatus == status) {
        getState()->gameStatus = GAME_RUNNING;
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
                    toggle(WORLDLIGHT);
                    break;
                case GLUT_KEY_F4:
                    toggle(SPOTLIGHT);
                    break;
                case GLUT_KEY_F5:
                    toggle(FULLSCREEN);
                    break;
                case GLUT_KEY_F6:
                    toggle(TEXTURES);
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

                case 's':
                case 'S':
                    toggle(SPHERES);
                    break;

                case 'p':
                case 'P':
                    switchGameStatus(GAME_PAUSED);
                    break;

                case '+':
                    changeGridSize(GL_TRUE);
                    break;

                case '-':
                    changeGridSize(GL_FALSE);
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
 * IdleTimer
 */
static void cbIdle() {

    /* Seit dem Programmstart vergangene Zeit in Millisekunden */
    int thisCallTime = glutGet(GLUT_ELAPSED_TIME);
    float refresh = 1.0f / TIMER_CALLS_PS;

    /* Seit dem letzten Funktionsaufruf vergangene Zeit in Sekunden */
    idleInterval += (double) (thisCallTime - lastCallTimeIdle) / 1000.0f;

    // Wasser simulieren lassen
    while (idleInterval >= refresh) {
        if (getState()->gameStatus == GAME_RUNNING) {
            simulateWater();
        }

        idleInterval -= refresh;
    }

    lastCallTimeIdle = thisCallTime;
    glutPostRedisplay();
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

    glutIdleFunc(cbIdle), glutTimerFunc(50, spotLightTimer, glutGet(GLUT_ELAPSED_TIME));

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

        /* DEBUG-Ausgabe */
        printf(("Initialisiere Szene...\n"));

        if (initScene()) {
            /* DEBUG-Ausgabe */
            printf(("...fertig.\n\n"));

            /* DEBUG-Ausgabe */
            printf(("Lade und initialisiere Texturen...\n"));

            if (initTextures()) {
                /* DEBUG-Ausgabe */
                printf(("...fertig.\n\n"));

                /* DEBUG-Ausgabe */
                printf(("Registriere Callbacks...\n"));

                registerCallbacks();

                /* DEBUG-Ausgabe */
                printf(("...fertig.\n\n"));

                /* DEBUG-Ausgabe */
                printf(("Trete in Schleife der Ereignisbehandlung ein...\n"));

                glutMainLoop();
            } else {
                /* DEBUG-Ausgabe */
                printf(("...fehlgeschlagen.\n\n"));

                glutDestroyWindow(windowID);
                windowID = 0;
            }
        } else {
            /* DEBUG-Ausgabe */
            printf(("...fehlgeschlagen.\n\n"));

            glutDestroyWindow(windowID);
            windowID = 0;
        }
    } else {
        /* DEBUG-Ausgabe */
        printf(("...fehlgeschlagen.\n\n"));
    }

    return windowID;
}
