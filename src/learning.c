#include <stdlib.h>

#include "learning.h"

#define EPSILON 0.5


Action e_greedy(Matrix* Q, Perception perception) {
    float r = rand() / (float) RAND_MAX;
    
    if (r < EPSILON) {
        float p_max = 0.;
        Action action = 0;
        
        for (unsigned int j = 0; j < Q->columns; j++) {
            float p = *get_matrix_element(Q, perception, j);
            if (p > p_max) {
                p_max = p;
                action = j;
            }
        }
        
        return action;
    } else {
        r = rand() / (float) RAND_MAX;
        unsigned int j;
        for (j = 0; j < Q->columns; j++)
            if (j / (float) Q->columns <= r)
                return j;
        
        return j-1;
    }
}
