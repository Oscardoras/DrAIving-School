#ifndef __GAME__H__
#define __GAME__H__

#include "level.h"


/**
 * @brief Updates the game status.
 * 
 * @param level the level.
 */
void update_game(Level* level);

/**
 * @brief Makes the player execute an action.
 * 
 * @param level the level.
 * @param action the action execute.
 * @return if the operation has succeed.
 */
bool player_action(Level* level, Action action);


#endif
