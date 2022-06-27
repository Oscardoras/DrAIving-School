#include <stdio.h>
#include <stdlib.h>

#include "level.h"


int main() {
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
    
    

    return EXIT_SUCCESS;
}
