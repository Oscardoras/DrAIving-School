#ifndef __GAME__H__
#define __GAME__H__

#include "level.h"

#define CAR_WIDTH 1.
#define CAR_LENGTH 2.


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

typedef struct {    
    float min_x;
    float max_x;
    float min_y;
    float max_y;
} HitBox;


/**
 * @brief Updates the game status.
 * 
 * @param level the level.
 * @return if there are accidents.
 */
bool update_game(Level* level);


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
 * @brief Checks if two hit boxes are hitting.
 * 
 * @param b1 the first hit box.
 * @param b2 the second hit box.
 * @return if the two hit boxes are hitting.
 */
bool are_entity_box_hitting(HitBox b1, HitBox b2);

/**
 * @brief Gets the hit box of an entity.
 * 
 * @param entity the entity.
 * @return the hit box.
 */
HitBox get_entity_hitbox(Entity* entity);


#endif
