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
    /*
    for (int n = 0; n < LINES_PER_DIRECTION*2; n++) {
        if (tab[n] < CAR_PER_LINE) {
            Location location;
            location.x = level->player->location.x + CAR_LENGTH*(10 + 10 * (rand() / (float) RAND_MAX));
            location.y = ((n + 0.3) / (LINES_PER_DIRECTION*2)) * level->width;
            location.velocity = n < LINES_PER_DIRECTION ? -DEFAULT_PLAYER_VELOCITY : DEFAULT_PLAYER_VELOCITY;
        
            add_level_entity(level, new_entity(CAR, location, level->matrix));
        }
    }
    */
    //printf("Perception : %u\n", get_entity_perception(level, level->player));
    if(level->score%100 == 0) {
        car_cluster(level, 1);
        car_cluster(level, 0);
    }
    level->player->location.x += level->player->location.velocity;
    if (player_box.min_y < 0 || player_box.max_y > level->width)
        return true;
    
    level->score++;
    return level->player->location.x >= level->length;
}

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

Perception get_entity_perception(Level* level, Entity* entity) {
    Perception p = 0b0;
    
    HitBox box = get_entity_hitbox(entity);
    float width = box.max_y - box.min_y;
    float small_width = 0.5 * width;
    float length = box.max_x - box.min_x;
    float big_length = 5. * length;
    
    p = p | (PERCEPTION_LEFT * (box.min_y - small_width < 0));
    p = p | (PERCEPTION_RIGHT * (box.max_y + small_width > level->width));
    
    for (struct EntityListCell* it = level->entities; it != NULL; it = it->next) if (it->entity != entity) {
        HitBox it_box = get_entity_hitbox(it->entity);
        HitBox b;
        
        b = box;
        b.max_y = box.min_y;
        b.min_y = box.min_y - small_width;
        p = p | (PERCEPTION_LEFT * are_entity_box_hitting(b, it_box));
        
        b = box;
        b.min_y = box.max_y;
        b.max_y = box.max_y + small_width;
        p = p | (PERCEPTION_RIGHT * are_entity_box_hitting(b, it_box));
        
        b.min_x = box.max_x;
        b.max_x = box.max_x + big_length;
        b.max_y = box.min_y;
        b.min_y = box.min_y - width;
        p = p | (PERCEPTION_TOP_LEFT * are_entity_box_hitting(b, it_box));
        
        b = box;
        b.min_x = box.max_x;
        b.max_x = box.max_x + big_length;
        p = p | (PERCEPTION_TOP * are_entity_box_hitting(b, it_box));
        
        b.min_x = box.max_x;
        b.max_x = box.max_x + big_length;
        b.min_y = box.max_y;
        b.max_y = box.max_y + width;
        p = p | (PERCEPTION_TOP_RIGHT * are_entity_box_hitting(b, it_box));
    }

    return p;
}

bool make_action(__attribute__((unused)) Level* level, Entity* entity, Action action) {
    switch (action) {
    case ACTION_LEFT:
        entity->location.y -= 0.1;
        return true;
    case ACTION_RIGHT:
        entity->location.y += 0.1;
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

void car_cluster(Level* level, int line) {
    Location cars_locations[3];
    for(int i=0; i<3; i++) {
        cars_locations[i].y = ((3.*line + i + 0.5) / (LINES_PER_DIRECTION*2)) * level->width;
        cars_locations[i].velocity = (!line) ? -DEFAULT_CAR_VELOCITY : DEFAULT_CAR_VELOCITY;
    }
    
    int car_mid = rand()%3;
    int car_gauche,
        car_gauche_2,
        car_droite_2,
        car_pos;
    switch (car_mid) {
        case 0:
            car_gauche_2 = rand()%2;
            car_droite_2 = 1-car_gauche_2;
            cars_locations[1].x = level->player->location.x + 13*CAR_LENGTH;
            cars_locations[0].x = cars_locations[1].x + (car_gauche_2+1.) * 1.2 * CAR_LENGTH;
            cars_locations[2].x = cars_locations[1].x + (car_droite_2+1.) * 1.2 * CAR_LENGTH;
            break;
        case 1:
            car_gauche = rand()%2 * 2;
            car_pos = rand()%3 - 1;
            cars_locations[1].x = level->player->location.x + 14.2*CAR_LENGTH;
            cars_locations[car_gauche].x = cars_locations[1].x + (float) car_pos * 1.2 * CAR_LENGTH;
            cars_locations[2 - car_gauche].x = cars_locations[1].x;
            cars_locations[2 - car_gauche].y = 20.;
            break;
        case 2:
            car_gauche_2 = rand()%2;
            car_droite_2 = 1-car_gauche_2;
            cars_locations[1].x = level->player->location.x + 15.4*CAR_LENGTH;
            cars_locations[0].x = cars_locations[1].x - (car_gauche_2+1.) * 1.2 * CAR_LENGTH;
            cars_locations[2].x = cars_locations[1].x - (car_droite_2+1.) * 1.2 * CAR_LENGTH;
            break;
    }
    
    for(int i=0; i<3; i++) add_level_entity(level, new_entity(CAR, cars_locations[i], level->matrix));
}