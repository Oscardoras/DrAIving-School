#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "viewport.h"

#define WIDTH 800
#define HEIGHT 600


int main() {
    Viewport* viewport = create_viewport(WIDTH, HEIGHT, NULL);
    //Vérifie aussi les problèmes d'init
    
    event_loop(viewport);
    close_viewport(viewport);

    return EXIT_SUCCESS;
}
