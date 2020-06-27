#ifndef drawWater_h
#define drawWater_h

void initGrid(Grid *grid, GLuint size);

void drawWater();

void drawSpheres();

int getIndex(int x, int y, int size);

void simulateWater();

#endif