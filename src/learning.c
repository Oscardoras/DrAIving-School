#include <stdlib.h>

#include "learning.h"

#define EPSILON 0.99
#define XI 0.0001
#define GAMMA 0.0001


void learn(unsigned long n, Matrix* q, Action action(Matrix*, Perception, float), void learning(Matrix*, Run*, float, float), Level* level) {
    float eps = EPSILON;
    float xi = XI;
    float gamma = GAMMA;
    for (unsigned long k = 0; k < n; k++) {
        remove_level_entities(level);
        init_level_player(level, q);
        
        Run run;
        run.first = NULL;
        run.last = NULL;
        simulate_game(level, &run, action, eps);
        learning(level->player->q, &run, xi, gamma);
        
        printf("Learning iteration %ld, reward %f, epsilon %f\n", k, run.last->reward, eps);

        free_run(&run);

        eps *= EPSILON;
        xi *= XI;
        gamma *= GAMMA;
    }
}

void simulate_game(Level* level, Run* run, Action action(Matrix*, Perception, float), float eps) {
    bool quit = false;
    run->first = NULL;
    run->last = NULL;
    
    while (!quit) {
        Perception p = get_entity_perception(level, level->player);
        Action a = action(level->player->q, p, eps);
        make_action(level, level->player, a);
        quit = update_game(level);

        if (run->last == NULL) {
            run->last = malloc(sizeof(struct RunListCell));
            run->last->next = NULL;
            run->last->previous = NULL;
            run->first = run->last;
        } else {
            run->last->next = malloc(sizeof(struct RunListCell));
            run->last->next->previous = run->last;
            run->last->next->next = NULL;
            run->last = run->last->next;
        }
        run->last->reward = 0.;
        run->last->action = a;
        run->last->state = p;
    }

    run->last->next = malloc(sizeof(struct RunListCell));
    run->last->next->previous = run->last;
    run->last->next->next = NULL;
    run->last = run->last->next;
    if (level->player->location.x >= level->length)
        run->last->reward = level->length / (DEFAULT_PLAYER_VELOCITY * level->score);
    else
        run->last->reward -= 1.;
}

void free_run(Run* run) {
    for(struct RunListCell* it = run->first; it != NULL;) {
        struct RunListCell* tmp = it;
        it = it->next;
        free(tmp);
    }
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

void q_learning(Matrix* q, Run* run, float xi, float gamma) {
    *get_matrix_element(q, run->last->previous->state, run->last->previous->action) += XI * (run->last->reward - *get_matrix_element(q, run->last->previous->state, run->last->previous->action)); 
    for (struct RunListCell* it = run->last->previous->previous; it != NULL; it = it->previous) {
        float M = *get_matrix_element(q, it->next->state, 0);
        for (unsigned int j = 1; j < ACTIONS; ++j) {
            float v = *get_matrix_element(q, it->next->state, j);
            if (v > M)
                M = v;
        }

        *get_matrix_element(q, it->state, it->action) += XI * (it->next->reward + GAMMA*M - *get_matrix_element(q, it->state, it->action));
    }
}