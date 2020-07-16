/* ---- System Header einbinden ---- */
#include <stdio.h>
#include <stddef.h>

#include "utility.h"
#include "main.h"
#include <stb/stb_image.h>

/** Buffer-Objekt, um die Vertizes zu speichern. */
static GLuint g_arrayBuffer;

/** Vertex-Array-Objekt, um die Attribut-Pointer zu speichern. */
static GLuint g_vertexArrayObject;

/** Programm-Objekt zum Rendern */
static GLuint g_program;

/** Textur */
static GLuint g_texture_WorldMap;
static GLuint g_texture_HeightMap;

/** Location der uniform-Variable "ModelView" */
static GLint g_locationModelViewMatrix;

/** Location der uniform-Variable "Elevation" */
static GLint g_locationElevation;

static GLint g_locationPhong;

/** Location der uniform-Variable "Texture" */
static GLuint g_locationTexture;
static GLuint g_locationHeightMap;
static GLuint g_locationColors;
static GLuint g_locationCamPos;
static GLuint g_locationShowTexture;
static GLuint g_locationShowNormals;

static float time = 0;
Vertex vert[GRID_LENGTH * GRID_LENGTH * 6];

/**
 * Zeichen-Funktion.
 */
void
drawScene(void) {

    float viewMatrix[16];
    const float distance = 3;

    // Wen nicht pausiert, die Position der Kamera berechnen
    if (!getSettings()->showBreak) {
        time = (float) glutGet(GLUT_ELAPSED_TIME) / CAMERA_SPEED;
    }

    float x = distance * sinf(time);
    float z = distance * cosf(time);

    lookAt(x, 1, z, 0, 0, 0, 0, 1, 0, viewMatrix);

    getSettings()->camPos.x = x;
    getSettings()->camPos.y = -3;
    getSettings()->camPos.z = z;

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
    glUniform1f(g_locationElevation, *getElevation());
    glUniform1f(g_locationPhong, getSettings()->showPhong ? 1.0f : 0);

    /* Übergeben der Textur an den Shader.
     * Texturen werden nicht direkt an den Shader übergeben, sondern
     * zuerst an eine Textureinheit gebunden. Anschließend wird dem
     * Programm nur der Index der Textureinheit übergeben. */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texture_WorldMap);
    glUniform1i(g_locationTexture, worldMap);

    // Kameraposition an Shader uebergeben,
    // damit das Licht mit der Kamera mitgeht
    CamPos cp = getSettings()->camPos;
    float pos[3] = {cp.x, cp.y, cp.z};

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_texture_HeightMap);
    glUniform1i(g_locationHeightMap, heightMap);
    glUniform1f(g_locationColors, getSettings()->color);
    glUniform1fv(g_locationCamPos, 3, pos);
    glUniform1f(g_locationShowTexture, getSettings()->showTextures ? 1 : 0);
    glUniform1f(g_locationShowNormals, getSettings()->showNormals ? 1 : 0);

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
void initScene(void) {
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
        g_locationPhong = glGetUniformLocation(g_program, "showPhong");
        g_locationTexture = glGetUniformLocation(g_program, "Texture");
        g_locationHeightMap = glGetUniformLocation(g_program, "HeightMap");
        g_locationColors = glGetUniformLocation(g_program, "colorType");
        g_locationCamPos = glGetUniformLocation(g_program, "camPos");
        g_locationShowTexture = glGetUniformLocation(g_program, "showTexture");
        g_locationShowNormals = glGetUniformLocation(g_program, "showNormals");

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
        perspective(60, (GLfloat) glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), 0.01f, 5.0f,
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
        // Settings intial setzen
        getSettings()->showNormals = GL_FALSE;
        getSettings()->showTextures = GLU_TRUE;
        getSettings()->showWireframe = GL_FALSE;
        getSettings()->showFullscreen = GL_FALSE;
        getSettings()->showPhong = GL_TRUE;
        getSettings()->showBreak = GL_FALSE;
        getSettings()->color = COLOR_NORMAL;

    }
}