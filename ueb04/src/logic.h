
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

#endif
