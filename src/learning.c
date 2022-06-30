#include <math.h>
#include <stdlib.h>

#include "learning.h"


void learn(unsigned long n, Matrix* q, Action action(Matrix*, Perception, float), void learning(Matrix*, Run*), Level* level) {
    float eps = EPSILON;
    for (unsigned long k = 0; k < n; k++) {
        remove_level_entities(level);
        init_level_player(level, q);
        
        Run run;
        run.first = NULL;
        run.last = NULL;
        simulate_game(level, &run, action, eps);
        printf("Learning iteration %ld, reward %f, epsilon %f\n", k, run.last->reward, eps);
        learning(level->player->q, &run);

        free_run(&run);
    
        //eps = EPSILON  * 2 / (float) (k + 1);
        eps *= EPSILON;
    }
}

void simulate_game(Level* level, Run* run, Action action(Matrix*, Perception, float), float eps) {
    bool quit = false;
    run->first = NULL;
    run->last = NULL;
    
    while (!quit) {
        Perception p = get_entity_perception(level, level->player);
        Action a = action(level->player->q, p, eps);
        //printf("Perception : %d, Action : %d\n", p, a);
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
        run->last->reward = REWARD_MULTIPLIER * level->length / (DEFAULT_PLAYER_VELOCITY * level->score);
    else
        run->last->reward = -1.;
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
        float p_max = *get_matrix_element(q, perception, 0);
        Action action = 0;
        
        for (unsigned int j = 1; j < q->columns; j++) {
            float p = *get_matrix_element(q, perception, j);
            if (p > p_max) {
                p_max = p;
                action = j;
            }
        }
        
        return action;
    } else
        return rand() % q->columns;
}

void q_learning(Matrix* q, Run* run) {
    *get_matrix_element(q, run->last->previous->state, run->last->previous->action) += XI * (run->last->reward - *get_matrix_element(q, run->last->previous->state, run->last->previous->action)); 
    for (struct RunListCell* it = run->last->previous->previous; it != NULL; it = it->previous) {
        float M = *get_matrix_element(q, it->next->state, 0);
        for (unsigned int a = 1; a < ACTIONS; a++) {
            float v = *get_matrix_element(q, it->next->state, a);
            if (v > M)
                M = v;
        }

        *get_matrix_element(q, it->state, it->action) += XI * (it->next->reward + GAMMA*M - *get_matrix_element(q, it->state, it->action));
    }
}

Action preference_learning_base(Matrix* q, Perception p, float temp) {
    float L[q->columns];
    float Z = 0;
    float sum = 0;
    float alpha = rand() / (float) RAND_MAX;
    Action action = q->columns;

    for (unsigned int i = 0 ; i < q->columns; i++) {
        L[i] = exp(*get_matrix_element(q, p, i) / temp);
        Z += L[i];
    }

    for (unsigned int i = 0; action > i; i++) {
        sum += L[i]/Z;
        if (alpha <= sum)
            action = i;
    }

    return action;
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
        XI *
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
        
        for (unsigned int j = 1; j < Q_e->columns; j++) {
            quality = *get_matrix_element(Q_e, next->state, j);
            max_action = (quality>max_quality) ?
                j :
                max_action;
            max_quality = (quality>max_quality) ?
                quality :
                max_quality;
        }

        *get_matrix_element(Q_a, it->state, it->action) +=
            XI *
            (
                next->reward +
                GAMMA * max_quality -
                *get_matrix_element(Q_a, it->state, it->action)
            );
    }
}

void sarsa(Matrix* matrix, Run* run) {
    *get_matrix_element(matrix, run->last->previous->state, run->last->previous->action) +=
        XI *
        (
            run->last->reward -
            *get_matrix_element(matrix, run->last->previous->state, run->last->previous->action)
        );

    struct RunListCell* next;
    for (struct RunListCell* it = run->last->previous->previous; it; it = it->previous) {
        next = it->next;
        *get_matrix_element(matrix, it->state, it->action) +=
            XI * 
            (
                it->next->reward +
                GAMMA * *get_matrix_element(matrix, next->state, next->action) -
                *get_matrix_element(matrix, it->state, it->action)
            );
    }
}
