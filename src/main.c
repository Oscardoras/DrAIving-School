#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "viewport.h"
#include "level.h"

#define WIDTH 800
#define HEIGHT 600


int main() {
    Level* level = new_level(15., 100.);
    if (level == NULL) return EXIT_FAILURE;
    
    Viewport* viewport = create_viewport(WIDTH, HEIGHT, level);
    if (viewport == NULL) {
        free_level(level);
        return EXIT_FAILURE;
    }
    
    event_loop(viewport);
    
    close_viewport(viewport);
    free_level(level);

    return EXIT_SUCCESS;
}
