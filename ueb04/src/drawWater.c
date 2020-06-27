#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else

#include <GL/glu.h>
#include "types.h"
#include "logic.h"
#include "debug.h"
#include "io.h"

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

            grid->vertices[idx][R] = 0.2f;
            grid->vertices[idx][G] = 0.3f;
            grid->vertices[idx][B] = 0.8f;

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

int validateIndex(int i, GLboolean decrease) {
    if (decrease && i > 0) {
        i--;
    } else if (!decrease
               && i < getState()->grid.length - 1) {
        i++;
    }
    return i;
}

void simulateWater() {
    // TODO: Werte anpassen
    const float speed = 0.1f; // auch speed
    float pillarSize = 1.5f; // speed xD
    float time = 0.99f; // force xD

    int len = getState()->grid.length;
    double *vals = malloc(sizeof(double) * (len * len));

    for (int y = 0; y < len; y++) {
        for (int x = 0; x < len; x++) {

            int index = getIndex(x, y, len);
            float force = (speed * speed) * (
                    getState()->grid.vertices[getIndex(validateIndex(x, GL_FALSE), y, len)][Y] +
                    getState()->grid.vertices[getIndex(validateIndex(x, GL_TRUE), y, len)][Y] +
                    getState()->grid.vertices[getIndex(x, validateIndex(y, GL_FALSE), len)][Y] +
                    getState()->grid.vertices[getIndex(x, validateIndex(y, GL_TRUE), len)][Y] -
                    4 * getState()->grid.vertices[getIndex(x, y, len)][Y])
                          / (pillarSize * pillarSize);

            getState()->grid.velocities[index] =
                    (getState()->grid.velocities[getIndex(x, y, len)] +force) * time;
            vals[index] =( getState()->grid.vertices[index][Y] + getState()->grid.velocities[index]) * time;
        }
    }

    for (int y = 0; y < len; y++) {
        for (int x = 0; x < len; x++) {
            int idx = getIndex(x, y, len);
            getState()->grid.vertices[idx][Y] = vals[idx];
        }
    }

    free(vals);
}


void drawWater() {
    unsigned int length = getState()->grid.length - 1;

    glDrawElements(GL_TRIANGLES,             // Primitivtyp
                   length * length * 6, // Anzahl Indizes zum Zeichnen
                   GL_UNSIGNED_INT,             // Typ der Indizes
                   getState()->grid.indices);   // Index Array
}

void drawSphere(int index) {
    glPushName(index);
    {
        glPushMatrix();
        {
            glTranslatef(
                    getState()->grid.vertices[index][X],
                    getState()->grid.vertices[index][Y],
                    getState()->grid.vertices[index][Z]);

            /* Quadric erzuegen */
            GLUquadricObj *qobj = gluNewQuadric();
            if (qobj != 0) {

                /* Normalen fuer Quadrics berechnen lassen */
                gluQuadricNormals(qobj, GLU_SMOOTH);

                /* Kugel zeichen */
                float radius = 1.0f / (getState()->grid.length * (float) 5);
                gluSphere(qobj, radius, 20, 20);

                /* Loeschen des Quadrics nicht vergessen */
                gluDeleteQuadric(qobj);
            } else {
                CG_ERROR (("Could not create Quadric\n"));
            }
        }
        glPopMatrix();
    }
    glPopName();
}

void drawSpheres() {
    int length = getState()->grid.length;

    for (int i = 0; i < length * length; i++) {
        drawSphere(i);
    }
}

void drawPickingSpheres() {
    glPushMatrix();
    {
        glScalef(5, 5, 5);
        drawSpheres();
    }
    glPopMatrix();
}
