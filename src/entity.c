#include "entity.h"

Entity* new_entity(Location location, Matrix* markov, unsigned int state, unsigned int type) {
    Entity* entity = malloc(sizeof(Entity));
    
    if (entity) {
        entity->location = location;
        entity->markov = markov;
        entity->state = state;
        entity->type = type;
    }
    else {
        printf("Error malloc entity.\n");
    }
    
    return entity;
}

void free_entity(Entity* entity) {
    if(entity) free(entity);
}

Player* new_player(Location location, Matrix* matrix) {
    Player* player = malloc(sizeof(Player));
    
    if (player) {
        player->location = location;
        player->matrix = matrix;
        player->time = 0.0;
    }
    else printf("Error malloc player.\n");
    
    return player;
}

void free_player(Player* player) {
    if (player) free(player);
}