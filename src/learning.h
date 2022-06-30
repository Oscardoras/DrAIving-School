#ifndef __LEARNING__H__
#define __LEARNING__H__

#include "game.h"

//#define LEARN_ITERATION 10000
#define EPSILON 0.999
#define XI 0.01
#define GAMMA 0.95
#define REWARD_MULTIPLIER 100


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
void learn(unsigned long n, Matrix* q, Action action(Matrix*, Perception, float), void learning(Matrix*, Run*), Level* level);

/**
 * @brief Does a play with the learning system.
 * 
 * @param level the level.
 * @param run a pointer to a run.
 * @param action a pointer to the action chooser function.
 * @param eps epsilon.
 */
void simulate_game(Level* level, Run* run, Action action(Matrix*, Perception, float), float eps);

/**
 * @brief Frees a run.
 * 
 * @param run the run to free.
 */
void free_run(Run* run);


Action e_greedy(Matrix* q, Perception perception, float eps);

void q_learning(Matrix* q, Run* run);

Action preference_learning_base(Matrix* q, Perception perception, float temperature);

void double_q_learning(Matrix* q1, Matrix* q2, Run* run);

void sarsa(Matrix* q, Run* run);


#endif
