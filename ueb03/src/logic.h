
#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "types.h"

/**
 * Setzt den Bewegunsstatus des Spielers.
 * @param direction Bewegungsrichtung deren Status veraendert werden soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Rechteck bewegt sich in Richtung
 * direction, GL_FALSE->Sticks bewegt sich nicht in Richtung direction.
 */
void setPlayerMovement(enum e_Direction direction);

/**
 * Vermindert die laufende Zeit
 * Wird verwendet, um die restliche Zeit des Levels anzuzeigen
 */
void decreaseTime();

/**
 * Initialisiert das Level
 * @param levelId - Dieses Level wird initialisiert
 */
void initLevel(int levelId);

/**
 * Getter fuer das Spiel
 * @return
 */
Game *getGame();

/**
 * Togglet verschiedene Dinge (z.B. FirstPerson oder Weltlich)
 * @param type - Das angegebene soll getoggled werden
 */
void toggle(enum e_ToggleTypes type);

#endif
