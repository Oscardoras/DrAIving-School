#include <stdlib.h>

#include "matrix.h"
#include "entity.h"


Entity* new_entity(EntityType type, Location location, Matrix* markov, unsigned int state) {
    Entity* entity = malloc(sizeof(Entity));
    
    if (entity != NULL) {
        entity->type = type;
        entity->location = location;
        entity->markov = markov;
        entity->state = state;
    }
    
    return entity;
}

void free_entity(Entity* entity) {
    if (entity != NULL)
        free(entity);
}  

Player* new_player(Location location, Matrix* matrix) {
    Player* player = malloc(sizeof(Player));
    
    if (player != NULL) {
        player->location = location;
        player->matrix = matrix;
        player->time = 0.;
    }
    
    return player;
}

void free_player(Player* player) {
    if (player != NULL)
        free(player);
}
