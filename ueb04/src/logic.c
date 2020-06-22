#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>

#endif

#include "variables.h"
#include <stdio.h>
#include "types.h"
#include "drawObjects.h"
#include "scene.h"

Game game;



/**
 * Initialisiert das Level
 * @param levelId - Dieses Level wird initialisiert
 */
void initLevel() {
    CameraView or = EMPTY_CAMERA_ORIENTATION;
    game.gameStatus = GAME_RUNNING;
    game.camera = or;
}

/**
 * Getter fuer das Spiel
 * @return
 */
Game *getGame(void) {
    return &game;
}

/**
 * Togglet verschiedene Dinge (z.B. FirstPerson oder Weltlich)
 * @param type - Das angegebene soll getoggled werden
 */
void toggle(enum e_ToggleTypes type) {
    switch (type) {
        case FIRSTPERSON:
            getGame()->settings.showFirstPerson = !getGame()->settings.showFirstPerson;
            break;
        case ANIMATION:
            getGame()->settings.showAnimation = !getGame()->settings.showAnimation;
            break;
        case NORMALS:
            getGame()->settings.showNormals = !getGame()->settings.showNormals;
            initDisplayList();
            break;
        case WORLDLIGHT:
            getGame()->settings.showWorldLight = !getGame()->settings.showWorldLight;

            if (getGame()->settings.showWorldLight) {
                glEnable(GL_LIGHT0);
            } else {
                glDisable(GL_LIGHT0);
            }
            break;
        case SPOTLIGHT:
            getGame()->settings.showSpotLight = !getGame()->settings.showSpotLight;

            if (getGame()->settings.showSpotLight) {
                glEnable(GL_LIGHT1);
            } else {
                glDisable(GL_LIGHT1);
            }
            break;
        case WIREFRAME:
            getGame()->settings.showWireframe = !getGame()->settings.showWireframe;
            if (getGame()->settings.showWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            break;
        case FULLSCREEN:
            getGame()->settings.showFullScreen = !getGame()->settings.showFullScreen;

            if (getGame()->settings.showFullScreen) {
                glutFullScreen();
            } else {
                glutPositionWindow(glutGet(GLUT_SCREEN_WIDTH) / 2, glutGet(GLUT_SCREEN_HEIGHT) / 2);
            }
            break;
        default:
            break;
    }
}