#include <stdlib.h>

#include "level.h"


Level* new_level(float width, float length, Matrix* matrix) {
    Level* level = malloc(sizeof(Level));

    if (level != NULL) {
        level->width = width;
        level->length = length;
        level->entities = NULL;
        level->matrix = matrix;
        level->player = NULL;
        level->score = 0;
    }

    return level;
}

void free_level(Level* level) {
    if (level != NULL) {
        remove_level_entities(level);
        free_entity(level->player);
        free(level);
    }
}

bool add_level_entity(Level* level, Entity* entity) {
    struct EntityListCell* cell = malloc(sizeof(struct EntityListCell));

    if (cell != NULL) {
        cell->entity = entity;
        cell->next = level->entities;
        level->entities = cell;

        return true;
    } else return false;
}

void remove_level_entities(Level* level) {
    for (struct EntityListCell* it = level->entities; it != NULL;) {
        struct EntityListCell* tmp = it;
        it = it->next;
        free_entity(tmp->entity);
        free(tmp);
    }
    level->entities = NULL;
}

bool init_level_player(Level* level, Matrix* q) {
    if (level->player != NULL)
        free_entity(level->player);
    
    Location location;
    location.velocity = DEFAULT_PLAYER_VELOCITY;
    location.x = 0.;
    location.y = (4.5 / 6) * level->width;
    level->player = new_entity(PLAYER_CAR, location, q);
    level->score = 0;
    
    return level->player != NULL;
}