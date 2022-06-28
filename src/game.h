#ifndef __GAME__H__
#define __GAME__H__

#include "level.h"


typedef enum {
    STATE_LEFT = 0b1,
    STATE_RIGHT = 0b10,
    STATE_TOP_LEFT = 0b100,
    STATE_TOP = 0b1000,
    STATE_TOP_RIGHT = 0b10000,
    STATE_DIRECTION_LEFT = 0b100000,
    STATE_DIRECTION_TOP = 0b1000000,
    STATE_DIRECTION_RIGHT = 0b10000000
} State;

typedef enum {
    ACTION_LEFT,
    ACTION_RIGHT,
    ACTION_FASTER,
    ACTION_SLOWER
} Action;


/**
 * @brief Updates the game status.
 * 
 * @param level the level.
 */
void update_game(Level* level);


/**
 * @brief Chooses the action that an enity will execute.
 * 
 * @param level the level.
 * @param entity the entity.
 * @return the choosen action.
 */
Action choose_action(Level* level, Entity* entity);

/**
 * @brief Makes an entity execute an action.
 * 
 * @param level the level.
 * @param entity the entity.
 * @param action the action to execute.
 * @return if the operation has succeed.
 */
bool make_action(Level* level, Entity* entity, Action action);

/**
 * @brief Checks if a location is in a rectangle.
 * 
 * @param location the location.
 * @param min_x
 * @param max_x
 * @param min_y
 * @param max_y
 * @return if the location is in the rectangle.  
 */
bool is_location_in(Location location, float min_x, float max_x, float min_y, float max_y);


#endif
