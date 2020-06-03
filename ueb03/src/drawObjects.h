#ifndef UEB02_DRAWOBJECTS_H
#define UEB02_DRAWOBJECTS_H

/**
 * Zeichnet die Mauer
 */
void drawWall();

/**
 * Zeichnet die Box
 */
void drawBox();

/**
 * Zeichnet den freien Block
 */
void drawFreeBlock();

/**
 * Zeichnet die Tuer
 */
void drawDoor();


/**
 * Zeichnet ein Dreieckobjekt
 */
void drawTriangleOject();

/**
 * Zeichnet den blauen Kasten
 * (Ziel fuer die Dreiecke)
 */
void drawFinish();

/**
 * Zeichnet den Tuerschalter
 */
void drawDoorSwitch();

/**
 * Zeichnet die Portale
 */
void drawPortals();

/**
 * Zeichnet das Haus
 */
void drawHouse();

/**
 * Zeichnet den Spieler
 */
void drawPlayer();

/**
 * Aendert die Farbe des Hauses
 * @param isGreenHouse - wenn True, dann Farbe Gruen
 */
void changeColor(GLboolean isGreenHouse);


void drawCube(pushyFieldType fieldType);

void drawAxes();

#endif //UEB02_DRAWOBJECTS_H
