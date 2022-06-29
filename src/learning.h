#ifndef __LEARNING__H__
#define __LEARNING__H__

#include "game.h"


struct RunListCell {
    Perception state;
    Action action;
    float reward;
    
    struct RunListCell* previous;
    struct RunListCell* next;
};

/**
 * @brief Represents a run.
 * 
 */
typedef struct {
    struct RunListCell* first;
    struct RunListCell* last;
} Run;


/**
 * @brief Does a learning.
 * 
 * @param n the number of iterations of the learn.
 * @param q the Q(s,a) matrix.
 * @param action a pointer to the action chooser function.
 * @param learning a pointer to the learning function.
 * @param level a level.
 */
void learn(unsigned long iterations, Matrix* q, Action action(Matrix* q, Perception p, float eps), void learning(Matrix* q, Run* run), Level* level);

/**
 * @brief Does a play with the learning system.
 * 
 * @param level the level.
 * @param run a pointer to a run.
 * @param action a pointer to the action chooser function.
 * @param eps epsilon.
 */
void learning_play(Level* level, Run* run, Action action(Matrix* q, Perception p, float eps), float eps);

/**
 * @brief Frees a run.
 * 
 * @param run the run to free.
 */
void free_run(Run* run);

int lenght_run(Run* run);

Action e_greedy(Matrix* q, Perception perception, float eps);

void q_learning(Matrix* q, Run* run);

void double_q_learning(Matrix* q1, Matrix* q2, Run* run);


#endif
