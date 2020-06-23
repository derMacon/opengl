#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>

#endif

#include "variables.h"
#include <stdio.h>
#include "types.h"
#include "scene.h"
#include "drawWater.h"

State state;

/**
 * Initialisiert das Level
 * @param levelId - Dieses Level wird initialisiert
 */
void initLevel() {
    CameraView or = EMPTY_CAMERA_ORIENTATION;
    state.gameStatus = GAME_RUNNING;
    state.camera = or;
    initGrid(&state.grid, 10);
}

/**
 * Getter fuer das Spiel
 * @return
 */
State *getState(void) {
    return &state;
}

/**
 * Togglet verschiedene Dinge (z.B. FirstPerson oder Weltlich)
 * @param type - Das angegebene soll getoggled werden
 */
void toggle(enum e_ToggleTypes type) {
    switch (type) {
        case ANIMATION:
            getState()->settings.showAnimation = !getState()->settings.showAnimation;
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