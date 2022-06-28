#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_image.h>

#include "game.h"
#include "level.h"
#include "viewport.h"
#include "learning.h"

Viewport* create_viewport(int width, int height, Level* level) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error SDL - %s", SDL_GetError());
        return NULL;
    }
    
    Viewport* viewport = malloc(sizeof(Viewport));
    if (viewport == NULL) {
        SDL_Quit();
        return NULL;
    }

    viewport->width = width;
    viewport->height = height;
    viewport->level = level;
    viewport->window = NULL;
    viewport->renderer = NULL;
    viewport->tilesets.roads = NULL;
    viewport->tilesets.vehicles = NULL;
    viewport->animation_loop = 0;
    
    viewport->window = SDL_CreateWindow("Jeu",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_RESIZABLE
    );
    if (viewport->window == NULL) {
        SDL_Log("Error SDL - %s", SDL_GetError());
        close_viewport(viewport);
        return NULL;
    }

    viewport->renderer = SDL_CreateRenderer(
        viewport->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (viewport->renderer == NULL) {
        SDL_Log("Error SDL - %s", SDL_GetError());
        close_viewport(viewport);
        return NULL;
    }
 
    viewport->tilesets.roads = IMG_LoadTexture(viewport->renderer, "sprites/sprites_road.png");
    viewport->tilesets.vehicles = IMG_LoadTexture(viewport->renderer, "sprites/cars.png");
    viewport->tilesets.preview = IMG_LoadTexture(viewport->renderer, "sprites/preview_2.png");
    if (viewport->tilesets.roads == NULL || viewport->tilesets.vehicles == NULL || viewport->tilesets.preview == NULL) {
        SDL_Log("Error IMG");
        close_viewport(viewport);
        return NULL;
    }
    
    viewport->state = (ViewportState)(TITLE);
    
    if (TTF_Init() < 0) 
    {
        SDL_Log("Error SDL - %s", SDL_GetError());
        close_viewport(viewport);
        return NULL;
    }
    viewport->font = TTF_OpenFont("./ttf/dogicabold.ttf", 65);
    if (viewport->font == NULL)
    {
        SDL_Log("Error SDL - %s", "cant load font");
        close_viewport(viewport);
        return NULL;
    }
        
    return viewport;
}

void close_viewport(Viewport* viewport) {
    if (viewport != NULL) {
        if (viewport->tilesets.vehicles != NULL) SDL_DestroyTexture(viewport->tilesets.vehicles);
        if (viewport->tilesets.roads != NULL) SDL_DestroyTexture(viewport->tilesets.roads);
        if (viewport->tilesets.preview != NULL) SDL_DestroyTexture(viewport->tilesets.preview);
        if (viewport->renderer != NULL) SDL_DestroyRenderer(viewport->renderer);
        if (viewport->window != NULL) SDL_DestroyWindow(viewport->window);
        free(viewport);
    }
    TTF_Quit();
    SDL_Quit();
}


void draw_road(Viewport* viewport, int lines, int side, int pos) { //Voies autoroute horizontales
    SDL_SetRenderDrawColor(viewport->renderer, 0, 0, 0, 255);
    SDL_RenderClear(viewport->renderer);
    
    SDL_Rect road[8]; //Récup des textures sur sprites_road.png
    for (int i=0; i<8; i++) {
        road[i].x = 0;
        road[i].y = i*32;
        road[i].w = road[i].h = 32;
    }
    SDL_Rect dest;
    
    for(int x=-pos; x<viewport->width; x += side) {
        dest.x = x;
        dest.w = dest.h = side;
        
        dest.y = 0*side; //1ère ligne : mur en briques
        SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[0], &dest);
        
        dest.y = 1*side; //2ème ligne : trottoir
        SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[1], &dest);
        
        dest.y = (lines-2)*side; //Avant-dernière ligne : trottoir
        SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.roads, &road[1], &dest, 0, NULL, SDL_FLIP_VERTICAL);
        
        dest.y = (lines-1)*side; //Dernière ligne : toit
        SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[6], &dest);
        
        if (lines == 6) { //Une seule voie par sens : voie avec ligne jaune sans voie extérieure
            dest.y = 2*side;
            SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[5], &dest);
            dest.y = 3*side; //Les deux sens sont sur les lignes 3 et 4 (sur 6 au total)
            SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.roads, &road[5], &dest, 0, NULL, SDL_FLIP_VERTICAL);
        }
        else {
            dest.y = 2*side; //1ère voie : voie sans ligne jaune ni voie extérieure
            SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[2], &dest);
            dest.y = (lines-3)*side;
            SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.roads, &road[2], &dest, 0, NULL, SDL_FLIP_VERTICAL);
            
            dest.y = (lines/2-1)*side; //Dernière voie : voie avec ligne jaune et voie extérieure
            SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[4], &dest);
            dest.y = (lines/2)*side;
            SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.roads, &road[4], &dest, 0, NULL, SDL_FLIP_VERTICAL);
            
            for(int i=3; i<lines/2-1; i++) { //Voies entre : voie avec voieS extérieureS sans ligne jaune
                dest.y = i*side;
                SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[3], &dest);
                dest.y = (lines-1-i)*side;
                SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[3], &dest);
            }
        }
    }
    
    
    float scaleX = 1;
    float scaleY = 1;
    scaleX = (float)viewport->width/800.0;
    scaleY = (float)viewport->height/600.0;
    
    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface* text_surface = NULL;
    char score[20] = "Score : ";
    sprintf(score,"Score : %d", viewport->level->score);
    text_surface = TTF_RenderText_Blended(viewport->font, score, color);
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

    SDL_Rect ttfdest = {10*scaleX, 10*scaleY, 200*scaleX, 40*scaleY};
    //SDL_QueryTexture(text_texture, NULL, NULL, &pos.w, &pos.h);
    SDL_RenderCopy(viewport->renderer, text_texture, NULL, &ttfdest);
    SDL_DestroyTexture(text_texture);           
    SDL_FreeSurface(text_surface);
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
    dest.x = side; //Position du joueur, à une longueur de voiture du bord gauche de l'écran.
    dest.h = side / (CAR_LENGTH/CAR_WIDTH);
    float entity_pos = (entity->location.y / viewport->level->width);
    dest.y = (2. + entity_pos * road_lines) * side;
    
    switch(entity->type) {
        case PLAYER_CAR:
            break;
        case CAR:
            source.y = 16;
            dest.x += ((entity->location.x - viewport->level->player->location.x) * side)/CAR_LENGTH;
            if (entity->location.direction) flip = SDL_FLIP_HORIZONTAL;
            break;
    }
    
    if (dest.x > -side) SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.vehicles, &source, &dest, 0, NULL, flip);
}

void draw_cars(Viewport* viewport, int road_lines, int side) {
    draw_car(viewport, viewport->level->player, road_lines, side);
    
    struct EntityListCell* cour = viewport->level->entities;
    while(cour) {
        draw_car(viewport, cour->entity, road_lines, side);
        cour = cour->next;
    }
}

void draw_viewportTitle(Viewport* viewport)
{
    SDL_RenderClear(viewport->renderer);
    float scaleX = 1;
    float scaleY = 1;
    scaleX = (float)viewport->width/628.0;
    scaleY = (float)viewport->height/500.0;
    SDL_Rect sprites[1];

    sprites[0].x=0;
    sprites[0].y=0;
    sprites[0].w=628;
    sprites[0].h=500;
    SDL_Rect dest;
    dest.x = 0*scaleX;
    dest.y = 0*scaleY;
    dest.w = 628*scaleX;
    dest.h = 500*scaleY;
    SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.preview, &sprites[0], &dest, 0, NULL, SDL_FLIP_NONE); 

    if(time(0)%2 == 0)
    {
        SDL_Color color = {0, 0, 0, 255};
        SDL_Surface* text_surface = NULL;
        text_surface = TTF_RenderText_Blended(viewport->font, "Press space", color);
        if (text_surface == NULL) 
        {
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

void event_loop(Viewport* viewport) {
    bool quit = false;
    bool collision = false;
    
    SDL_Event event;
    
    int duree_frame = 1000/FPS;
    int lines = 2*NB_LINES + 4;
    int side = viewport->height/lines + 1;
    int pos = 0;
    float scrolling_speed = (5.*viewport->level->player->location.velocity*(float) side)/FPS; //Vitesse du joueur : 5 blocs/seconde
    
    while (!quit & !collision) {
        while (SDL_PollEvent(&event)) {
            switch(event.type)
            {
                case SDL_QUIT: quit = true;
                    break;
                    
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        viewport->width = event.window.data1;
                        viewport->height = event.window.data2;
                        side = viewport->height/lines + 1;
                    }
                    break;
                    
                case SDL_KEYDOWN: 
                    switch (viewport->state) {
                        case TITLE:
                            if(event.key.keysym.sym == SDLK_SPACE) viewport->state = GAME;
                            if(event.key.keysym.sym == SDLK_l) 
                                {
                                    viewport->state = GAMEIA;
                                    FILE *file = fopen("learning", "r");
                                    if(file)
                                    {
                                        viewport->level->player->markov = load_matrix(file);
                                        fclose(file);
                                    }
                                }
                            break;
                        default:
                            switch (event.key.keysym.sym) {
                                case SDLK_RIGHT:
                                    make_action(viewport->level, viewport->level->player, ACTION_FASTER);
                                    scrolling_speed = (5.*viewport->level->player->location.velocity*(float) side)/FPS;
                                    break;
                                case SDLK_LEFT:
                                    make_action(viewport->level, viewport->level->player, ACTION_SLOWER);
                                    scrolling_speed = (5.*viewport->level->player->location.velocity*(float) side)/FPS;
                                    break;
                                case SDLK_UP:
                                    make_action(viewport->level, viewport->level->player, ACTION_LEFT);
                                    break;
                                case SDLK_DOWN:
                                    make_action(viewport->level, viewport->level->player, ACTION_RIGHT);
                                    break;
                            }
                    }
                    break;
            
                }
        }
            
        
        if(viewport->state == GAMEIA)
        {
            make_action(viewport->level, viewport->level->player,
                e_greedy(viewport->level->player->markov,
                    get_entity_perception(viewport->level, viewport->level->player)
                )
            );
            scrolling_speed = (5.*viewport->level->player->location.velocity*(float) side)/FPS;
        }
        switch(viewport->state) {
            
            case GAME:
                collision = update_game(viewport->level);
                draw_road(viewport, lines, side, pos);
                draw_cars(viewport, lines-4, side);
                pos = (int) (pos + scrolling_speed) % side;
                SDL_RenderPresent(viewport->renderer);
                SDL_Delay(duree_frame);
                break;
            case (GAMEIA):
                collision = update_game(viewport->level);
                break;
            case (TITLE):
                draw_viewportTitle(viewport);
                SDL_RenderPresent(viewport->renderer);
                SDL_Delay(duree_frame);
                break;
        }
    }
}