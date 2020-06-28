#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>

#endif

#include "variables.h"
#include "types.h"
#include "drawWater.h"

State state;

/**
 * Initialisiert das Level
 * @param levelId - Dieses Level wird initialisiert
 */
void initLevel() {
    CameraView or = EMPTY_CAMERA_ORIENTATION;
    state.gameStatus = GAME_RUNNING;
    state.settings.showSpheres = GL_TRUE;
    state.camera = or;
    initGrid(&state.grid, INITIAL_GRID_SIZE);
}

/**
 * Getter fuer das Spiel
 * @return
 */
State *getState(void) {
    return &state;
}

void freeWater() {
    free(getState()->grid.velocities);
    free(getState()->grid.vertices);
    free(getState()->grid.indices);
}

void changeGridSize(GLboolean increase) {
    int currentSize = getState()->grid.length;
    double *vel = malloc(sizeof(double) * (currentSize * currentSize));
    GLboolean changed = GL_FALSE;

    for (int y = 0; y < currentSize; y++) {
        for (int x = 0; x < currentSize; x++) {
            int idx = getIndex(x, y, currentSize);
            vel[idx] = getState()->grid.vertices[idx][Y];
        }
    }

    if (increase && currentSize + 1 <= MAX_GRID_SIZE) {
        getState()->grid.length = (currentSize + 1);
        changed = GL_TRUE;
    } else if (!increase && currentSize - 1 >= MIN_GRID_SIZE) {
        getState()->grid.length = (currentSize - 1);
        changed = GL_TRUE;
    }

    if (changed) {
        freeWater();

        // Wenn wir increasen, ist das neue Array groesser als vals
        int len = increase
                  ? currentSize
                  : getState()->grid.length;

        initGrid(&getState()->grid, getState()->grid.length);

        for (int y = 0; y < len; y++) {
            for (int x = 0; x < len; x++) {
                int idx = getIndex(x, y, getState()->grid.length);
                int idx2 = getIndex(x, y, currentSize);
                getState()->grid.vertices[idx][Y] = vel[idx2];
            }
        }
    }
    free(vel);
}

/**
 * Togglet verschiedene Dinge (z.B. FirstPerson oder Weltlich)
 * @param type - Das angegebene soll getoggled werden
 */
void toggle(enum e_ToggleTypes type) {
    switch (type) {
        case SPHERES:
            getState()->settings.showSpheres = !getState()->settings.showSpheres;
            break;
        case NORMALS:
            getState()->settings.showNormals = !getState()->settings.showNormals;
            break;
        case WORLDLIGHT:
            getState()->settings.showWorldLight = !getState()->settings.showWorldLight;

            if (getState()->settings.showWorldLight) {
                glEnable(GL_LIGHT0);
            } else {
                glDisable(GL_LIGHT0);
            }
            break;
        case SPOTLIGHT:
            getState()->settings.showSpotLight = !getState()->settings.showSpotLight;

            if (getState()->settings.showSpotLight) {
                glEnable(GL_LIGHT1);
            } else {
                glDisable(GL_LIGHT1);
            }
            break;
        case WIREFRAME:
            getState()->settings.showWireframe = !getState()->settings.showWireframe;
            if (getState()->settings.showWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            break;
        case FULLSCREEN:
            getState()->settings.showFullScreen = !getState()->settings.showFullScreen;

            if (getState()->settings.showFullScreen) {
                glutFullScreen();
            } else {
                glutPositionWindow(glutGet(GLUT_SCREEN_WIDTH) / 2, glutGet(GLUT_SCREEN_HEIGHT) / 2);
            }
            break;
        default:
            break;
    }
}