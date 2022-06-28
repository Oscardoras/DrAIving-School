#include <SDL2/SDL_image.h>

#include "level.h"
#include "viewport.h"

#define SCROLLING_SPEED 2
#define NB_LINES 3


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
    if (viewport->tilesets.roads == NULL || viewport->tilesets.vehicles == NULL) {
        SDL_Log("Error IMG");
        close_viewport(viewport);
        return NULL;
    }
    viewport->state = (ViewportState)(GAME);
    
    return viewport;
}

void close_viewport(Viewport* viewport) {
    if (viewport != NULL) {
        if (viewport->tilesets.vehicles != NULL) SDL_DestroyTexture(viewport->tilesets.vehicles);
        if (viewport->tilesets.roads != NULL) SDL_DestroyTexture(viewport->tilesets.roads);

        if (viewport->renderer != NULL) SDL_DestroyRenderer(viewport->renderer);
        if (viewport->window != NULL) SDL_DestroyWindow(viewport->window);
        free(viewport);
    }
    SDL_Quit();
}


void draw_road(Viewport* viewport, int lines, int side, int pos) { //Voies autoroute horizontales
    SDL_SetRenderDrawColor(viewport->renderer, 0, 0, 0, 255);
    SDL_RenderClear(viewport->renderer);
    
    SDL_Rect road[8];
    for (int i=0; i<8; i++) {
        road[i].x = 0;
        road[i].y = i*32;
        road[i].w = road[i].h = 32;
    }
    SDL_Rect dest;
    
    for(int x=-pos; x<viewport->width; x += side) {
        dest.x = x;
        dest.w = dest.h = side;
        
        dest.y = 0*side;
        SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[0], &dest);
        
        dest.y = 1*side;
        SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[1], &dest);
        
        dest.y = (lines-2)*side;
        SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.roads, &road[1], &dest, 0, NULL, SDL_FLIP_VERTICAL);
        
        dest.y = (lines-1)*side;
        SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[6], &dest);
        
        if (lines == 6) { //Une seule voie par sens
            dest.y = 2*side;
            SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[5], &dest);
            dest.y = 3*side; //Les deux sens sont sur les lignes 3 et 4 (sur 6 au total)
            SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.roads, &road[5], &dest, 0, NULL, SDL_FLIP_VERTICAL);
        }
        else {
            dest.y = 2*side;
            SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[2], &dest);
            dest.y = (lines-3)*side;
            SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.roads, &road[2], &dest, 0, NULL, SDL_FLIP_VERTICAL);
            
            dest.y = (lines/2-1)*side;
            SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[4], &dest);
            dest.y = (lines/2)*side;
            SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.roads, &road[4], &dest, 0, NULL, SDL_FLIP_VERTICAL);
            
            for(int i=3; i<lines/2-1; i++) {
                dest.y = i*side;
                SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[3], &dest);
                dest.y = (lines-1-i)*side;
                SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[3], &dest);
            }
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
    
    dest.x = 0;
    dest.w = side;
    dest.h = side/2;
    float entity_pos = (entity->location.y / viewport->level->width);
    dest.y = (2. + entity_pos * road_lines) * side;
    
    switch(entity->type) {
        case PLAYER_CAR:
            dest.x = 0.1 * viewport->width;
            break;
        case CAR:
            source.y = 16;
            dest.x = 0;
            if (entity->location.direction) flip = SDL_FLIP_HORIZONTAL;
            break;
    }
    
    SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.vehicles, &source, &dest, 0, NULL, flip);
}

void draw_cars(Viewport* viewport, int road_lines, int side) {
    draw_car(viewport, viewport->level->player, road_lines, side);
    
    struct EntityListCell* cour = viewport->level->entities;
    while(cour) {
        draw_car(viewport, cour->entity, road_lines, side);
        cour = cour->next;
    }
}

void draw_viewportTitle(Viewport* viewport, int lines, int pos, int side)
{

}

void event_loop(Viewport* viewport) {
    bool quit = false;
    
    SDL_Event event;
    
    int lines = 2*NB_LINES + 4;
    int side = viewport->height/lines + 1;
    int pos = 0;
    int scrolling_speed = SCROLLING_SPEED;
    
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        viewport->width = event.window.data1;
                        viewport->height = event.window.data2;
                        side = viewport->height/lines + 1; 
                    }
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        
                    }
            }
        }
        
        switch(viewport->state) {
            case ((ViewportState)(GAME)):
                draw_road(viewport, lines, side, pos);
                draw_cars(viewport, lines-4, side);
                pos = (pos+1) % side;
            break;
            case ((ViewportState)(TITLE)):
                draw_viewportTitle(viewport, lines, side, pos);
            break;
        }
        
        SDL_RenderPresent(viewport->renderer);
        SDL_Delay(5);
    }
}
