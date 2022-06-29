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
 * @brief Does a play with the learning system.
 * 
 * @param level the level.
 * @param run a pointer to a run.
 * @param action a pointer to the action chooser function.
 */
void learning_play(Level* level, Run* run, Action action(Matrix* q, Perception perception));

/**
 * @brief Frees a run.
 * 
 * @param run the run to free.
 */
void free_run(Run* run);


Action e_greedy(Matrix* q, Perception perception);


void learning_update(Matrix* q, Run* run);


#endif
