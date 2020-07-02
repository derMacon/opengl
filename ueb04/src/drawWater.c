#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "types.h"
#include "logic.h"
#include "texture.h"

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

/**
 * Initialisiert das Wasssergrid
 * @param grid - Neues Grid
 * @param size - Diese Groesse sopll es haben
 */
void initGrid(Grid *grid, GLuint size) {

    GLuint gridSize = size * size;
    grid->length = size;

    // Speicher reservieren
    grid->vertices = malloc(gridSize * sizeof(Vertex));
    grid->velocities = malloc(gridSize * sizeof(double));
    grid->indices = malloc(gridSize * 6 * sizeof(GLuint));

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int idx = getIndex(x, y, size);

            // Werte setzen
            grid->velocities[idx] = 0.0f;

            grid->vertices[idx][X] = 0.5f - ((double) x) / ((double) size - 1);
            grid->vertices[idx][Y] = 0.0f;
            grid->vertices[idx][Z] = 0.5f - ((double) y) / ((double) size - 1);

            grid->vertices[idx][NX] = 0.0f;
            grid->vertices[idx][NY] = 1.0f;
            grid->vertices[idx][NZ] = 0.0f;

            grid->vertices[idx][R] = 0.2f;
            grid->vertices[idx][G] = 0.3f;
            grid->vertices[idx][B] = 0.8f;

            grid->vertices[idx][TX] = (double) x / size - 1;
            grid->vertices[idx][TY] = (double) y / size - 1;

            // Indexarray
            int max = size - 1;
            int noOfIndex = 6;
            if (x < max && y < max) {
                GLuint iIdx = (y * (max) + x) * noOfIndex;

                grid->indices[iIdx] = getIndex(x, y, size);
                grid->indices[iIdx + 1] = getIndex(x, y + 1, size);
                grid->indices[iIdx + 2] = getIndex(x + 1, y, size);

                grid->indices[iIdx + 3] = getIndex(x + 1, y, size);
                grid->indices[iIdx + 4] = getIndex(x, y + 1, size);
                grid->indices[iIdx + 5] = getIndex(x + 1, y + 1, size);
            }
        }
    }

    getState()->grid = *grid;

    // Pointer setzen
    glVertexPointer(3, GL_DOUBLE, sizeof(Vertex), &(grid->vertices[0][X]));
    glColorPointer(3, GL_DOUBLE, sizeof(Vertex), &(grid->vertices[0][R]));
    glNormalPointer(GL_DOUBLE, sizeof(Vertex), &(grid->vertices[0][NX]));
    glTexCoordPointer(2, GL_DOUBLE, sizeof(Vertex), &(grid->vertices[0][TX]));
}

/**
 * Laesst das Wasser bewegen
 */
void simulateWater() {

    //Ausbreitungsgschwindigkeit
    const float c = 0.1f;
    // Saeulenbreite
    float h = 1.5f;
    // vergangene Zeit
    float t = 0.99f;

    int len = getState()->grid.length;
    double *vals = malloc(sizeof(double) * (len * len));

    for (int y = 0; y < len; y++) {
        for (int x = 0; x < len; x++) {

            int index = getIndex(x, y, len);
            float force = (c * c) * (
                    getState()->grid.vertices[getIndex(validateIndex(x, GL_FALSE), y, len)][Y] +
                    getState()->grid.vertices[getIndex(validateIndex(x, GL_TRUE), y, len)][Y] +
                    getState()->grid.vertices[getIndex(x, validateIndex(y, GL_FALSE), len)][Y] +
                    getState()->grid.vertices[getIndex(x, validateIndex(y, GL_TRUE), len)][Y] -
                    4 * getState()->grid.vertices[getIndex(x, y, len)][Y])
                          / (h * h);

            // Lokale Kopie, da wir rechnen
            // Hoehe und Geschwindigkeit
            getState()->grid.velocities[index] =
                    (getState()->grid.velocities[getIndex(x, y, len)] + force) * t;
            vals[index] = (getState()->grid.vertices[index][Y] + getState()->grid.velocities[index]) * t;
        }
    }

    // Lokale Kopie wieder setzen
    for (int y = 0; y < len; y++) {
        for (int x = 0; x < len; x++) {
            int idx = getIndex(x, y, len);
            getState()->grid.vertices[idx][Y] = vals[idx];
            calcNormals(x, y, vals[idx]);
        }
    }

    free(vals);
}

/**
 * Laesst das Wasser zeichnen
 */
void drawWater() {
    unsigned int length = getState()->grid.length - 1;

    bindTexture(texWater);
    glDrawElements(GL_TRIANGLES,             // Primitivtyp
                   length * length * 6, // Anzahl Indizes zum Zeichnen
                   GL_UNSIGNED_INT,             // Typ der Indizes
                   getState()->grid.indices);   // Index Array
}

