#include "game.h"


void update_game(Level* level) {
    for (struct EntityListCell* it = level->entities; it != NULL; it = it->next) {
        switch (it->entity->state) {
            case ENTITYSTATE_LEFT:

            break;
            case ENTITYSTATE_RIGHT:

            break;
            case ENTITYSTATE_FASTER:

            break;
            case ENTITYSTATE_SLOWER:

            break;
        } 
    }
}

bool player_action(Level* level, Action action) {
    switch (action) {
        case ENTITYSTATE_LEFT:
            level->player->location.y -= level->player->location.velocity;
        break;
        case ENTITYSTATE_RIGHT:
            level->player->location.y += level->player->location.velocity;
        break;
        case ENTITYSTATE_FASTER:
            level->player->location.velocity.x += 0.1;
        break;
        case ENTITYSTATE_SLOWER:
            level->player->location.velocity.x -= 0.1;
        break;
    }
    
    return true;
}