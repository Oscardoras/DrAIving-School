#include "game.h"


bool update_game(Level* level) {
    for (struct EntityListCell* it = level->entities; it != NULL; it = it->next) {
        make_action(level, it->entity, choose_action(level, it->entity));
        
        if (are_entity_box_hitting(get_entity_hitbox(it->entity), get_entity_hitbox(level->player)))
            return true;
    }
    
    return false;
}

Action choose_action(Level* level, Entity* entity) {
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
    
    return forward_state(entity->markov, p);
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

bool are_entity_box_hitting(HitBox b1, HitBox b2) {
    if (b2.min_x <= b1.min_x && b1.min_x <= b2.max_x && b2.min_y <= b1.min_y && b1.min_y <= b2.max_y) return true;
    if (b2.min_x <= b1.max_x && b1.max_x <= b2.max_x && b2.min_y <= b1.min_y && b1.min_y <= b2.max_y) return true;
    if (b2.min_x <= b1.min_x && b1.min_x <= b2.max_x && b2.min_y <= b1.max_y && b1.max_y <= b2.max_y) return true;
    if (b2.min_x <= b1.max_x && b1.max_x <= b2.max_x && b2.min_y <= b1.max_y && b1.max_y <= b2.max_y) return true;
    return false;
}

HitBox get_entity_hitbox(Entity* entity) {
    HitBox b;
    b.min_x = entity->location.x - CAR_LENGTH/2;
    b.max_x = entity->location.x + CAR_LENGTH/2;
    b.min_y = entity->location.y - CAR_WIDTH/2;
    b.min_y = entity->location.y + CAR_WIDTH/2;
    return b;
}