#include <stdlib.h>

#include "game.h"


bool update_game(Level* level) {
    HitBox player_box = get_entity_hitbox(level->player);
    
    int tab[LINES_PER_DIRECTION*2];
    for (int n = 0; n < LINES_PER_DIRECTION*2; n++) {
        tab[n] = 0;
    }
    
    struct EntityListCell** ptr = &level->entities;
    while (*ptr != NULL) {
        if ((*ptr)->entity->location.x < level->player->location.x - 3*CAR_LENGTH) {
            struct EntityListCell* tmp = *ptr;
            *ptr = (*ptr)->next;
            free_entity(tmp->entity);
            free(tmp);
        } else {
            struct EntityListCell* it = *ptr;
            make_action(level, it->entity, compute_state(it->entity->q, get_entity_perception(level, it->entity)));
            it->entity->location.x += it->entity->location.velocity;
            
            if (are_entity_box_hitting(get_entity_hitbox(it->entity), player_box))
                return true;
            
            int line = (int) ((it->entity->location.y / level->width) * LINES_PER_DIRECTION*2);
            tab[line]++;
            
            ptr = &(*ptr)->next;
        }
    }
    
    for (int n = 0; n < LINES_PER_DIRECTION*2; n++) {
        if (tab[n] < CAR_PER_LINE) {
            Location location;
            location.x = level->player->location.x + CAR_LENGTH*(10 + 10 * (rand() / (float) RAND_MAX));
            location.y = ((n + 0.3) / (LINES_PER_DIRECTION*2)) * level->width;
            location.velocity = n < LINES_PER_DIRECTION ? -0.1 : 0.1;
        
            add_level_entity(level, new_entity(CAR, location, level->matrix));
        }
    }
    
    level->player->location.x += level->player->location.velocity;
    if (player_box.min_y < 0 || player_box.max_y > level->width-0.8)
        return true;
    
    level->score++;
    return level->player->location.x >= level->length;
}

Perception get_entity_perception(Level* level, Entity* entity) {
    Perception p = 0b0;
    
    for (struct EntityListCell* it = level->entities; it != NULL; it = it->next) {
        HitBox box = get_entity_hitbox(it->entity);
        
        HitBox b;
        b.min_x = entity->location.x-CAR_LENGTH/2-0.1;
        b.max_x = entity->location.x+CAR_LENGTH/2+0.1;
        b.min_y = entity->location.y-CAR_WIDTH/2;
        b.max_y = entity->location.y-CAR_WIDTH/2-0.1;
        p = p | (PERCEPTION_LEFT * are_entity_box_hitting(box, b));
        
        b.min_x = entity->location.x-CAR_LENGTH/2-0.1;
        b.max_x = entity->location.x+CAR_LENGTH/2+0.1;
        b.min_y = entity->location.y+CAR_WIDTH/2;
        b.max_y = entity->location.y+CAR_WIDTH/2+0.1;
        p = p | (PERCEPTION_RIGHT * are_entity_box_hitting(box, b));
        
        b.min_x = entity->location.x+CAR_LENGTH/2+0.1;
        b.max_x = entity->location.x+CAR_LENGTH*5;
        b.min_y = entity->location.y-CAR_WIDTH/2;
        b.max_y = entity->location.y-CAR_WIDTH*1.5;
        p = p | (PERCEPTION_TOP_LEFT * are_entity_box_hitting(box, b));
        
        b.min_x = entity->location.x-CAR_LENGTH/2+0.1;
        b.max_x = entity->location.x+CAR_LENGTH*5;
        b.min_y = entity->location.y-CAR_WIDTH/2;
        b.max_y = entity->location.y+CAR_WIDTH/2;
        p = p | (PERCEPTION_TOP * are_entity_box_hitting(box, b));
        
        b.min_x = entity->location.x+CAR_LENGTH/2+0.1;
        b.max_x = entity->location.x+CAR_LENGTH*5;
        b.min_y = entity->location.y+CAR_WIDTH/2;
        b.max_y = entity->location.y+CAR_WIDTH*1.5;
        p = p | (PERCEPTION_TOP_RIGHT * are_entity_box_hitting(box, b));
    }
    
    return p;
}

bool make_action(__attribute__((unused)) Level* level, Entity* entity, Action action) {
    switch (action) {
        case ACTION_LEFT:
            entity->location.y -= 0.1;
        break;
        case ACTION_RIGHT:
            entity->location.y += 0.1;
        break;
        case ACTION_FASTER:
            if (entity->location.velocity < 0.4)
                entity->location.velocity += 0.01;
        break;
        case ACTION_SLOWER:
            if (entity->location.velocity > 0.15)
                entity->location.velocity -= 0.01;
        break;
        default:
        break;
    }
    
    return true;
}
