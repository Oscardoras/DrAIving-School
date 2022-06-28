#ifndef __GAME__H__
#define __GAME__H__

#include "level.h"


typedef enum {
    PERCEPTION_LEFT = 0b1,
    PERCEPTION_RIGHT = 0b10,
    PERCEPTION_TOP_LEFT = 0b100,
    PERCEPTION_TOP = 0b1000,
    PERCEPTION_TOP_RIGHT = 0b10000,
    PERCEPTION_DIRECTION_LEFT = 0b100000,
    PERCEPTION_DIRECTION_TOP = 0b1000000,
    PERCEPTION_DIRECTION_RIGHT = 0b10000000
} Perception;

typedef enum {
    ACTION_NONE,
    ACTION_LEFT,
    ACTION_RIGHT,
    ACTION_FASTER,
    ACTION_SLOWER
} Action;


/**
 * @brief Updates the game status.
 * 
 * @param level the level.
 * @return if the game has finished.
 */
bool update_game(Level* level);

/**
 * @brief Gets the perception of an entity.
 * 
 * @param level the level.
 * @param entity the entity.
 * @return the perception.
 */
Perception get_entity_perception(Level* level, Entity* entity);

/**
 * @brief Makes an entity execute an action.
 * 
 * @param level the level.
 * @param entity the entity.
 * @param action the action to execute.
 * @return if the operation has succeed.
 */
bool make_action(Level* level, Entity* entity, Action action);


#endif
