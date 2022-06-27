#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <stdbool.h>

#include "matrix.h"


typedef struct {
    float x;
    float y;
    bool direction;
} Location;


typedef enum {
    Car1,
    Car2
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
 * @param location the location in the level.
 * @param markov the Markov matrix of the entity.
 * @param state the Markov state of the entity.
 * @return the new entity.
 */
Entity* new_entity(Location location, Matrix* markov, unsigned int state, unsigned int type);

/**
 * @brief Frees an entity.
 * 
 * @param entity the entity.
 */
void free_entity(Entity* entity);


typedef struct {
    Location location;
    Matrix* matrix;
    float time;
} Player;

/**
 * @brief Creates a new player.
 * 
 * @param location the location in the level.
 * @param matrix the matrix of the player.
 * @return the new player.
 */
Player* new_player(Location location, Matrix* matrix);

/**
 * @brief Frees a player.
 * 
 * @param player the player.
 */
void free_player(Player* player);


#endif
