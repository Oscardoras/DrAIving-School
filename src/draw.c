#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "draw.h"


void draw_road(Viewport* viewport, int lines, int side) { //Voies autoroute horizontales
    SDL_SetRenderDrawColor(viewport->renderer, 0, 0, 0, 255);
    SDL_RenderClear(viewport->renderer);
    
    SDL_Rect road[8]; //Recup des textures sur sprites_road.png
    for (int i = 0; i < 8; i++) {
        road[i].x = 0;
        road[i].y = i*32;
        road[i].w = road[i].h = 32;
    }
    SDL_Rect dest;

    float player_x = viewport->level->player->location.x;
    float dec_player_x = ((int) (player_x/2.)*2) - player_x;
    
    for (float x = dec_player_x; x*side < viewport->width*2; x += 2.) {
        for (int i = 0; i < 2; i++) {
            dest.x = (int) ((x+i) * side);
            dest.w = dest.h = side;
        
            dest.y = 0*side; //1ere ligne : mur en briques
            SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[0], &dest);
        
            dest.y = 1*side; //2eme ligne : trottoir
            SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[1], &dest);
        
            dest.y = (lines-2)*side; //Avant-derniere ligne : trottoir
            SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.roads, &road[1], &dest, 0, NULL, SDL_FLIP_VERTICAL);
        
            dest.y = (lines-1)*side; //Derniere ligne : toit
            SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[6], &dest);
        
            if (lines == 6) { //Une seule voie par sens : voie avec ligne jaune sans voie exterieure
                dest.y = 2*side;
                SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[5], &dest);
                dest.y = 3*side; //Les deux sens sont sur les lignes 3 et 4 (sur 6 au total)
                SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.roads, &road[5], &dest, 0, NULL, SDL_FLIP_VERTICAL);
            }
            else {
                dest.y = 2*side; //1ere voie : voie sans ligne jaune ni voie exterieure
                SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[2], &dest);
                dest.y = (lines-3)*side;
                SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.roads, &road[2], &dest, 0, NULL, SDL_FLIP_VERTICAL);
            
                dest.y = (lines/2-1)*side; //Derniere voie : voie avec ligne jaune et voie exterieure
                SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[4], &dest);
                dest.y = (lines/2)*side;
                SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.roads, &road[4], &dest, 0, NULL, SDL_FLIP_VERTICAL);
            
                for (int i = 3; i < lines/2 - 1; i++) { //Voies entre : voie avec voieS exterieures sans ligne jaune
                    dest.y = i*side;
                    SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[3], &dest);
                    dest.y = (lines-1-i)*side;
                    SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[3], &dest);
                }
            }
        }
    }
}

void draw_score(Viewport* viewport) {
    SDL_Color color = {0, 0, 0, 255};

    {
        char score[256];
        sprintf(score, "Temps : %d          Fin : %d", viewport->level->score / FPS, (int) (viewport->level->length - viewport->level->player->location.x));

        SDL_Surface* text_surface = TTF_RenderText_Blended(viewport->font, score, color);
        if (text_surface) {
            SDL_Texture* text_texture = SDL_CreateTextureFromSurface(viewport->renderer, text_surface);
            if (text_texture) {
                SDL_Rect ttfdest = {10, 10, 500, 30};
                SDL_RenderCopy(viewport->renderer, text_texture, NULL, &ttfdest);
                SDL_DestroyTexture(text_texture);
                SDL_FreeSurface(text_surface);
            } else {
                SDL_Log("Error SDL - %s", SDL_GetError());
                SDL_FreeSurface(text_surface);
            }
        } else {
            SDL_Log("Error TTF - %s", TTF_GetError());
        }
    }

    if (viewport->state == VIEWPORTSTATE_VICTORY || viewport->state == VIEWPORTSTATE_DEFEAT) {
        char msg[256];
        switch (viewport->state) {
        case VIEWPORTSTATE_VICTORY:
            strcpy(msg, "GG EZ");
            break;
        case VIEWPORTSTATE_DEFEAT:
            strcpy(msg, "Press F to pay respect");
            break;
        default:
            break;
        }

        SDL_Surface* text_surface = TTF_RenderText_Blended(viewport->font, msg, color);
        if (text_surface) {
            SDL_Texture* text_texture = SDL_CreateTextureFromSurface(viewport->renderer, text_surface);
            if (text_texture) {
                SDL_Rect ttfdest = {viewport->width / 2 - 250, viewport->height / 2 - 50, 500, 100};
                SDL_RenderCopy(viewport->renderer, text_texture, NULL, &ttfdest);
                SDL_DestroyTexture(text_texture);
                SDL_FreeSurface(text_surface);
            } else {
                SDL_Log("Error SDL - %s", SDL_GetError());
                SDL_FreeSurface(text_surface);
            }
        } else {
            SDL_Log("Error TTF - %s", TTF_GetError());
        }
    }
}

void draw_car(Viewport* viewport, Entity* entity, int road_lines, int side) {
    SDL_Rect source; //Quel sprite prendre sur le cars.png
    SDL_Rect dest; //Où mettre la voiture
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    
    source.x = 0;
    source.y = 0;
    source.w = 32;
    source.h = 16;
    
    dest.w = side; //Longueur d'une voiture = longueur d'un bloc de route.
    dest.x = side; //Position du joueur, a une longueur de voiture du bord gauche de l'ecran.
    dest.h = side / (CAR_LENGTH/CAR_WIDTH);
    float y = get_entity_hitbox(entity).min_y / (viewport->level->width - CAR_WIDTH);
    float line_per_width = road_lines / viewport->level->width;
    dest.y = (int) ((2. + y * (road_lines - line_per_width * CAR_WIDTH)) * (float) side);
    
    switch (entity->type) {
    case PLAYER_CAR:
        break;
    case CAR:
        source.y = 16;
        dest.x += ((entity->location.x - viewport->level->player->location.x) * side)/CAR_LENGTH;
        if (entity->location.velocity > 0) flip = SDL_FLIP_HORIZONTAL;
        break;
    }
    
    if (dest.x > -side) SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.vehicles, &source, &dest, 0, NULL, flip);
    
    
    if (viewport->display_hitboxes) {
        float px = entity->location.x;
        float py = entity->location.y;
        SDL_SetRenderDrawColor(viewport->renderer, 255, 255, 255, 255);
        if (entity->type == PLAYER_CAR) {
            HitBox boxes[PERCEPTIONS];
            get_entity_perception_hitbox(entity, boxes);
        
            for (HitBox* b=boxes; b<boxes+PERCEPTIONS; b++) {
                b->min_x += CAR_LENGTH/2;
                b->max_x += CAR_LENGTH/2;
                b->min_y += CAR_WIDTH/2;
                b->max_y += CAR_WIDTH/2;

                SDL_RenderDrawLine(viewport->renderer, dest.x + (b->min_x - px)*side/2, dest.y + (b->min_y - py)*side/2, dest.x + (b->max_x - px)*side/2, dest.y + (b->min_y - py)*side/2);
                SDL_RenderDrawLine(viewport->renderer, dest.x + (b->min_x - px)*side/2, dest.y + (b->max_y - py)*side/2, dest.x + (b->max_x - px)*side/2, dest.y + (b->max_y - py)*side/2);
                SDL_RenderDrawLine(viewport->renderer, dest.x + (b->min_x - px)*side/2, dest.y + (b->min_y - py)*side/2, dest.x + (b->min_x - px)*side/2, dest.y + (b->max_y - py)*side/2);
                SDL_RenderDrawLine(viewport->renderer, dest.x + (b->max_x - px)*side/2, dest.y + (b->min_y - py)*side/2, dest.x + (b->max_x - px)*side/2, dest.y + (b->max_y - py)*side/2);
            }
        } else {
            HitBox b = get_entity_hitbox(entity);
            
            b.min_x += CAR_LENGTH/2;
            b.max_x += CAR_LENGTH/2;
            b.min_y += CAR_WIDTH/2;
            b.max_y += CAR_WIDTH/2;

            SDL_RenderDrawLine(viewport->renderer, dest.x + (b.min_x - px)*side/2, dest.y + (b.min_y - py)*side/2, dest.x + (b.max_x - px)*side/2, dest.y + (b.min_y - py)*side/2);
            SDL_RenderDrawLine(viewport->renderer, dest.x + (b.min_x - px)*side/2, dest.y + (b.max_y - py)*side/2, dest.x + (b.max_x - px)*side/2, dest.y + (b.max_y - py)*side/2);
            SDL_RenderDrawLine(viewport->renderer, dest.x + (b.min_x - px)*side/2, dest.y + (b.min_y - py)*side/2, dest.x + (b.min_x - px)*side/2, dest.y + (b.max_y - py)*side/2);
            SDL_RenderDrawLine(viewport->renderer, dest.x + (b.max_x - px)*side/2, dest.y + (b.min_y - py)*side/2, dest.x + (b.max_x - px)*side/2, dest.y + (b.max_y - py)*side/2);
        }
    }
}

void draw_cars(Viewport* viewport, int road_lines, int side) {
    for (struct EntityListCell* it = viewport->level->entities; it != NULL; it = it->next)
        draw_car(viewport, it->entity, road_lines, side);
    
    draw_car(viewport, viewport->level->player, road_lines, side);
}

void draw_menu(Viewport* viewport) {
    SDL_RenderCopy(viewport->renderer, viewport->tilesets.menu, NULL, NULL); 

    float scaleX = viewport->width / 628.;
    float scaleY = viewport->height / 500.;

    if (time(NULL) % 2 == 0) {
        SDL_Color color = {0, 0, 0, 255};
        SDL_Surface* text_surface = TTF_RenderText_Blended(viewport->font, "Press Enter", color);
        if (text_surface == NULL) {
            SDL_Log("Error SDL - %s", "cant create surface");
            close_viewport(viewport);
        }

        SDL_Texture* text_texture = NULL;
        text_texture = SDL_CreateTextureFromSurface(viewport->renderer, text_surface);
        if (text_texture == NULL) {
            SDL_Log("Error SDL - %s", "cant create texture");
            close_viewport(viewport);
        }

        SDL_Rect ttfdest = {110*scaleX, 330*scaleY, 200*scaleX, 80*scaleY};
        //SDL_QueryTexture(text_texture, NULL, NULL, &pos.w, &pos.h);
        SDL_RenderCopy(viewport->renderer, text_texture, NULL, &ttfdest);
        SDL_DestroyTexture(text_texture);           
        SDL_FreeSurface(text_surface);
    }
}
