#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else

#include <GL/glu.h>
#include "types.h"
#include "logic.h"

#endif

int getIndex(int x, int y, int size) {
    return (y * size) + x;
}

void initGrid(Grid *grid, GLuint size) {

    GLuint gridSize = size * size;
    grid->length = size;

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

            grid->vertices[idx][R] = 0.5f;
            grid->vertices[idx][G] = 0.5f;
            grid->vertices[idx][B] = 0.5f;

            // todo wtf
            if (y < size - 1 && x < size - 1) {
                GLuint iIdx = (y * (size - 1) + x) * 6;
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

    glVertexPointer(3, GL_DOUBLE, sizeof(Vertex), &(grid->vertices[0][X]));
    glColorPointer(3, GL_DOUBLE, sizeof(Vertex), &(grid->vertices[0][R]));
}

void drawWater() {
    unsigned int length = getState()->grid.length - 1;

        glDrawElements(GL_TRIANGLES,             // Primitivtyp
                       length * length * 6, // Anzahl Indizes zum Zeichnen
                       GL_UNSIGNED_INT,             // Typ der Indizes
                       getState()->grid.indices);   // Index Array

}
