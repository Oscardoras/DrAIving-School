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
    
    Matrix* matrix = NULL;
    FILE* file = fopen("config.txt", "r");
    if (file) {
        matrix = load_matrix(file);
        fclose(file);
    } else return EXIT_FAILURE;
    
    Level* level = new_level(15., 500., matrix);
    if (level == NULL) return EXIT_FAILURE;
    
    Location location;
    location.velocity = 0.1;
    location.x = 2.;
    location.y = (5. / 6) * level->width;
    level->player = new_entity(PLAYER_CAR, location, NULL);
    
    
    #ifndef LEARN
        Viewport* viewport = create_viewport(WIDTH, HEIGHT, level);
        if (viewport == NULL) {
            free_level(level);
            return EXIT_FAILURE;
        }
        
        event_loop(viewport);
        
        close_viewport(viewport);
        free_level(level);
    #else
        for(unsigned int it = 0; it < LEARN_ITERATION; ++it)
        {
            Level* level = new_level(15., 100, matrix);
            if (level == NULL) return EXIT_FAILURE;
            Location location;
            location.velocity = 0.1;
            location.x = 2.;
            location.y = (5. / 6) * level->width;
            level->player = new_entity(PLAYER_CAR, location, NULL);
            Run currentRun;
            currentRun.first = NULL;
            currentRun.last = NULL;
            FILE *file = fopen("learning", "r");
            level->player->markov = load_matrix(file);
            fclose(file);
            printf("Learning iteration %d\n", it);
            learning_play(level, &currentRun, e_greedy);
            learning_update(level->player->markov, &currentRun);
            freeRun(&currentRun);
            file = fopen("learning", "w");
            save_matrix(level->player->markov, file);
            free_matrix(level->player->markov);
            fclose(file);
            free_level(level);
        }
    #endif
    
    
    return EXIT_SUCCESS;
}
