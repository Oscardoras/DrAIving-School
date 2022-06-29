#include <stdlib.h>

#include "entity.h"


Entity* new_entity(EntityType type, Location location, Matrix* q) {
    Entity* entity = malloc(sizeof(Entity));
    
    if (entity != NULL) {
        entity->type = type;
        entity->location = location;
        entity->q = q;
    }
    
    return entity;
}

void free_entity(Entity* entity) {
    if (entity != NULL)
        free(entity);
}

HitBox get_entity_hitbox(Entity* entity) {
    HitBox b;
    b.min_x = entity->location.x - CAR_LENGTH/2;
    b.max_x = entity->location.x + CAR_LENGTH/2;
    b.min_y = entity->location.y - CAR_WIDTH/2;
    b.max_y = entity->location.y + CAR_WIDTH/2;
    return b;
}

bool are_entity_box_hitting(HitBox b1, HitBox b2) {
    if (b2.min_x <= b1.min_x && b1.min_x <= b2.max_x && b2.min_y <= b1.min_y && b1.min_y <= b2.max_y) return true;
    if (b2.min_x <= b1.max_x && b1.max_x <= b2.max_x && b2.min_y <= b1.min_y && b1.min_y <= b2.max_y) return true;
    if (b2.min_x <= b1.min_x && b1.min_x <= b2.max_x && b2.min_y <= b1.max_y && b1.max_y <= b2.max_y) return true;
    if (b2.min_x <= b1.max_x && b1.max_x <= b2.max_x && b2.min_y <= b1.max_y && b1.max_y <= b2.max_y) return true;
    return false;
}
