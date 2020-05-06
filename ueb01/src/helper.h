#ifndef UEB01_HELPER_H
#define UEB01_HELPER_H

#include <stdlib.h>

/**
 * Generiert eine Zufallszahl
 * @param highestValue - bis zu dieser Zahl wird generiert
 * @param minValue - ab dieser Zahl wird generiert
 * @return gibt die Zufallszahl wider
 */
int genRandomNumber(int highestValue, int minValue);

/**
 * Liefert einen Zufallswinkel fuer den Startball
 * zwischen -45° und 45°
 * @return Winkelwert
 */
float randomBallXValue();

/**
 * Waehlt anhand einer Zufallszahl eine Farbkombination aus.
 * Wird fuer die Bloecke verwendet, um diese ansehnlicher zu mache
 * @param randomNumber Zahl zwischen 1 - 5
 * @return Array mit RGB-Farben
 */
float *selectColor(int randomNumber);
#endif //UEB01_HELPER_H
