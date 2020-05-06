#include "helper.h"

int genRandomNumber(int highestValue, int minValue) {
    return (rand() % highestValue) + minValue;
}