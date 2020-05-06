#include "helper.h"

int genRandomNumber(int highestValue, int minValue) {
    return (rand() % (highestValue - minValue + 1)) + minValue;
}

float randomBallXValue() {
    return (float) genRandomNumber(50, -50) / 100;
}

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

        default:
            colors[0] = 0.941f;
            colors[1] = 1.000f;
            colors[2] = 1.000f;
            break;
    }

    return colors;
}