#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "io.h"

int
main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    /* Initialisierung des I/O-Sytems
       (inkl. Erzeugung des Fensters und Starten der Ereignisbehandlung). */
    if (!initAndStartIO
            ("Pushy", 1500, 800)) {
        fprintf(stderr, "Initialisierung fehlgeschlagen!\n");
        return 1;
    } else {
        return 0;
    }
}




