#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "entity.h"
#include "viewport.h"
#include "level.h"


int main() {
    Level* level = new_level(15., 100., LINES_PER_DIR);
    if (level == NULL) return EXIT_FAILURE;
    
    Viewport* viewport = create_viewport(WIDTH, HEIGHT, level);
    if (viewport == NULL) {
        free_level(level);
        return EXIT_FAILURE;
    }
    
    level->player = new_entity(PLAYER_CAR, location_from_line(level, 0., 0., 4, 6), NULL);
    add_level_entity(level, new_entity(CAR, location_from_line(level, 2., 0., 3, 6), NULL));
    add_level_entity(level, new_entity(CAR, location_from_line(level, 3., 0., 5, 6), NULL));
    add_level_entity(level, new_entity(CAR, location_from_line(level, -1., 0., 5, 6), NULL));
    
    event_loop(viewport);
    
    close_viewport(viewport);
    free_level(level);

    return EXIT_SUCCESS;
}
