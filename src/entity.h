#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <stdbool.h>

#include "matrix.h"

#define ENTITY_TYPES 2

typedef struct {
    float x;
    float y;
    bool direction; //true si vers la droite, false sinon
    float velocity;
} Location;

typedef enum {
    PLAYER_CAR,
    CAR
} EntityType;

typedef struct {
    EntityType type;
    Location location;
    Matrix* markov;
    unsigned int state;
} Entity;

/**
 * @brief Creates a new entity.
 * 
 * @param type the type of the entity.
 * @param location the location in the level.
 * @param markov the Markov matrix of the entity.
 * @param state the Markov state of the entity.
 * @return the new entity.
 */
Entity* new_entity(EntityType type, Location location, Matrix* markov, unsigned int state);

/**
 * @brief Frees an entity.
 * 
 * @param entity the entity.
 */
void free_entity(Entity* entity);


#endif
