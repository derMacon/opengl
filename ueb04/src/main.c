#include <stdio.h>
#include "io.h"

int
main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    /* Initialisierung des I/O-Sytems
       (inkl. Erzeugung des Fensters und Starten der Ereignisbehandlung). */
    if (!initAndStartIO
            ("Wasser-Animation", 1500, 800)) {
        fprintf(stderr, "Initialisierung fehlgeschlagen!\n");
        return 1;
    } else {
        return 0;
    }
}




