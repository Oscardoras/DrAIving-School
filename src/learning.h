#ifndef __LEARNING__H__
#define __LEARNING__H__

#include "game.h"


/**
 * @brief Represents a run.
 * 
 */
typedef struct {
    struct {
        State state;
        Action action;
        float reward;
    } *tab;
    unsigned int size;
} Run;


#endif
