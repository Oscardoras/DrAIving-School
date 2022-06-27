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
 * @brief Makes the player go left.
 * 
 * @param level the level.
 * @return if the operation has succeed.
 */
bool player_left(Level* level);

/**
 * @brief Makes the player go right.
 * 
 * @param level the level.
 * @return if the operation has succeed.
 */
bool player_right(Level* level);

/**
 * @brief Makes the player go faster.
 * 
 * @param level the level.
 * @return if the operation has succeed.
 */
bool player_faster(Level* level);

/**
 * @brief Makes the player go slower.
 * 
 * @param level the level.
 * @return if the operation has succeed.
 */
bool player_slower(Level* level);


#endif
