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
        } 
        else {
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
            location.velocity = n < LINES_PER_DIRECTION ? -DEFAULT_PLAYER_VELOCITY/2 : DEFAULT_PLAYER_VELOCITY/2;
        
            add_level_entity(level, new_entity(CAR, location, level->matrix));
        }
    }
    
    //printf("Perception : %u\n", get_entity_perception(level, level->player));
    
    for(int i=0; i<WIDTH_PERCEPTS; i++) {
        for(int j=0; j<2*LINES_PER_DIRECTION; j++) {
            level->percepts[j*WIDTH_PERCEPTS + i].location.x += level->player->location.velocity;
        }
    }
    
    level->player->location.x += level->player->location.velocity;
    if (player_box.min_y < 0 || player_box.max_y > level->width)
        return true;
    
    level->score++;
    return level->player->location.x >= level->length;
}

/*
HitBox* get_entity_perception_hitbox(Entity* entity) {
    HitBox* tab = malloc(6*sizeof(HitBox));
    
    tab[0] = get_entity_hitbox(entity);
    tab[1] = tab[2] = tab[3] = tab[4] = tab[5] = tab[0];
    
    float width = tab[0].max_y - tab[0].min_y;
    float small_width = 0.5 * width;
    float length = tab[0].max_x - tab[0].min_x;
    float big_length = 5. * length;
    
    
    
    tab[1].max_y = tab[0].min_y;
    tab[1].min_y = tab[0].min_y - small_width;
    
    
    tab[2].min_y = tab[0].max_y;
    tab[2].max_y = tab[0].max_y + small_width;
    
    tab[3].min_x = tab[0].max_x;
    tab[3].max_x = tab[0].max_x + big_length;
    tab[3].max_y = tab[0].min_y;
    tab[3].min_y = tab[0].min_y - width;
    
    tab[4].min_x = tab[0].max_x;
    tab[4].max_x = tab[0].max_x + big_length;
    
    tab[5].min_x = tab[0].max_x;
    tab[5].max_x = tab[0].max_x + big_length;
    tab[5].min_y = tab[0].max_y;
    tab[5].max_y = tab[0].max_y + width;

    return tab;
}
*/

Perception get_entity_perception(Level* level, Entity* entity) {
    Perception p = 0b0;
    HitBox b = get_entity_hitbox(entity);
    int p_aux = 1;
    bool hit = false;
    for(int i=0; i<2*LINES_PER_DIRECTION; i++) {
        p += (p_aux * are_entity_box_hitting(b, get_entity_hitbox(&(level->percepts[i*WIDTH_PERCEPTS]))));
        p_aux *= 2;
        
        hit = false;
        struct EntityListCell* cour = level->entities;
        while (!hit && cour) {
            if (are_entity_box_hitting(get_entity_hitbox(cour->entity), get_entity_hitbox(&(level->percepts[i*WIDTH_PERCEPTS + 1])))) {
                hit = true;
                cour = NULL;
            }
            else cour = cour->next;
        }
        p += (p_aux * hit);
        p_aux *= 2;
    }
    return p;
}

bool make_action(__attribute__((unused)) Level* level, Entity* entity, Action action) {
    switch (action) {
    case ACTION_LEFT:
        entity->location.y -= 0.15;
        return true;
    case ACTION_RIGHT:
        entity->location.y += 0.15;
        return true;
    case ACTION_FASTER:
        if (entity->location.velocity + 0.01 <= 2*DEFAULT_PLAYER_VELOCITY) {
            entity->location.velocity += 0.01;
            return true;
        } else return false;
    case ACTION_SLOWER:
        if (entity->location.velocity -0.01 >= DEFAULT_PLAYER_VELOCITY) {
            entity->location.velocity -= 0.01;
            return true;
        } else return false;
    default:
        return false;
    }
}
