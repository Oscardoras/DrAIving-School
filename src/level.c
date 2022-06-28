#include <stdlib.h>

#include "level.h"


Level* new_level(float width, float length, int lines_per_dir) {
    Level* level = malloc(sizeof(Level));

    if (level != NULL) {
        level->width = width;
        level->length = length;
        level->entities = NULL;
        level->player = NULL;
        level->score = 0;
        level->lines_per_dir = lines_per_dir;
    }

    return level;
}

void free_level(Level* level) {
    for (struct EntityListCell* it = level->entities; it != NULL;) {
        struct EntityListCell* tmp = it;
        it = it->next;
        free_entity(tmp->entity);
        free(tmp);
    }

    free(level);
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

Location location_from_line(Level* level, float x, float velocity, int line, int lines_max) {
    Location player_location;
    
    player_location.x = x;
    player_location.velocity = velocity;
    
    player_location.direction = (line>lines_max/2);
    
    float width_per_block = level->width / lines_max;    
    player_location.y = (line-1.+0.2) * width_per_block;
    
    return player_location;
}