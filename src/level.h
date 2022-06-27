#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "entity.h"


typedef struct {
    struct EntityListCell {
        Entity* entity;
        struct EntityListCell* cell;
    } entities;
    Player* player;
    float width;
    float lenght;
} Level;


/**
 * @brief Creates a new level from the seed.
 * 
 * @param width the width of the level.
 * @param lenght the lenght of the level.
 * @return the created level.
 */
Level* new_level(float width, float lenght);

/**
 * @brief Frees a level.
 * 
 * @param level the level to free.
 */
void free_level(Level* level);

/**
 * @brief Adds an entity into a level.
 * 
 * @param level the level.
 * @param entity the entity to add.
 * @return if the entity has been added correctly.
 */
bool add_level_entity(Level* level, Entity* entity);

/**
 * @brief Removes deleted entities from a level.
 * 
 * @param level the level.
 */
void clean_level_entities(Level* level);


#endif
