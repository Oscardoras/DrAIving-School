#include <stdlib.h>

#include "learning.h"

#define EPSILON 0.99
#define EPSILON_LEARNING 0.0001
#define GAMMA 0.0001

int length_run(Run* run) {
    struct RunListCell* cour = run->first;
    int length = 0;
    while(cour) cour = cour->next;
    return length;
}

void learn(unsigned long n, Matrix* q, Action action(Matrix* q, Perception p, float eps), void learning(Matrix* q, Run* run), Level* level) {
    float eps = EPSILON;
    for (unsigned long k = 0; k < n; k++) {
        printf("Learning iteration %ld\n", k);
        
        init_level_player(level, q);
        
        Run run;
        run.first = NULL;
        run.last = NULL;
        learning_play(level, &run, action, eps);
        learning(level->player->q, &run);
        
        free_run(&run);
        eps *= EPSILON;
    }
}

void learning_play(Level* level, Run* run, Action action(Matrix* q, Perception p, float eps), float eps) {
    bool quit = false;
    run->first = NULL;
    run->last = NULL;
    
    while (!quit) {
        Perception p = get_entity_perception(level, level->player);
        Action a = action(level->player->q, p, eps);
        
        quit = update_game(level);
        make_action(level, level->player, a);
        if(!run->last) {
            run->last = malloc(sizeof(struct RunListCell));
            run->first = run->last;
            run->first->next = NULL;
            run->first->previous = NULL;
        }
        else {
            run->last->next = malloc(sizeof(struct RunListCell));
            run->last->next->previous = run->last;
            run->last = run->last->next;
            run->last->next = NULL;
        }
        run->last->reward = 0;
        run->last->action = a;
        run->last->state = p;
    }
    run->last->next = malloc(sizeof(struct RunListCell));
    run->last->next->previous = run->last;
    run->last = run->last->next;
    
    if (level->player->location.x >= level->length)
        run->last->reward = level->length / (0.15 * level->score);
    else
        run->last->reward = -1;
}

Action e_greedy(Matrix* q, Perception perception, float eps) {
    float r = rand() / (float) RAND_MAX;
    
    if (r > eps) {
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
    }
    else {
        r = rand() / (float) RAND_MAX;
        unsigned int j;
        for (j = 0; j < q->columns; j++)
            if (r <= (j+1) / (float) q->columns)
                return j;
        
        return j-1;
    }
}

void q_learning(Matrix* matrix, Run* run) {
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

void double_q_learning(Matrix* matrix1, Matrix* matrix2, Run* run) {
    int alpha = rand()%2;
    Matrix* Q_a; //Q_apprentissage
    Matrix* Q_e; //Q_estimation

    struct RunListCell* next;
    float quality;
    float max_quality;
    Action max_action;

    if (alpha) {
        Q_a = matrix1;
        Q_e = matrix2;
    }
    else {
        Q_a = matrix2;
        Q_e = matrix1;
    }

    *get_matrix_element(Q_a, run->last->previous->state, run->last->previous->action) +=
        EPSILON_LEARNING *
        (
            run->last->reward -
            *get_matrix_element(Q_a, run->last->previous->state, run->last->previous->action)
        );

    for(struct RunListCell* it = run->last->previous->previous; it; it = it->previous) {
        alpha = rand()%2;
        if (alpha) {
        Q_a = matrix1;
        Q_e = matrix2;
        }
        else {
        Q_a = matrix2;
        Q_e = matrix1;
        }

        next = it->next;
        max_action = 0;
        max_quality = *get_matrix_element(Q_e, next->state, 0);
        
        for (int j = 1; j<Q_e->columns; j++) {
            quality = *get_matrix_element(Q_e, next->state, j);
            max_action = (quality>max_quality) ?
                j :
                max_action;
            max_quality = (quality>max_quality) ?
                quality :
                max_quality;
        }

        *get_matrix_element(Q_a, it->state, it->action) +=
            EPSILON_LEARNING *
            (
                next->reward +
                GAMMA * max_quality -
                *get_matrix_element(Q_a, it->state, it->action)
            );
    }
}

void free_run(Run* run) {
    for(struct RunListCell* it = run->first; it != NULL;) {
        struct RunListCell* tmp = it;
        it = it->next;
        free(tmp);
    }
}