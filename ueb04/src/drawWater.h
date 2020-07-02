#ifndef drawWater_h
#define drawWater_h

/**
 * Initialisiert das Wasssergrid
 * @param grid - Neues Grid
 * @param size - Diese Groesse sopll es haben
 */
void initGrid(Grid *grid, GLuint size);

/**
 * Laesst das Wasser zeichnen
 */
void drawWater();

/**
 * Gibt den Index eines Vertex wider an den Stellen:
 * @param x - X-Wert
 * @param y - Y-Wert
 * @param size - Groeße des Vertex-Arrays
 * @return - Index an o.g. Stelle
 */
int getIndex(int x, int y, int size);

/**
 * Laesst das Wasser bewegen
 */
void simulateWater();

#endif