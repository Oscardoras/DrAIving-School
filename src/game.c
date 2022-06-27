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