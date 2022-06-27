#include "game.h"


void update_game(Level* level) {
    for (struct EntityListCell* it = level->entities; it != NULL; it = it->next) {
        switch (it->entity->state) {
            /*
            case ENTITYSTATE_LEFT:

            break;
            case ENTITYSTATE_RIGHT:

            break;
            case ENTITYSTATE_FASTER:

            break;
            case ENTITYSTATE_SLOWER:

            break;
            */
        } 
    }
}

bool player_action(Level* level, Action action) {
    switch (action) {
        case ACTION_LEFT:
            level->player->location.y -= level->player->location.velocity;
        break;
        case ACTION_RIGHT:
            level->player->location.y += level->player->location.velocity;
        break;
        case ACTION_FASTER:
            level->player->location.velocity += 0.1;
        break;
        case ACTION_SLOWER:
            level->player->location.velocity -= 0.1;
        break;
    }
    
    return true;
}
