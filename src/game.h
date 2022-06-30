#ifndef __GAME__H__
#define __GAME__H__

#include "level.h"

#define CAR_PER_LINE 1
#define LINES_PER_DIRECTION 3


typedef enum {
    PERCEPTION_LEFT = 0b1,
    PERCEPTION_RIGHT = 0b10,
    PERCEPTION_TOP_LEFT = 0b100,
    PERCEPTION_TOP = 0b1000,
    PERCEPTION_TOP_RIGHT = 0b10000,
    PERCEPTION_FAR_LEFT = 0b100000,
    PERCEPTION_FAR_RIGHT = 0b1000000
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

/**
 * @brief Fills the perceptions boxes of an entity.
 * 
 * @param entity the entity.
 * @param boxes the array to fill.
 */
void get_entity_perception_hitbox(Entity* entity, HitBox boxes[6]);

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

void car_cluster(Level* level, int line);

#endif
