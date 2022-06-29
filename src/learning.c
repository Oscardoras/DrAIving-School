#include <stdlib.h>

#include "learning.h"

#define EPSILON 0.2
#define EPSILON_LEARNING 0.01
#define GAMMA 0.01


void learning_play(Level* level, Run* run, Action action(Matrix*, Perception)) {
    bool quit = false;
    run->first = NULL;
    run->last = NULL;
    while(!quit)
    {
        Perception pct = get_entity_perception(level, level->player);
        Action act = action(level->player->q,
                    pct);
        //printf("perception : %d\nAction : %d\n", pct, act);
        quit = update_game(level);
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
    run->last->next = malloc(sizeof(struct RunListCell));
    run->last->next->previous = run->last;
    run->last = run->last->next;
    run->last->next = NULL;
    if(level->player->location.x >= level->length)
        run->last->reward = (1.0/(float)level->score);
    else
        run->last->reward = -10000.0;
}

Action e_greedy(Matrix* q, Perception perception) {
    float r = rand() / (float) RAND_MAX;
    
    if (r < EPSILON) {
        float p_max = 0.;
        Action action = 0;
        
        for (unsigned int j = 0; j < q->columns; j++) {
            float p = *get_matrix_element(q, perception, j);
            if (p > p_max) {
                p_max = p;
                action = j;
            }
        }
        
        return action;
    } else {
        r = rand() / (float) RAND_MAX;
        unsigned int j;
        for (j = 0; j < q->columns; j++)
            if (j / (float) q->columns <= r)
                return j;
        
        return j-1;
    }
}

void learning_update(Matrix* matrix, Run* run) {
    *get_matrix_element(matrix, run->last->previous->state, run->last->previous->action) += EPSILON_LEARNING * (run->last->reward - *get_matrix_element(matrix, run->last->previous->state, run->last->previous->action)); 
    for(struct RunListCell* iterator = run->last->previous->previous; iterator != NULL; iterator = iterator->previous) {
        // Defind M
        struct RunListCell* next = iterator->next;
        float highest = *get_matrix_element(matrix, next->state, 0);
        for(unsigned int j = 1; j < 5; ++j)
            if(*get_matrix_element(matrix, next->state, j) > highest)
                highest = *get_matrix_element(matrix, next->state, j);

        // M defined
        *get_matrix_element(matrix, iterator->state, iterator->action) += EPSILON_LEARNING * (iterator->reward + GAMMA* highest - *get_matrix_element(matrix, iterator->state, iterator->action)); 
    }
}

void free_run(Run* run) {
    for(struct RunListCell* it = run->first; it != NULL;) {
        struct RunListCell* tmp = it;
        it = it->next;
        free(tmp);
    }
}