#ifndef UEB01_HELPER_H
#define UEB01_HELPER_H

#include <stdlib.h>

/**
 * Waehlt anhand einer Zufallszahl eine Farbkombination aus.
 * Wird fuer die Bloecke verwendet, um diese ansehnlicher zu mache
 * @param index Zahl zwischen 1 - 5
 * @return Array mit RGB-Farben
 */
float *selectColor(int index);
#endif