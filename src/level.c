#include "level.h"
#include <stdbool.h>

Level* new_level(float width, float lenght)
{
    Level* newLevel =  (Level*)malloc(sizeof(Level));
    if(newLevel)
    {
        newLevel->width = width;
        newLevel->lenght = lenght;
    }
    return newLevel;
}

void free_level(Level* level)
{
    struct EntityListCell * next;
    for(struct EntityListCell * iterator = &level->entities; iterator; iterator = next)
    {
        free_entity(iterator->entity);
        next = iterator->cell;
        free(iterator);
    }
    free(level);
}

bool add_level_entity(Level* level, Entity* entity)
{
    struct EntityListCell * newEntity = (struct EntityListCell *)malloc(sizeof(struct EntityListCell));
    if(newEntity)
    {
        newEntity->entity = entity;
        newEntity->cell = &level->entities;
        level->entities = *newEntity;
        return true;
    }
    return false;
}