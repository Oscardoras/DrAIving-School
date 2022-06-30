#include <stdlib.h>

#include "game.h"
#include "entity.h"
#include "level.h"


Level* new_level(float width, float length, Matrix* matrix) {
    Level* level = malloc(sizeof(Level));

    if (level != NULL) {
        level->width = width;
        level->length = length;
        level->entities = NULL;
        level->percepts = malloc(2 * LINES_PER_DIRECTION * WIDTH_PERCEPTS * sizeof(Entity));
        level->matrix = matrix;
        level->player = NULL;
        level->score = 0;
    }

    return level;
}

void init_percepts(Level* level) {
    float px = level->player->location.x;
    for(int i=0; i<WIDTH_PERCEPTS; i++) {
        for(int j=0; j<2*LINES_PER_DIRECTION; j++) {
            level->percepts[j*WIDTH_PERCEPTS + i].type = PERCEPT;
            level->percepts[j*WIDTH_PERCEPTS + i].location.x = px + (float) i*CAR_LENGTH;
            level->percepts[j*WIDTH_PERCEPTS + i].location.y = ((float) j+0.4)*level->width/(2.*LINES_PER_DIRECTION);
            level->percepts[j*WIDTH_PERCEPTS + i].location.velocity = 0;
            level->percepts[j*WIDTH_PERCEPTS + i].q = NULL;
        }
    }
}

void free_level(Level* level) {
    if (level != NULL) {
        remove_level_entities(level);
        free(level->percepts);
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
    }
    else return false;
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
    location.y = (5. / 6) * level->width;
    level->player = new_entity(PLAYER_CAR, location, q);
    level->score = 0;
    
    return level->player != NULL;
}