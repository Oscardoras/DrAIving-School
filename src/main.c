#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "learning.h"
#include "matrix.h"
#include "entity.h"
#include "viewport.h"
#include "level.h"


//#define LEARN
#define LEARN_ITERATION 500

int main() {
    srand(time(NULL));
    
    Level* level = new_level(15., 10000., LINES_PER_DIR);
    if (level == NULL) return EXIT_FAILURE;
    
    #ifndef LEARN
    Viewport* viewport = create_viewport(WIDTH, HEIGHT, level);
    if (viewport == NULL) {
        free_level(level);
        return EXIT_FAILURE;
    }
    #endif
    
    Matrix* matrix = NULL;
    FILE* file = fopen("config.txt", "r");
    if(file) {
        matrix = load_matrix(file);
        fclose(file);
    }
    
    
    
    level->player = new_entity(PLAYER_CAR, location_from_line(level, 0., 1., 4, 6), NULL);
    add_level_entity(level, new_entity(CAR, location_from_line(level, 2., 1., 3, 6), matrix));
    add_level_entity(level, new_entity(CAR, location_from_line(level, 3., 1., 5, 6), matrix));
    add_level_entity(level, new_entity(CAR, location_from_line(level, -1., 1., 5, 6), matrix));
    
    #ifndef LEARN
    {
        event_loop(viewport);
        close_viewport(viewport);
    }
    #else
    {
        Run currentRun;
        currentRun.first = NULL;
        currentRun.last = NULL;
        for(unsigned int it = 0; it < LEARN_ITERATION; ++it)
        {
            learning_play(level, &currentRun, e_greedy);
        }
        FILE *file = fopen("learning", "w");
        save_matrix(level->player->markov, file);
        fclose(file);
    }
    #endif
    free_level(level);

    return EXIT_SUCCESS;
}
