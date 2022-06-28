#include "game.h"


bool update_game(Level* level) {
    HitBox player_box = get_entity_hitbox(level->player);
    
    for (struct EntityListCell* it = level->entities; it != NULL; it = it->next) {
        make_action(level, it->entity, forward_state(it->entity->markov, get_entity_perception(level, it->entity)));
        it->entity->location.x += it->entity->location.velocity;
        
        if (are_entity_box_hitting(get_entity_hitbox(it->entity), player_box))
            return true;
    }
    
    level->player->location.x += level->player->location.velocity;
    if (player_box.min_y < 0 || player_box.max_y > level->width)
        return true;
    
    level->score++;
    return false;
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
        default:
        break;
    }
    
    return true;
}
