#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "learning.h"
#include "viewport.h"

#define WIDTH 800
#define HEIGHT 600
#define LEVEL_WIDTH 15.
#define LEVEL_LENGTH 500.


int main() {
    srand(time(NULL));
    
    Matrix* matrix = NULL;
    FILE* file = fopen("config.txt", "r");
    if (file) {
        matrix = load_matrix(file);
        fclose(file);
    } else return EXIT_FAILURE;
    
    Level* level = new_level(LEVEL_WIDTH, LEVEL_LENGTH, matrix);
    if (level == NULL) return EXIT_FAILURE;


    #ifdef LEARN
        file = fopen("base.txt", "r");
        if (file) {
            Matrix* q = load_matrix(file);
            fclose(file);
            
            learn(LEARN, q, e_greedy, sarsa, level);
            
            file = fopen("learning.txt", "w");
            if (file) {
                save_matrix(q, file);
                fclose(file);
            } else return EXIT_FAILURE;
        } else return EXIT_FAILURE;
    #else
        init_level_player(level, NULL);
        
        Viewport* viewport = create_viewport(WIDTH, HEIGHT, level);
        if (viewport == NULL) {
            free_level(level);
            return EXIT_FAILURE;
        }
        
        event_loop(viewport);
        close_viewport(viewport);
    #endif
    
    
    if (level->player->q != NULL) free_matrix(level->player->q);
    free_matrix(matrix);
    free_level(level);
    
    return EXIT_SUCCESS;
}
