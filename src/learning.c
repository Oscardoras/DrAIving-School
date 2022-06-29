#include <stdlib.h>

#include "learning.h"

#define EPSILON 0.5
#define GAMMA 0.5

void learning_play(Level* level, Run* run, Action action(Matrix*, Perception)) {
    bool collision = true;
    run = NULL;
    run = malloc(sizeof(Run));
    run->first = NULL;
    run->last = NULL;
    while(!collision)
    {
        Perception pct = get_entity_perception(level, level->player);
        Action act = action(level->player->markov,
                    pct);
        
        collision = update_game(level);
        make_action(level, level->player,
                act
                );
        if(!run->last)
        {
            run->last = malloc(sizeof(struct RunListCell));
            run->first = run->last;
            run->first->next = NULL;
            run->first->previous = NULL;
        }
        else{
            run->last->next = malloc(sizeof(struct RunListCell));
            run->last->next->previous = run->last;
            run->last = run->last->next;
            run->last->next = NULL;
        }
        run->last->reward = 0;
        run->last->action = act;
        run->last->state = pct;
    }
    // todo : Ajouter reward finale (temps)
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
    *get_matrix_element(matrix, run->last->previous->state, run->last->previous->action) += EPSILON * (run->last->reward - *get_matrix_element(matrix, run->last->previous->state, run->last->previous->action)); 
    for(struct RunListCell* iterator = run->last-2; iterator != NULL; iterator = iterator->previous)
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