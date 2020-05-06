#include "helper.h"
#include <stdio.h>

/**
 * Generiert eine Zufallszahl
 * @param highestValue - bis zu dieser Zahl wird generiert
 * @param minValue - ab dieser Zahl wird generiert
 * @return gibt die Zufallszahl wider
 */
int genRandomNumber(int highestValue, int minValue) {
    return (rand() % (highestValue - minValue + 1)) + minValue;
}

/**
 * Liefert einen Zufallswinkel fuer den Startball
 * zwischen -45° und 45°
 * @return Winkelwert
 */
float randomBallXValue() {
    return (float) genRandomNumber(50, -50) / 100;
}

/**
 * Waehlt anhand einer Zufallszahl eine Farbkombination aus.
 * Wird fuer die Bloecke verwendet, um diese ansehnlicher zu mache
 * @param randomNumber Zahl zwischen 1 - 5
 * @return Array mit RGB-Farben
 */
float *selectColor(int randomNumber) {

    float *colors = malloc(3);

    switch (randomNumber) {
        // Lila
        case 1:
            colors[0] = 0.576f;
            colors[1] = 0.439f;
            colors[2] = 0.859f;
            break;

            // Rot
        case 2:
            colors[0] = 0.980f;
            colors[1] = 0.502f;
            colors[2] = 0.447f;
            break;

            // Blau
        case 3:
            colors[0] = 0.941f;
            colors[1] = 0.902f;
            colors[2] = 0.549f;
            break;

            // Gruen
        case 4:
            colors[0] = 0.596f;
            colors[1] = 0.984f;
            colors[2] = 0.596f;
            break;

            // Gelb
        case 5:
            colors[0] = 0.529f;
            colors[1] = 0.808f;
            colors[2] = 0.922f;
            break;
    }

    return colors;
}

/**
 * Zeigt die Hilfe fuer den Benutzer an
 */
void showHelp() {
    printf("--- HILFE ---\n");
    printf("Pfeiltaste links: Stick nach links bewegen\n");
    printf("Pfeiltaste rechts: Stick nach rechts bewegen\n");
    printf("a, A: Stick nach links bewegen\n");
    printf("d, D: Stick nach rechts bewegen\n");
    printf("F1: Wireframe umschalten\n");
    printf("p, P: Spiel pausieren\n");
    printf("h, H: Hilfe anzeigen\n");
    printf("q, Q, ESC: Spiel beenden\n");
    printf("--------------\n");
}