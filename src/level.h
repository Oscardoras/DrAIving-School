#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <stdbool.h>

#include "entity.h"

#define DEFAULT_PLAYER_VELOCITY 0.2
#define WIDTH_PERCEPTS 4


typedef struct {
    float width;
    float length;
    struct EntityListCell {
        Entity* entity;
        struct EntityListCell* next;
    } *entities;
    Entity* percepts;
    Matrix* matrix;
    Entity* player;
    int score;
} Level;


/**
 * @brief Creates a new level.
 * 
 * @param width the width of the level.
 * @param length the length of the level.
 * @param matrix the matrix to create entities.
 * @return the created level.
 */
Level* new_level(float width, float length, Matrix* matrix);

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
 * @brief Removes all the entities in a level.
 * 
 * @param level the level.
 */
void remove_level_entities(Level* level);

/**
 * @brief Sets the player in the level.
 * 
 * @param level the level.
 * @param q the Q(s,a) matrix of the player. Can be NULL.
 * @return if the player has been added correctly set.
 */
bool init_level_player(Level* level, Matrix* q);

void init_percepts(Level* level);
#endif
