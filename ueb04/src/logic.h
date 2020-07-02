
#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "types.h"


/**
 * Initialisiert das Level
 * @param levelId - Dieses Level wird initialisiert
 */
void initLevel();

/**
 * Getter fuer das Spiel
 * @return
 */
State *getState();

/**
 * Togglet verschiedene Dinge (z.B. FirstPerson oder Weltlich)
 * @param type - Das angegebene soll getoggled werden
 */
void toggle(enum e_ToggleTypes type);

/**
 * Aendert die Grid-Size nachdem man die Anzahl der
 * Kugeln veraendert hat
 * @param increase True, wenn es groesser werden soll
 */
void changeGridSize(GLboolean increase);

/**
 * Sorgt dafuer, dass der Index innerhalb des Grids bleibt
 * @param i - dieser Wert
 * @param decrease - Wenn der Wert verkleinert werden soll
 * @return - neuer Wert
 */
int validateIndex(int i, GLboolean decrease);

/**
 * Berechnet die Normalen fuer das Wassergrid an Stelle idx
 * @param idx
 */
void calcNormals(int x, int y, int idx);

#endif
