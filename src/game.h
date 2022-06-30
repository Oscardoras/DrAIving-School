#ifndef __GAME__H__
#define __GAME__H__

#include "level.h"

#define CAR_PER_LINE 1
#define LINES_PER_DIRECTION 3


typedef enum {
    PLAYER_1 = 0b1,
    PLAYER_2 = 0b10,
    PLAYER_3 = 0b100,
    PLAYER_4 = 0b1000,
    PLAYER_5 = 0b10000,
    PLAYER_6 = 0b100000,
    CAR_1    = 0b1000000,
    CAR_2    = 0b10000000,
    CAR_3    = 0b100000000,
    CAR_4    = 0b1000000000,
    CAR_5    = 0b10000000000,
    CAR_6    = 0b100000000000,
} Perception;

typedef enum {
    ACTION_NONE = 0,
    ACTION_LEFT,
    ACTION_RIGHT,
    ACTION_FASTER,
    ACTION_SLOWER
} Action;
#define ACTIONS 5


/**
 * @brief Updates the game status.
 * 
 * @param level the level.
 * @return if the game has finished.
 */
bool update_game(Level* level);

HitBox* get_entity_perception_hitbox(Entity* entity);

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
