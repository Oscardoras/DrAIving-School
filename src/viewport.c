#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL_image.h>

#include "draw.h"
#include "learning.h"
#include "viewport.h"


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
    viewport->tilesets.preview = NULL;
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
 
    viewport->tilesets.preview = IMG_LoadTexture(viewport->renderer, "sprites/preview_2.png");
    viewport->tilesets.roads = IMG_LoadTexture(viewport->renderer, "sprites/sprites_road.png");
    viewport->tilesets.vehicles = IMG_LoadTexture(viewport->renderer, "sprites/cars.png");
    if (viewport->tilesets.roads == NULL || viewport->tilesets.vehicles == NULL || viewport->tilesets.preview == NULL) {
        SDL_Log("Error IMG");
        close_viewport(viewport);
        return NULL;
    }
    
    viewport->state = VIEWPORTSTATE_TITLE;
    
    if (TTF_Init() < 0) {
        SDL_Log("Error SDL - %s", SDL_GetError());
        close_viewport(viewport);
        return NULL;
    }
    viewport->font = TTF_OpenFont("./ttf/dogicabold.ttf", 65);
    if (viewport->font == NULL) {
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

void event_loop(Viewport* viewport) {
    bool quit = false;
    bool collision = false;
    
    SDL_Event event;
    
    int duree_frame = 1000/FPS;
    int lines = 2*LINES_PER_DIRECTION + 4;
    int side = viewport->height/lines + 1;
    
    while (!quit && !collision) {
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
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
                    if (viewport->state == VIEWPORTSTATE_TITLE) {
                        if(event.key.keysym.sym == SDLK_SPACE) viewport->state = VIEWPORTSTATE_GAME;
                        if(event.key.keysym.sym == SDLK_l) {
                            viewport->state = VIEWPORTSTATE_GAMEIA;
                            FILE *file = fopen("learning", "r");
                            if(file)
                            {
                                viewport->level->player->q = load_matrix(file);
                                fclose(file);
                            }
                        }
                    }
                }
        }
        

        
        if(viewport->state == VIEWPORTSTATE_GAMEIA) {
            make_action(viewport->level, viewport->level->player,
                e_greedy(viewport->level->player->q,
                    get_entity_perception(viewport->level, viewport->level->player)
                )
            );
        }
        else
        {
            const Uint8* keystates = SDL_GetKeyboardState(NULL);
            if (keystates[SDL_SCANCODE_UP])
                make_action(viewport->level, viewport->level->player, ACTION_LEFT);
            if (keystates[SDL_SCANCODE_DOWN])
                make_action(viewport->level, viewport->level->player, ACTION_RIGHT);
            if (keystates[SDL_SCANCODE_RIGHT])
                make_action(viewport->level, viewport->level->player, ACTION_FASTER);
            if (keystates[SDL_SCANCODE_LEFT])
                make_action(viewport->level, viewport->level->player, ACTION_SLOWER);
        }
        switch(viewport->state) {
        case VIEWPORTSTATE_GAME:
        case VIEWPORTSTATE_GAMEIA:
            collision = update_game(viewport->level);
            draw_road(viewport, lines, side);
            draw_cars(viewport, lines-4, side);
            SDL_RenderPresent(viewport->renderer);
            SDL_Delay(duree_frame);
            break;
        case VIEWPORTSTATE_TITLE:
            draw_viewportTitle(viewport);
            SDL_RenderPresent(viewport->renderer);
            SDL_Delay(duree_frame);
            break;
        }
    }
}
