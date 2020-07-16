#ifndef main_h
#define main_h
#include "variables.h"

/**
 * Zeichen-Funktion.
 */
void drawScene(void);

/**
 * Getter fuer die Settings
 * @return die Settings
 */
Settings *getSettings();

/**
 * Getter fuer die Elevation
 * @return die Elevation?
 */
float *getElevation();

#endif