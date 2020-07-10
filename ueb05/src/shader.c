/**
 * @file
 * Einfaches Beispielprogramm fuer OpenGL, GLUT & GLEW
 * sowie einen Shader in GLSL.
 * Vier texturierte Quadrate werden gezeigt.
 * @author copyright (C) Fachhochschule Wedel 1999-2018. All rights reserved.
 */

/* ---- System Header einbinden ---- */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

/* ---- Eigene Header einbinden ---- */
#include "utility.h"
#include "debugGL.h"
#include "variables.h"

/* Bibliothek um Bilddateien zu laden. Es handelt sich um eine
 * Bibliothek, die sowohl den Header als auch die Quelle in einer Datei
 * bereitstellt. Die Quelle kann durch die Definition des Makros
 * STB_IMAGE_IMPLEMENTATION eingebunden werden. Genauere Informationen
 * sind der "stb_image.h" zu entnehmen.
 * 
 * Quelle: https://github.com/nothings/stb */
// define global functions as static inline
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>  // Bibliothek um Bilder zu laden
// Quelle: https://github.com/nothings/stb

/* ---- Konstanten ---- */

/** Anzahl der Aufrufe der Timer-Funktion pro Sekunde */
#define TIMER_CALLS_PS 144

/* ---- Globale Variablen ---- */

/** Buffer-Objekt, um die Vertizes zu speichern. */
static GLuint g_arrayBuffer;

/** Vertex-Array-Objekt, um die Attribut-Pointer zu speichern. */
static GLuint g_vertexArrayObject;

/** Programm-Objekt zum Rendern */
static GLuint g_program;

/** Anhebung der Vertizes */
static float g_elevation;

/** Textur */
static GLuint g_texture;

/** Location der uniform-Variable "ModelView" */
static GLint g_locationModelViewMatrix;

/** Location der uniform-Variable "Elevation" */
static GLint g_locationElevation;

/** Location der uniform-Variable "Texture" */
static GLuint g_locationTexture;

/**
 * Alle Daten eines Vertexes.
 */
typedef struct {
    float x, y, z; /**< Position */
    float s, t;    /**< Textur-Koordinate */
} Vertex;

Vertex vert[10000];

/* ---- Funktionen ---- */

/**
 * Zeichen-Funktion.
 * Stellt die Szene dar.
 * Ausgabe eines Rechtecks.
 */
static void
drawScene(void) {
    float viewMatrix[16];
    float t = (float) glutGet(GLUT_ELAPSED_TIME) / 1000;

    const float distance = 2;
    lookAt(distance * sinf(t), 1, distance * cosf(t), 0, 0, 0, 0, 1, 0, viewMatrix);

    /* Aktivieren des Programms. Ab jetzt ist die Fixed-Function-Pipeline
     * inaktiv und die Shader des Programms aktiv. */
    glUseProgram(g_program);

    /* Übermitteln der View-Matrix an den Shader.
     * OpenGL arbeitet intern mit Matrizen in column-major-layout. D.h.
     * nicht die Reihen, sondern die Spalten liegen hintereinander im
     * Speicher. Die Funktionen zur Erzeugen von Matrizen in diesem
     * Programm berücksichtigen dies. Deswegen müssen die Matrizen nicht
     * transponiert werden und es kann GL_FALSE übergeben werden. Beim
     * Schreiben eigener Matrix-Funktionen oder beim Verwenden von
     * Mathematik-Bibliotheken muss dies jedoch berücksichtigt werden. */
    glUniformMatrix4fv(g_locationModelViewMatrix, 1, GL_FALSE, viewMatrix);

    /* Übermitteln der aktuellen Anhebung */
    glUniform1f(g_locationElevation, g_elevation);

    /* Übergeben der Textur an den Shader.
     * Texturen werden nicht direkt an den Shader übergeben, sondern
     * zuerst an eine Textureinheit gebunden. Anschließend wird dem
     * Programm nur der Index der Textureinheit übergeben. */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texture);
    glUniform1i(g_locationTexture, 0);

    /* Aktivieren des Vertex-Array-Objekts (VAO).
     * Hiermit werden alle Attribut-Pointer aktiv, die auf diesem VAO
     * gesetzt wurden. */
    glBindVertexArray(g_vertexArrayObject);

    /* Rendern der Dreiecke.
     * Ab dem ersten Dreieck im Buffer werden alle 8 Dreiecke gerendert.
     * Dem Draw-Command wird jedoch die Anzahl der Vertizes übergeben, die
     * gezeichnet werden sollen. */
    glDrawArrays(GL_TRIANGLES, 0, ((SUBDIVS - 1) * (SUBDIVS - 1)));

    /* Zurücksetzen des OpenGL-Zustands, um Seiteneffekte zu verhindern */
    glBindVertexArray(0);
    glUseProgram(0);
}

/**
 * Gibt den Index eines Vertex wider an den Stellen:
 * @param x - X-Wert
 * @param y - Y-Wert
 * @param size - Groeße des Vertex-Arrays
 * @return - Index an o.g. Stelle
 */
int getIndex(int x, int y, int size) {
    return (y * size) + x;
}

void createMesh(float x, float z, int idx) {

    float meshWidth = 1;
    float cellWidth = meshWidth / (SUBDIVS);
    float offset = -meshWidth / 2;


    vert[idx].x = offset + cellWidth * x;
    vert[idx].y = 0;
    vert[idx].z = offset + cellWidth * z;
    vert[idx].s = (vert[idx].x - offset) / meshWidth;
    vert[idx].t = (vert[idx].z - offset) / meshWidth;

    vert[idx + 1].x = vert[idx].x + cellWidth;
    vert[idx + 1].y = 0;
    vert[idx + 1].z = vert[idx].z + cellWidth;
    vert[idx + 1].s = (vert[idx + 1].x - offset) / meshWidth;
    vert[idx + 1].t = (vert[idx + 1].z - offset) / meshWidth;

    vert[idx + 2].x = vert[idx].x;
    vert[idx + 2].y = 0;
    vert[idx + 2].z = vert[idx].z + cellWidth;
    vert[idx + 2].s = (vert[idx + 2].x - offset) / meshWidth;
    vert[idx + 2].t = (vert[idx + 2].z - offset) / meshWidth;

    vert[idx + 3].x = vert[idx].x;
    vert[idx + 3].y = 0;
    vert[idx + 3].z = vert[idx].z;
    vert[idx + 3].s = (vert[idx + 3].x - offset) / meshWidth;
    vert[idx + 3].t = (vert[idx + 3].z - offset) / meshWidth;

    vert[idx + 4].x = vert[idx].x + cellWidth;
    vert[idx + 4].y = 0;
    vert[idx + 4].z = vert[idx].z;
    vert[idx + 4].s = (vert[idx + 4].x - offset) / meshWidth;
    vert[idx + 4].t = (vert[idx + 4].z - offset) / meshWidth;

    vert[idx + 5].x = vert[idx].x + cellWidth;
    vert[idx + 5].y = 0;
    vert[idx + 5].z = vert[idx].z + cellWidth;
    vert[idx + 5].s = (vert[idx + 5].x - offset) / meshWidth;
    vert[idx + 5].t = (vert[idx + 5].z - offset) / meshWidth;


}


/**
 * Initialisiert das Wasssergrid
 * @param grid - Neues Grid
 * @param size - Diese Groesse sopll es haben
 */
void initGrid() {
    int idx = 0;

    for (int z = 0; z < SUBDIVS; z++) {
        for (int x = 0; x < SUBDIVS; x++) {
            createMesh(x, z, idx);
            idx += 6;

        }
        printf("Testlpl\n");
    }

    glGenBuffers(1, &g_arrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, g_arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/**
 * Bereitet die Szene vor.
 */
static void
initScene(void) {
    initGrid();

    {
        const GLuint positionLocation = 0;
        const GLuint texCoordLocation = 1;
        /* Erstellen eines Vertex-Array-Objektes (VAO).
         * Damit die OpenGL-Implementierung weiß welche Daten der Pipeline
         * bereitgestellt werden müssen, werden Attribut-Pointer gesetzt.
         * Um mehrere Datenquellen (unterschiedliche Meshes) zu verwalten
         * können die Attribut-Pointer in VAOs gruppiert werden.
         *
         * Die Erzeugung von VAOs geschieht prinzipiell genauso wie bei
         * Buffern oder anderen OpenGL-Objekten. */
        glGenVertexArrays(1, &g_vertexArrayObject);
        glBindVertexArray(g_vertexArrayObject);

        /* Die Pointer werden immer in den Buffer gesetzt, der am
         * GL_ARRAY_BUFFER-Target gebunden ist! */
        glBindBuffer(GL_ARRAY_BUFFER, g_arrayBuffer);

        /* Im Vertex-Shader existieren folgende Zeilen:
         * > layout (location = 0) in vec4 vPosition;
         * > layout (location = 1) in vec2 vTexCoord;
         *
         * Beim Aufruf einen Draw-Command, müssen diesen beiden Attributen
         * Daten bereitgestellt werden. Diese sollen aus dem oben erstellten
         * Buffer gelesen werden. Dafür müssen zwei Attribut-Pointer aktiviert
         * und eingerichtet werden. */
        glEnableVertexAttribArray(positionLocation);
        glVertexAttribPointer(
                positionLocation,                           /* location (siehe Shader) */
                3,                           /* Dimensionalität */
                GL_FLOAT,                    /* Datentyp im Buffer */
                GL_FALSE,                    /* Keine Normierung notwendig */
                sizeof(Vertex),              /* Offset zum nächsten Vertex */
                (void *) offsetof(Vertex, x)); /* Offset zum ersten Vertex */

        /* Zweiter Attribut-Pointer für die Textur-Koordinate */
        glEnableVertexAttribArray(texCoordLocation);
        glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, s));

        /* Um Programmierfehler leichter zu finden, sollte der OpenGL-
         * Zustand wieder zurückgesetzt werden. Wird beispielweise das Binden
         * eines Vertex-Array-Objekts vergessen werden, arbeitet OpenGL
         * auf dem vorher gebundenen. Vor allem bei starker Modularisierung
         * sind diese Fehler schwer zu finden. */
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    {
        /* Erstellen des Programms */
        g_program = createProgram("../content/shaders/color.vert", "../content/shaders/color.frag");

        /* Abfragen der uniform-locations.
         * Dies kann auch zur Laufzeit gemacht werden, es ist jedoch
         * performanter dies vorab zu tun, da String-Vergleiche gemacht
         * werden müssen. */
        g_locationModelViewMatrix = glGetUniformLocation(g_program, "ModelView");
        g_locationElevation = glGetUniformLocation(g_program, "Elevation");
        g_locationTexture = glGetUniformLocation(g_program, "Texture");

        /* DEBUG-Ausgabe */
        printf("ModelView hat 'location': %i\n", g_locationModelViewMatrix);
        printf("Elevation hat 'location': %i\n", g_locationElevation);
        printf("Texture hat 'location': %i\n", g_locationTexture);
    }

    {
        /* Erstellen der Projektions-Matrix.
         * Da die Projektions-Matrix für die Laufzeit des Programms konstant
         * ist, wird sie einmalig gesetzt. */
        float projectionMatrix[16];
        perspective(60, (float) glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), 0.01f, 5.0f,
                    projectionMatrix);
        glUseProgram(g_program);
        glUniformMatrix4fv(glGetUniformLocation(g_program, "Projection"), 1, GL_FALSE, projectionMatrix);
        glUseProgram(0);
    }

    {
        /* Laden der Textur. */
        int width, height, comp;
        GLubyte *data = stbi_load("../content/textures/eye.png", &width, &height, &comp, 4);

        /* Erstellen des Textur-Objekts. */
        glGenTextures(1, &g_texture);
        glBindTexture(GL_TEXTURE_2D, g_texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
    }
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
 * Callback fuer Tastendruck.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Tastendrucks (In).
 * @param y y-Position der Maus zur Zeit des Tastendrucks (In).
 */
static void
cbKeyboard(unsigned char key, int x, int y) {
    (void) x;
    (void) y;
/** Keycode der ESC-Taste */
#define ESC 27

    switch (key) {
        /* Programm beenden */
        case 'q':
        case 'Q':
        case ESC:
            exit(0);
            break;
            /* Anhebung erhöhen */
        case '+':
            g_elevation += 0.1f;
            printf("Elevation: %f\n", g_elevation);
            break;
            /* Anhebung verringern */
        case '-':
            g_elevation -= 0.1f;
            printf("Elevation: %f\n", g_elevation);
            break;
        case 'f':
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
    }
}

/**
 * Registrierung der GLUT-Callback-Routinen.
 */
static void
registerCallbacks(void) {
    /* DEBUG-Ausgabe */
    INFO (("Registriere Callbacks...\n"));

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

    /* Timer-Callback - wird einmalig nach msescs Millisekunden ausgefuehrt */
    glutTimerFunc(1000 / TIMER_CALLS_PS,         /* msecs - bis Aufruf von func */
                  cbTimer,                       /* func  - wird aufgerufen    */
                  glutGet(GLUT_ELAPSED_TIME));  /* value - Parameter, mit dem
                                                   func aufgerufen wird */

    /* DEBUG-Ausgabe */
    INFO (("...fertig.\n\n"));
}

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
