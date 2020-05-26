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