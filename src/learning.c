#include <stdlib.h>

#include "learning.h"

#define EPSILON 0.5
#define GAMMA 0.5

void learning_play(Level* level, Run* run, Action action) {

}

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

void learning_update(Matrix* matrix, Run* run)
{
    *get_matrix_element(matrix, run->last->state, run->last->action) += EPSILON * (1 - *get_matrix_element(matrix, run->last->state, run->last->action)); 
    for(struct RunListCell* iterator = run->last-1; iterator != NULL; iterator = iterator->previous)
    {
        // Defind M
        struct RunListCell* next = iterator->next;
        float highest = *get_matrix_element(matrix, next->state, 0);
        unsigned int highestid = 0;
        for(unsigned int j = 1; j < 5; ++j)
        {
            if(*get_matrix_element(matrix, next->state, j) > highest)
            {
                highestid = j;
                highest = *get_matrix_element(matrix, next->state, j);
            }
        }
        // M defined
        *get_matrix_element(matrix, iterator->state, iterator->action) += EPSILON * (iterator->reward + GAMMA* highest - *get_matrix_element(matrix, iterator->state, iterator->action)); 
    }
}