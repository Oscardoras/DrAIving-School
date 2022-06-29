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
#define EPS 0.99
#define LEARN_ITERATION 1000000

int main() {
    srand(time(NULL));
    
    Matrix* matrix = NULL;
    FILE* file = fopen("config.txt", "r");
    if (file) {
        matrix = load_matrix(file);
        fclose(file);
    } else return EXIT_FAILURE;
    
    Level* level = NULL;
    Entity* player = NULL;
    Location location;
    
    #ifndef LEARN
        player = new_entity(PLAYER_CAR, location, NULL);
        level = new_level(15., 500, matrix);
        player->location.x = 2.;
        player->location.velocity = 1.5;
        player->location.y = (5. / 6) * level->width;
        level->player = player;
        Viewport* viewport = create_viewport(WIDTH, HEIGHT, level);
        if (viewport == NULL) {
            free_level(level);
            return EXIT_FAILURE;
        }
        
        event_loop(viewport);
        
        close_viewport(viewport);
        free_level(level);
    #else
        player = new_entity(PLAYER_CAR, location, NULL);
        level = NULL;
        float eps = EPS;
        
        file = fopen("learning.txt", "r");
        if(file) {
            player->q = load_matrix(file);
            fclose(file);
        }
        for(unsigned int it = 0; it < LEARN_ITERATION; ++it)    {
            level = new_level(15., 500, matrix);
            if (!level) return EXIT_FAILURE;
            
            player->location.x = 2.;
            player->location.velocity = 1.5;
            player->location.y = (5. / 6) * level->width;
            level->player = player;
            
            Run currentRun;
            currentRun.first = NULL;
            currentRun.last = NULL;
            
            
            printf("Learning iteration %d\n", it);
            learning_play(level, &currentRun, eps);
            learning_update(level->player->q, &currentRun);
            free_run(&currentRun);
            
            free_level(level);
            
            if(!(it%10000)) eps *= EPS;
        }
        file = fopen("learning.txt", "w");
        if(file) {
            save_matrix(player->q, file);
            fclose(file);
        }
        
        free_matrix(player->q);
        free_entity(player);
                
    #endif
    
    
    return EXIT_SUCCESS;
}
