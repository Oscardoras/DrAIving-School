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

/**
 * @brief Determines the length of a run.
 * 
 * @param run the said run
 * @return int 
 */
int lenght_run(Run* run);

/**
 * @brief Choose which action to do with the e-greedy algorithm
 * 
 * @param q the matrix of state/action/reward.
 * @param perception the perception of the AI.
 * @param eps the ratio exploration/exploitation.
 * @return the action the AI will do.
 */
Action e_greedy(Matrix* q, Perception perception, float eps);

/**
 * @brief Choose which action to do with the preference learning-base algorithm
 * 
 * @param q the matrix of state/action/reward.
 * @param perception the perception of the AI.
 * @param temperature how much we value the quality of an action.
 * @return the action the AI will do.
 */
Action preference_learning_base(Matrix* q, Perception perception, float temperature);

/**
 * @brief Updates the matrix of the AI with the Q-Learning algorithm
 * 
 * @param q the matrix of the AI.
 * @param run the run the AI just did.
 */
void q_learning(Matrix* q, Run* run);

/**
 * @brief Updates the matrix of the AI with the Double Q-Learning algorithm
 * 
 * @param q1 one of the matrix of the AI.
 * @param q2 one of the matrix of the AI.
 * @param run the run the AI just did.
 */
void double_q_learning(Matrix* q1, Matrix* q2, Run* run);

/**
 * @brief Updates the matrix of the AI with the SARSA algorithm
 * 
 * @param q the matrix of the AI.
 * @param run the run the AI just did.
 */
void sarsa(Matrix* q, Run* run);


#endif
