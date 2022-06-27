#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "viewport.h"
#include "level.h"

#define WIDTH 800
#define HEIGHT 600

int main() {
    Viewport* viewport = create_viewport(WIDTH, HEIGHT, NULL);
    //Vérifie aussi les problèmes d'init
    
    /*
    Matrix* m = NULL;
    FILE* file = fopen("config.txt", "r");
    if (file) {
        m = load_matrix(file);
        fclose(file);
    }
    
    for (unsigned int i = 0; i < m->lines; i++) {
        for (unsigned int j = 0; j < m->columns; j++) {
            printf("%f ", *get_matrix_element(m, i, j));
        }
        printf("\n");
    }
    */
   
    event_loop(viewport);
    close_viewport(viewport);

    return EXIT_SUCCESS;
}