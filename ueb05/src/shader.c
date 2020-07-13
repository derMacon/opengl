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
#include "io.h"

/* Bibliothek um Bilddateien zu laden. Es handelt sich um eine
 * Bibliothek, die sowohl den Header als auch die Quelle in einer Datei
 * bereitstellt. Die Quelle kann durch die Definition des Makros
 * STB_IMAGE_IMPLEMENTATION eingebunden werden. Genauere Informationen
 * sind der "stb_image.h" zu entnehmen.
 * 
 * Quelle: https://github.com/nothings/stb */
// define global functions as static inline

#include <stb/stb_image.h>

/* ---- Globale Variablen ---- */

/** Buffer-Objekt, um die Vertizes zu speichern. */
static GLuint g_arrayBuffer;

/** Vertex-Array-Objekt, um die Attribut-Pointer zu speichern. */
static GLuint g_vertexArrayObject;

/** Programm-Objekt zum Rendern */
static GLuint g_program;

/** Anhebung der Vertizes */
static float elevation;

/** Textur */
static GLuint g_texture_WorldMap;
static GLuint g_texture_HeightMap;

/** Location der uniform-Variable "ModelView" */
static GLint g_locationModelViewMatrix;

/** Location der uniform-Variable "Elevation" */
static GLint g_locationElevation;

/** Location der uniform-Variable "Texture" */
static GLuint g_locationTexture;
static GLuint g_locationHeightMap;

static Settings settings;

Settings *getSettings() {
    return &settings;
}

float *getElevation() {
    return &elevation;
}

Vertex vert[GRID_LENGTH * GRID_LENGTH * 6];

/**
 * Zeichen-Funktion.
 * Stellt die Szene dar.
 * Ausgabe eines Rechtecks.
 */
void
drawScene(void) {
    float viewMatrix[16];
    float t = (float) glutGet(GLUT_ELAPSED_TIME) / 2000;

    const float distance = 3;

    if (settings.showBreak) {
        t = 0;
    }

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
    glUniform1f(g_locationElevation, elevation);

    /* Übergeben der Textur an den Shader.
     * Texturen werden nicht direkt an den Shader übergeben, sondern
     * zuerst an eine Textureinheit gebunden. Anschließend wird dem
     * Programm nur der Index der Textureinheit übergeben. */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texture_WorldMap);
    glUniform1i(g_locationTexture, worldMap);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_texture_HeightMap);
    glUniform1i(g_locationHeightMap, heightMap);

    /* Aktivieren des Vertex-Array-Objekts (VAO).
     * Hiermit werden alle Attribut-Pointer aktiv, die auf diesem VAO
     * gesetzt wurden. */
    glBindVertexArray(g_vertexArrayObject);

    /* Rendern der Dreiecke.
     * Ab dem ersten Dreieck im Buffer werden alle 8 Dreiecke gerendert.
     * Dem Draw-Command wird jedoch die Anzahl der Vertizes übergeben, die
     * gezeichnet werden sollen. */
    glDrawArrays(GL_TRIANGLES, 0, ((GRID_LENGTH) * (GRID_LENGTH) * 6));

    /* Zurücksetzen des OpenGL-Zustands, um Seiteneffekte zu verhindern */
    glBindVertexArray(0);
    glUseProgram(0);
}

/**
 * Setzt die Textur und die Hoehe fuer ein Tile des Meshes
 * @param idx - Derzeitger Index
 */
void setTextureAndHeight(int idx) {
    for (int i = 0; i < 6; ++i) {
        vert[idx + i].s = (vert[idx + i].x + OFFSET_X) / GRID_LENGTH_Z;
        vert[idx + i].t = (vert[idx + i].z + OFFSET_Z) / GRID_LENGTH_X;
        vert[idx + i].y = 0;
    }
}

/**
 * Erzeugt ein Tile des gesamten Meshes
 * @param x Derzeitge x-Position im Mesh
 * @param z Derzeitge y-Position im Mesh
 * @param idx  Derzeitger im Mesh Array
 */
void initSingleTile(int x, int z, int idx) {
    // Erstes Dreieck
    vert[idx].x = GRID_LENGTH_Z / GRID_LENGTH * (GLfloat) x - OFFSET_X;
    vert[idx].z = GRID_LENGTH_X / GRID_LENGTH * (GLfloat) z - OFFSET_Z;

    vert[idx + 1].x = vert[idx].x;
    vert[idx + 1].z = vert[idx].z + TILE_HEIGHT;

    vert[idx + 2].x = vert[idx].x + TILE_WIDTH;
    vert[idx + 2].z = vert[idx].z + TILE_HEIGHT;

    // Zweites Dreieck
    vert[idx + 3].x = vert[idx].x + TILE_WIDTH;
    vert[idx + 3].z = vert[idx].z + TILE_HEIGHT;

    vert[idx + 4].x = vert[idx].x + TILE_WIDTH;
    vert[idx + 4].z = vert[idx].z;

    vert[idx + 5].x = vert[idx].x;
    vert[idx + 5].z = vert[idx].z;

    setTextureAndHeight(idx);
}

/**
 * Initialisiert das Wasssergrid
 * @param grid - Neues Grid
 * @param size - Diese Groesse sopll es haben
 */
void initGridArray() {
    int idx = 0;
    for (int z = 0; z < GRID_LENGTH; z++) {
        for (int x = 0; x < GRID_LENGTH; x++) {
            initSingleTile(x, z, idx);
            idx += 6;
        }
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
    initGridArray();

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
        g_locationHeightMap = glGetUniformLocation(g_program, "HeightMap");

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
        for (int i = 0; i < 2; ++i) {
            int width, height, comp;
            GLubyte *data;
            if (i == 0) {
                data = stbi_load("../content/textures/SS20_heightmap.jpg", &width, &height, &comp, 4);

                glGenTextures(1, &g_texture_HeightMap);
                glBindTexture(GL_TEXTURE_2D, g_texture_HeightMap);
            } else {
                data = stbi_load("../content/textures/SS20_worldmap.png", &width, &height, &comp, 4);

                glGenTextures(1, &g_texture_WorldMap);
                glBindTexture(GL_TEXTURE_2D, g_texture_WorldMap);
            }

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, i);

            stbi_image_free(data);
        }
    }

    {
        settings.showNormals = GL_FALSE;
        settings.showTextures = GLU_TRUE;
        settings.showWireframe = GL_FALSE;
        settings.showFullscreen = GL_FALSE;
        settings.showWorldlight = GL_TRUE;
        settings.showBreak = GL_FALSE;
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
