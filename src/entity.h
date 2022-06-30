#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <stdbool.h>

#include "matrix.h"

#define CAR_WIDTH 1.2
#define CAR_LENGTH 2.


typedef struct {
    float x;
    float y;
    float velocity;
} Location;

typedef enum {
    PLAYER_CAR,
    CAR,
    PERCEPT
} EntityType;
#define ENTITY_TYPES 2

typedef struct {
    EntityType type;
    Location location;
    Matrix* q;
} Entity;

typedef struct {    
    float min_x;
    float max_x;
    float min_y;
    float max_y;
} HitBox;


/**
 * @brief Creates a new entity.
 * 
 * @param type the type of the entity.
 * @param location the location in the level.
 * @param q the Q(s,a) matrix of the entity.
 * @return the new entity.
 */
Entity* new_entity(EntityType type, Location location, Matrix* q);

/**
 * @brief Frees an entity.
 * 
 * @param entity the entity.
 */
void free_entity(Entity* entity);


/**
 * @brief Gets the hit box of an entity.
 * 
 * @param entity the entity.
 * @return the hit box.
 */
HitBox get_entity_hitbox(Entity* entity);

/**
 * @brief Checks if two hit boxes are hitting.
 * 
 * @param b1 the first hit box.
 * @param b2 the second hit box.
 * @return if the two hit boxes are hitting.
 */
bool are_entity_box_hitting(HitBox b1, HitBox b2);


#endif
