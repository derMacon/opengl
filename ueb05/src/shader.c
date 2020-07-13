/**
 * @file
 * Einfaches Beispielprogramm fuer OpenGL, GLUT & GLEW
 * sowie einen Shader in GLSL.
 * Vier texturierte Quadrate werden gezeigt.
 * @author copyright (C) Fachhochschule Wedel 1999-2018. All rights reserved.
 */
/* ---- System Header einbinden ---- */
#include <stdio.h>
#include <stdlib.h>

/* ---- Eigene Header einbinden ---- */
#include "utility.h"
#include "debugGL.h"
#include "variables.h"
#include "io.h"
#include "scene.h"

/** Anhebung der Vertizes */
static float elevation;
static Settings settings;

/**
 * Getter fuer die Settings
 * @return die Settings
 */
Settings *getSettings() {
    return &settings;
}

/**
 * Getter fuer die Elevation
 * @return die Elevation?
 */
float *getElevation() {
    return &elevation;
}

/**
 * Initialisierung des OpenGL-Systems.
 * Setzt Shading, Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
static int
initOpenGL(void) {
    /* DEBUG-Ausgabe */
    INFO (("Initialisiere OpenGL...\n"));

    /* Beim Aufruf von glClear werden alle Pixel des Framebuffers auf
     * diesen Wert gesetzt. */
    glClearColor(0, 0, 0, 0);

    /* Beim Aufrus von glClear werden alle Pixel des Depthbuffer auf
     * diesen Wert gesetzt */
    glClearDepth(1);

    /* Aktivieren des Tiefentests */
    glEnable(GL_DEPTH_TEST);

    /* DEBUG-Ausgabe */
    INFO (("...fertig.\n\n"));

    /* Alles in Ordnung? */
    return 1;
}

/**
 * Debug-Callback.
 * @param type Art der Debug-Information. Einer der folgenden Werte:
 *     DEBUG_TYPE_ERROR_ARB
 *     DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB
 *     DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB
 *     DEBUG_TYPE_PORTABILITY_ARB
 *     DEBUG_TYPE_PERFORMANCE_ARB
 *     DEBUG_TYPE_OTHER_ARB
 * @param message Debug-Nachricht.
 */
#ifdef GLEW_CORRECT
static void
cbDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
  if (type == GL_DEBUG_TYPE_ERROR_ARB)
    {
      printf("ARB_debug_output: %s\n", message);
      exit(1);
    }
}
#endif

/**
 * Erzeugt ein Fenster.
 * @param title Beschriftung des Fensters
 * @param width Breite des Fensters
 * @param height Hoehe des Fensters
 * @return ID des erzeugten Fensters, 0 im Fehlerfall
 */
static int
createWindow(char *title, int width, int height) {
    int windowID = 0;

    /* DEBUG-Ausgabe */
    INFO (("Erzeuge Fenster...\n"));

    /* Initialisieren des Fensters */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH) / 2, glutGet(GLUT_SCREEN_HEIGHT) / 2);

    /* Es wird explizit ein Context mit der Version 3.3 und dem Kern-
     * Profil angefordert. */
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    windowID = glutCreateWindow(title);

    /* DEBUG-Ausgabe */
    INFO (("...fertig..\n\n"));

    return windowID;
}

/**
 * Hauptprogramm.
 * Initialisiert Fenster, Anwendung und Callbacks, startet glutMainLoop.
 * @param argc Anzahl der Kommandozeilenparameter (In).
 * @param argv Kommandozeilenparameter (In).
 * @return Rueckgabewert im Fehlerfall ungleich Null.
 */
int
main(int argc, char **argv) {
    GLenum error;

    /* Glut initialisieren */
    glutInit(&argc, argv);


    /* Erzeugen des Fensters */
    if (!createWindow("ueb05", 500, 500)) {
        fprintf(stderr, "Initialisierung des Fensters fehlgeschlagen!");
        exit(1);
    }

    /* Initialisieren von GLEW.
     * Wenn glewExperimental auf GL_TRUE gesetzt wird, läd GLEW
     * zusätzliche Extensions. In diesem konkreten Programm wird dies
     * beispielsweise benötigt, um glGenVertexArrays() aufzurufen. (Ohne
     * glewExperimental auf GL_TRUE zu setzen tritt ein Speicherzugriffs-
     * fehler auf.) */
    glewExperimental = GL_TRUE;
    error = glewInit();
    if (error != GLEW_OK) {
        fprintf(stderr, "Initialisierung von GLEW fehlgeschlagen!");
        exit(1);
    }

    /* Initialen OpenGL-Zustand setzen */
    if (!initOpenGL()) {
        fprintf(stderr, "Initialisierung von OpenGL fehlgeschlagen!");
        exit(1);
    }

    /* registriere Callbacks */
    registerCallbacks();

    /* Initialisiere Szene */
    initScene();

    /* Hauptschleife der Ereignisbehandlung starten */
    glutMainLoop();

    return 0;
}
