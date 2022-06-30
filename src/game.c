#include <stdlib.h>

#include "game.h"
#include "learning.h"


bool update_game(Level* level) {
    HitBox player_box = get_entity_hitbox(level->player);
    
    int tab[LINES_PER_DIRECTION*2];
    for (int n = 0; n < LINES_PER_DIRECTION*2; n++)
        tab[n] = 0;
    
    struct EntityListCell** ptr = &level->entities;
    while (*ptr != NULL) {
        if ((*ptr)->entity->location.x < level->player->location.x - 3*CAR_LENGTH) {
            struct EntityListCell* tmp = *ptr;
            *ptr = (*ptr)->next;
            free_entity(tmp->entity);
            free(tmp);
        } else {
            struct EntityListCell* it = *ptr;
            make_action(level, it->entity, e_greedy(it->entity->q, get_entity_perception(level, it->entity), 0.));
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
            location.x = level->player->location.x + CAR_LENGTH*(15 + 5*rand() / (float) RAND_MAX);
            location.y = ((n + 0.3) / (LINES_PER_DIRECTION*2)) * level->width;
            
            switch (n) {
            case 0:
                location.velocity = -CAR3_VELOCITY;
                break;
            case 1:
                location.velocity = -CAR2_VELOCITY;
                break;
            case 2:
                location.velocity = -CAR1_VELOCITY;
                break;
            case 3:
                location.velocity = CAR1_VELOCITY;
                break;
            case 4:
                location.velocity = CAR2_VELOCITY;
                break;
            case 5:
                location.velocity = CAR3_VELOCITY;
                break;
            default:
                location.velocity = 0;
                break;
            }
        
            add_level_entity(level, new_entity(CAR, location, level->matrix));
        }
    }
    
    //printf("Perception : %u\n", get_entity_perception(level, level->player));
    /*
    if(level->score%100 == 0) {
        car_cluster(level, 1);
        car_cluster(level, 0);
    }
    */
    level->player->location.x += level->player->location.velocity;
    if (player_box.min_y < 0 || player_box.max_y > level->width)
        return true;
    
    level->score++;
    return level->player->location.x >= level->length;
}

void get_entity_perception_hitbox(Entity* entity, HitBox boxes[8]) {
    boxes[0] = get_entity_hitbox(entity);
    boxes[1] = boxes[2] = boxes[3] = boxes[4] = boxes[5] = boxes[0];
    
    float width = boxes[0].max_y - boxes[0].min_y;
    float small_width = 0.25 * width;
    float medium_width = 0.5 * width;
    float length = boxes[0].max_x - boxes[0].min_x;
    float big_length = 3. * length;
    
    
    boxes[1].max_y = boxes[0].min_y;
    boxes[1].min_y = boxes[0].min_y - small_width;
    
    boxes[2].min_y = boxes[0].max_y;
    boxes[2].max_y = boxes[0].max_y + small_width;
    
    boxes[3].min_x = boxes[0].max_x;
    boxes[3].max_x = boxes[0].max_x + big_length;
    boxes[3].max_y = boxes[0].min_y;
    boxes[3].min_y = boxes[0].min_y - medium_width;
    
    boxes[4].min_x = boxes[0].max_x;
    boxes[4].max_x = boxes[0].max_x + big_length;
    boxes[4].min_y = boxes[0].min_y;
    boxes[4].max_y = boxes[0].max_y;
    
    boxes[5].min_x = boxes[0].max_x;
    boxes[5].max_x = boxes[0].max_x + big_length;
    boxes[5].min_y = boxes[0].max_y;
    boxes[5].max_y = boxes[0].max_y + medium_width;
    
    boxes[6].min_x = boxes[0].max_x;
    boxes[6].max_x = boxes[0].max_x + big_length;
    boxes[6].max_y = boxes[0].min_y - medium_width;
    boxes[6].min_y = boxes[0].min_y -  medium_width - medium_width;
    
    boxes[7].min_x = boxes[0].max_x;
    boxes[7].max_x = boxes[0].max_x + big_length;
    boxes[7].min_y = boxes[0].max_y + medium_width;
    boxes[7].max_y = boxes[0].max_y + medium_width + medium_width;
}

Perception get_entity_perception(Level* level, Entity* entity) {
    Perception p = 0b0;
    
    HitBox boxes[8];
    get_entity_perception_hitbox(entity, boxes);
    
    HitBox top;
    HitBox bot;
    bot.min_x = top.min_x = 0.;
    bot.max_x = top.max_x = level->length;
    top.min_y = -CAR_WIDTH;
    top.max_y = 0.;
    bot.min_y = level->width;
    bot.max_y = level->width + CAR_WIDTH;
    
    p = p | (PERCEPTION_LEFT * are_entity_box_hitting(boxes[1], top));
    p = p | (PERCEPTION_RIGHT * are_entity_box_hitting(boxes[2], bot));
    p = p | (PERCEPTION_TOP_LEFT * are_entity_box_hitting(boxes[3], top));
    p = p | (PERCEPTION_TOP_RIGHT * are_entity_box_hitting(boxes[5], bot));
    p = p | (PERCEPTION_FAR_LEFT * are_entity_box_hitting(boxes[6], top));
    p = p | (PERCEPTION_FAR_RIGHT * are_entity_box_hitting(boxes[7], bot));
    
    for (struct EntityListCell* it = level->entities; it != NULL; it = it->next) if (it->entity != entity) {
        HitBox it_box = get_entity_hitbox(it->entity);
        
        p = p | (PERCEPTION_LEFT * are_entity_box_hitting(boxes[1], it_box));
        p = p | (PERCEPTION_RIGHT * are_entity_box_hitting(boxes[2], it_box));
        p = p | (PERCEPTION_TOP_LEFT * are_entity_box_hitting(boxes[3], it_box));
        p = p | (PERCEPTION_TOP * are_entity_box_hitting(boxes[4], it_box));
        p = p | (PERCEPTION_TOP_RIGHT * are_entity_box_hitting(boxes[5], it_box));
        p = p | (PERCEPTION_FAR_LEFT * are_entity_box_hitting(boxes[6], it_box));
        p = p | (PERCEPTION_FAR_RIGHT * are_entity_box_hitting(boxes[7], it_box));
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
        cars_locations[i].velocity = (!line) ? -CAR2_VELOCITY : CAR2_VELOCITY;
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
