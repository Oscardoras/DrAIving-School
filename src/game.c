#include "game.h"

#define PERCEPTION_DISTANCE 1.


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

Action choose_action(Level* level, Entity* entity) {
    
}

bool make_action(Level* level, Entity* entity, Action action) {
    switch (action) {
        case ACTION_LEFT:
            entity->location.y -= entity->location.velocity;
        break;
        case ACTION_RIGHT:
            entity->location.y += entity->location.velocity;
        break;
        case ACTION_FASTER:
            entity->location.velocity += 0.1;
        break;
        case ACTION_SLOWER:
            entity->location.velocity -= 0.1;
        break;
    }
    
    return true;
}
