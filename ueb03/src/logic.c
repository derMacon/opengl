#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>

#endif

#include "variables.h"
#include <stdio.h>
#include "types.h"
#include "drawObjects.h"
#include "levels.h"

Game game;

/**
 * Gibt den Block an einer bestimmten Stelle zurueck
 * @param x - x Wert der Position
 * @param y - x Wert der Position
 * @return - Gibt den Block wider (z.B. P_FREE)
 */
pushyFieldType getBlockOfPos(int x, int y) {
    return game.levelSettings.level[y][x];
}

/**
 * Neue Position fuer x oder y bestimmen
 * @param val - x oder y Wert
 * @param direction - Wenn richtung z.B nach oben ist, muss der Y Wert verringert werden
 * @param isX - true, wenn der X-Wert bearbeitet werden soll
 * @return
 */
int newPos(int val, enum e_Direction direction, GLboolean isX) {
    if ((direction == dirLeft && isX) || (direction == dirUp && !isX)) {
        val -= 1;
    } else if ((direction == dirRight && isX) || (direction == dirDown && !isX)) {
        val += 1;
    }

    return val;
}

/**
 * Prueft, ob noch Dreiecke uebrig sind
 * Falls nicht, wird das Haus gruen gemalt
 */
void handleTriangles() {
    if (game.levelSettings.numberOfTriangles > 0) {
        game.levelSettings.numberOfTriangles--;
    }

    if (game.levelSettings.numberOfTriangles == 0) {
        // MAKE HAUS GRUEN
        changeColor(GL_TRUE);
    }
}

/**
 * Hilfsfunktion fuer MoveObject. Sie ueberprueft und behandelt die Faelle mit den Tueren
 * Wir verwenden diese Funktion, um Codeverdopplung zu beseitigen
 * @param x - X-Wert des Blocks
 * @param y - Y-Wert des Blocks
 * @param newX - X-Wert des neuen Blocks
 * @param newY - Y-Wert des neuen Blocks
 * @param currentType - aktueller Blocktyp
 * @param nextType  - naechster Blocktyp
 * @param doorType - Entweder P_FREE oder P_DOOR
 * @param withNewX - Wenn true, dann werden "newX" und "newY" fuer x und y verwendet
 * @return True, wenn das Objekt bewegt wurde
 */
GLboolean
checkForDoors(int x, int y, int newX, int newY, pushyFieldType currentType, pushyFieldType nextType,
              pushyFieldType doorType, GLboolean withNewX) {
    int numberOfDoors = game.levelSettings.numberOfDoors;
    GLboolean hasMoved = GL_FALSE;

    // wenn withNewX true, dann die Werte "newX" und "newY" verwenden,
    // um die Stelle der Tuer zu finden
    int doorX = withNewX ? newX : x;
    int doorY = withNewX ? newY : y;

    Level level = levels[game.levelId - 1];

    // Alle tueren durchlaufen, um zu ueberpruefen,
    // ob eine Tuer oder Tuerschalter betreten wird
    for (int i = 0; i < numberOfDoors; ++i) {
        int levelDoorX = level.doors[i][0];
        int levelDoorY = level.doors[i][1];

        // Wenn ein Tuerschalter zu der Tuer passt
        // Box bewegen / Frei machen usw.
        if (level.doorSwitch[i][0] == doorX && level.doorSwitch[i][1] == doorY) {
            game.levelSettings.level[y][x] = currentType;
            game.levelSettings.level[newY][newX] = nextType;
            game.levelSettings.level[levelDoorY][levelDoorX] = doorType;
            hasMoved = GL_TRUE;
        }

        // Bei mehreren Switches wird Tuer geoeffnet
        if (currentType == P_DOOR_SWITCH && nextType == P_BOX_DOOR_SWITCH &&
            level.doorSwitch[i][0] == newX && level.doorSwitch[i][1] == newY) {

            game.levelSettings.level[levelDoorY][levelDoorX] = P_FREE;
            hasMoved = GL_TRUE;
        }
    }

    return hasMoved;
}

int moveObject(enum e_Direction direction, int x, int y, pushyFieldType currentTileType) {
    int newX = newPos(x, direction, GL_TRUE);
    int newY = newPos(y, direction, GL_FALSE);
    GLboolean hasMoved = GL_FALSE;
    pushyFieldType targetTileType = getBlockOfPos(newX, newY);

    if (targetTileType == P_FREE && currentTileType != P_BOX_DOOR_SWITCH) {
        // Objekt bewegen (Box, Triangle)
        game.levelSettings.level[y][x] = P_FREE;
        game.levelSettings.level[newY][newX] = currentTileType;
        hasMoved = GL_TRUE;

    } else if (targetTileType == P_DOOR_SWITCH && currentTileType == P_BOX) {
        // pruefen, welche Tuer geoeffnet werden kann
        hasMoved = checkForDoors(x, y, newX, newY, P_FREE, P_BOX_DOOR_SWITCH, P_FREE, GL_TRUE);

    } else if (targetTileType == P_TARGET && currentTileType == P_OBJECT_TRIANGLE) {
        // Dreieck beseitigen
        game.levelSettings.level[y][x] = P_FREE;

        // Pruefen, ob alle Dreiecke beseitigt wurden
        handleTriangles();
        hasMoved = GL_TRUE;

    } else if (targetTileType == P_FREE && currentTileType == P_BOX_DOOR_SWITCH) {
        // Box-Switch wird wieder zur Door-Switch
        hasMoved = checkForDoors(x, y, newX, newY, P_DOOR_SWITCH, P_BOX, P_DOOR, GL_FALSE);
    } else if (targetTileType == P_DOOR_SWITCH && currentTileType == P_BOX_DOOR_SWITCH) {
        // Mehrere Doorswitches hintereinander
        hasMoved = checkForDoors(x, y, newX, newY, P_DOOR_SWITCH, P_BOX_DOOR_SWITCH, P_DOOR, GL_FALSE);
    }

    return hasMoved;
}

/**
 * Setzt die Spielerposition
 * @param x neuer X-Wert
 * @param y neuer Y-Wert
 */
void setPlayerPos(int x, int y) {
    game.levelSettings.playerPosX = x;
    game.levelSettings.playerPosY = y;
}

/**
 * Teleportiert den Spieler
 * @param portalX - X-Wert des Portals
 * @param portalY - Y-Wert des Portals
 */
void teleportPlayer(int portalX, int portalY) {

    LevelSettings ls = game.levelSettings;

    // Level Portalpositions
    int lpX1 = ls.portal1PosX;
    int lpY1 = ls.portal1PosY;
    int lpX2 = ls.portal2PosX;
    int lpY2 = ls.portal2PosY;

    // Nur ausfuehren, wenn korrekte Anzahl an Portalen vorhanden
    // -1 wird gesetzt, wenn zuviele Portale vorhanden sind.
    if (lpX1 != -1) {
        if (portalX == lpX1 && portalY == lpY1) {
            setPlayerPos(lpX2, lpY2);
        } else if (portalX == lpX2 && portalY == lpY2) {
            setPlayerPos(lpX1, lpY1);
        }
    }
}

/**
 * Vermindert die laufende Zeit
 * Wird verwendet, um die restliche Zeit des Levels anzuzeigen
 */
void decreaseTime() {
    if (game.levelSettings.time > 1) {
        game.levelSettings.time--;
    } else {
        game.gameStatus = GAME_LOST;
    }
}

/**
 * Prieft, ob der Spieler sich in die gegebene Richtung bewegen darf.
 * @param direction - Richtung, fuer die geprueft wird
 * @return True, wenn er sich bewegen darf
 */
GLboolean playerMovementAllowed(enum e_Direction direction) {
    int pX = game.levelSettings.playerPosX;
    int pY = game.levelSettings.playerPosY;

    // Zielfeld
    int pXNew = newPos(pX, direction, GL_TRUE);
    int pYNew = newPos(pY, direction, GL_FALSE);

    // Auf diesen Block will der Spieler gehen
    int checkTile = getBlockOfPos(pXNew, pYNew);

    // Abfrage, ob der Spieler sich noch im legalen Levelbereich befindet
    if ((direction == dirUp && pY >= 1)
        || (direction == dirDown && pY < LEVEL_SIZE)
        || (direction == dirLeft && pX >= 1)
        || (direction == dirRight && pX < LEVEL_SIZE)) {

        // Zielblock behandeln
        switch (checkTile) {
            case P_BOX:
            case P_BOX_DOOR_SWITCH:
            case P_OBJECT_TRIANGLE:
                return moveObject(direction, pXNew, pYNew, checkTile);

            case P_HOUSE:
                if (game.levelSettings.numberOfTriangles == 0) {
                    game.gameStatus = GAME_WON;
                    return GL_TRUE;
                }
                break;

            case P_FREE:
            case P_DOOR_SWITCH:
                return GL_TRUE;

            case P_PORTAL:
                teleportPlayer(pXNew, pYNew);

            case P_WALL:
            case P_TARGET:
            default:
                return GL_FALSE;
        }
    }

    return GL_FALSE;
}

/**
 * Setzt den Bewegunsstatus des Spielers.
 * @param direction Bewegungsrichtung deren Status veraendert werden soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Rechteck bewegt sich in Richtung
 * direction, GL_FALSE->Sticks bewegt sich nicht in Richtung direction.
 */
void
setPlayerMovement(enum e_Direction direction) {
    if (playerMovementAllowed(direction)) {
        if (direction == dirLeft) {
            game.levelSettings.playerPosX -= 1;
        } else if (direction == dirRight) {
            game.levelSettings.playerPosX += 1;
        } else if (direction == dirUp) {
            game.levelSettings.playerPosY -= 1;
        } else if (direction == dirDown) {
            game.levelSettings.playerPosY += 1;
        }
    }
}

/**
 * Prueft, ob die Anzahl der Portale == 2 ist.
 * Bei weniger oder mehr als 2, werden die Positionen initial negativ belegt
 * @param numberOfPortals - diese Anzahl wird ueberprueft
 */
void checkForInvalidPortals(int numberOfPortals) {
    if (numberOfPortals != 2) {
        game.levelSettings.portal1PosX = -1;
        game.levelSettings.portal1PosY = -1;
        game.levelSettings.portal2PosX = -1;
        game.levelSettings.portal2PosY = -1;
    }
}

/**
 * Liest das Level aus und zaehlt die Komponenten (Portale, Dreiecke, Tueren)
 * und setzt in den Levelsettings die Werte
 */
void setObjectCoords() {

    int numberOfPortals = 0;
    int numberOfTriangles = 0;
    int numberOfDoors = 0;

    // Level durchgehen und Komponenten zaehlen und setzen
    for (int y = 0; y < LEVEL_SIZE; y++) {
        for (int x = 0; x < LEVEL_SIZE; x++) {

            // Dreiecke
            if (getBlockOfPos(x, y) == P_OBJECT_TRIANGLE) {
                numberOfTriangles++;
            }

            // Portale
            if (getBlockOfPos(x, y) == P_PORTAL) {

                if (numberOfPortals < 1) {
                    game.levelSettings.portal1PosX = x;
                    game.levelSettings.portal1PosY = y;
                } else {
                    game.levelSettings.portal2PosX = x;
                    game.levelSettings.portal2PosY = y;
                }
                numberOfPortals++;
            }

            // Tuer
            if (getBlockOfPos(x, y) == P_DOOR) {
                numberOfDoors++;
            }
        }
    }

    game.levelSettings.numberOfDoors = numberOfDoors;
    game.levelSettings.numberOfTriangles = numberOfTriangles;

    // Falls es keine Dreiecke gibt,
    // dann kann das Haus direkt gruen dargestellt werden
    if (numberOfTriangles == 0) {
        handleTriangles();
    }

    checkForInvalidPortals(numberOfPortals);
}

/**
 * Laedt die Leveldinfos in die Levelsettings
 * @param levelId - Die ID des Levels welches geladen werden soll
 */
void loadLevel(int levelId) {
    pushyFieldType (*tempLevel)[9] = NULL;
    tempLevel = levels[levelId - 1].field;

    for (int y = 0; y < LEVEL_SIZE; ++y) {
        for (int x = 0; x < LEVEL_SIZE; ++x) {
            game.levelSettings.level[y][x] = tempLevel[y][x];
        }
    }
}

/**
 * Initialisiert das Level
 * @param levelId - Dieses Level wird initialisiert
 */
void initLevel(int levelId) {
    CameraOrientation or = EMPTY_CAMERA_ORIENTATION;
    game.gameStatus = GAME_RUNNING;
    game.levelId = levelId;
    game.camera = or;
    game.levelSettings.time = levels[levelId - 1].time;
    game.levelSettings.playerPosX = levels[levelId - 1].startPos[0];
    game.levelSettings.playerPosY = levels[levelId - 1].startPos[1];
    game.lastDirection = 0;

    loadLevel(levelId);

    // Hausfarbe auf Pink setzen
    changeColor(GL_FALSE);

    // Positionen der Portale setzen
    setObjectCoords();
}

/**
 * Getter fuer das Spiel
 * @return
 */
Game *getGame(void) {
    return &game;
}
