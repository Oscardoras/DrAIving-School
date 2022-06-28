#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <stdbool.h>

#include "entity.h"


typedef struct {
    float width;
    float length;
    struct EntityListCell {
        Entity* entity;
        struct EntityListCell* next;
    } *entities;
    Entity* player;
    int score;
    int lines_per_dir;
} Level;


/**
 * @brief Creates a new level.
 * 
 * @param width the width of the level.
 * @param length the length of the level.
 * @param lines_per_dir
 * @return the created level.
 */
Level* new_level(float width, float length, int lines_per_dir);

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
 * @brief Gives an entity location based on its x, its velocity, and the line it will be on.
 * 
 * @param level the level.
 * @param x the x of the entity.
 * @param velocity the velocity of the entity.
 * @param line the line the vehicle will be on.
 * @param lines_max the number of lines on the road (both directions).
 * @return the location filled.
 */
Location location_from_line(Level* level, float x, float velocity, int line, int lines_max);


#endif
