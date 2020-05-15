#include <stdio.h>
#include <stdlib.h>

/**
 * Waehlt anhand einer Zufallszahl eine Farbkombination aus.
 * Wird fuer die Bloecke verwendet, um diese ansehnlicher zu mache
 * @param index Zahl zwischen 1 - 5
 * @return Array mit RGB-Farben
 */
float *selectColor(int index) {

    float *colors = malloc(3);

    switch (index) {
        // Rot
        case 0:
            colors[0] = 1.0f;
            colors[1] = 0;
            colors[2] = 0;
            break;

        case 1:
            // Braun
            colors[0] = 1.0f;
            colors[1] = 0.647f;
            colors[2] = 0;
            break;

            // Gelb
        case 2:
            colors[0] = 0.941f;
            colors[1] = 0.902f;
            colors[2] = 0.549f;
            break;

            // Gruen
        case 3:
            colors[0] = 0.196f;
            colors[1] = 0.804f;
            colors[2] = 0.196f;
            break;

            // Blau
        case 4:
            colors[0] = 0.137f;
            colors[1] = 0.137f;
            colors[2] = 0.557f;
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
    printf("p, P: Spiel pausieren\n");
    printf("h, H: Hilfe anzeigen\n");
    printf("f, F: Vollbild umschalten\n");
    printf("F1: Wireframe umschalten\n");
    printf("q, Q, ESC: Spiel beenden\n");
    printf("--------------\n");
}
