#ifndef __IO_H__
#define __IO_H__

/**
 * Setzen der Projektionsmatrix.
 * Setzt die Projektionsmatrix unter Berücksichtigung des Seitenverhaeltnisses
 * des Anzeigefensters, sodass das Seitenverhaeltnisse der Szene unveraendert
 * bleibt und gleichzeitig entweder in x- oder y-Richtung der Bereich von -1
 * bis +1 zu sehen ist.
 * @param aspect Seitenverhaeltnis des Anzeigefensters (In).
 */
void setProjection(GLdouble aspect);

/**
 * Registrierung der GLUT-Callback-Routinen.
 */
void registerCallbacks(void);

/**
 * Initialisiert das Programm (inkl. I/O und OpenGL) und startet die
 * Ereignisbehandlung.
 * @param title Beschriftung des Fensters
 * @param width Breite des Fensters
 * @param height Hoehe des Fensters
 * @return ID des erzeugten Fensters, 0 im Fehlerfall
 */
int
initAndStartIO(char *title, int width, int height);


// TODO: Umschreiben
typedef enum {
    mouseButton,
    mouseMotion,
    mousePassiveMotion
} CGMouseEventType;

// TODO: Umschreiben
void handleMouseEvent(int x, int y, CGMouseEventType eventType,
                      int button, int buttonState);

typedef GLfloat CGColor3f[3];

enum e_Direction getDirection();

#endif