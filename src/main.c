#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "entity.h"
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
    
    Location player_pos;
    player_pos.y = level->width * 3./5.;
    player_pos.x = 0;
    player_pos.direction = true;
    player_pos.velocity = 0;
    
    level->player = new_entity(PLAYER_CAR, player_pos, NULL, 0);
    
    event_loop(viewport);
    
    close_viewport(viewport);
    free_level(level);

    return EXIT_SUCCESS;
}
