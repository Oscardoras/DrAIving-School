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
void learning_play(Level* level, Run* run, Action action(Matrix* Q, Perception perception));


Action e_greedy(Matrix* Q, Perception perception);


void learning_update(Matrix* Q, Run* run);

void freeRun(Run* run);
#endif
