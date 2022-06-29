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
#define LEARN_ITERATION 10000
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
    
    
    #ifndef LEARN
        init_level_player(level, NULL);
        
        Viewport* viewport = create_viewport(WIDTH, HEIGHT, level);
        if (viewport == NULL) {
            free_level(level);
            return EXIT_FAILURE;
        }
        
        event_loop(viewport);
        
        close_viewport(viewport);
    #else
        file = fopen("learning.txt", "r");
        if (file) {
            Matrix* q = load_matrix(file);
            fclose(file);
            
            learn(LEARN_ITERATION, q, e_greedy, q_learning, level);
            
            file = fopen("learning.txt", "w");
            if (file) {
                save_matrix(q, file);
                fclose(file);
            } else return EXIT_FAILURE;
        } else return EXIT_FAILURE;
    #endif
    
    free_level(level);
    
    return EXIT_SUCCESS;
}
