#include <stdlib.h>
#include <time.h>

#include "draw.h"
#include "learning.h"
#include "viewport.h"


Viewport* create_viewport(int width, int height, Level* level) {
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
    viewport->tilesets.menu = NULL;
    viewport->tilesets.roads = NULL;
    viewport->tilesets.vehicles = NULL;
    viewport->state = VIEWPORTSTATE_MENU;

    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error SDL - %s", SDL_GetError());
        close_viewport(viewport);
        return NULL;
    }
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_Log("Error IMG - %s", IMG_GetError());
        close_viewport(viewport);
        return NULL;
    }
    if (TTF_Init()) {
        SDL_Log("Error TTF - %s", TTF_GetError());
        close_viewport(viewport);
        return NULL;
    }
    
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
 
    viewport->tilesets.menu = IMG_LoadTexture(viewport->renderer, "sprites/preview_2.png");
    viewport->tilesets.roads = IMG_LoadTexture(viewport->renderer, "sprites/sprites_road.png");
    viewport->tilesets.vehicles = IMG_LoadTexture(viewport->renderer, "sprites/cars.png");
    if (viewport->tilesets.roads == NULL || viewport->tilesets.vehicles == NULL || viewport->tilesets.menu == NULL) {
        SDL_Log("Error IMG - %s", IMG_GetError());
        close_viewport(viewport);
        return NULL;
    }

    viewport->font = TTF_OpenFont("./ttf/dogicabold.ttf", 65);
    if (viewport->font == NULL) {
        SDL_Log("Error TTF - %s", TTF_GetError());
        close_viewport(viewport);
        return NULL;
    }
        
    return viewport;
}

void close_viewport(Viewport* viewport) {
    if (viewport != NULL) {
        if (viewport->tilesets.vehicles != NULL) SDL_DestroyTexture(viewport->tilesets.vehicles);
        if (viewport->tilesets.roads != NULL) SDL_DestroyTexture(viewport->tilesets.roads);
        if (viewport->tilesets.menu != NULL) SDL_DestroyTexture(viewport->tilesets.menu);
        if (viewport->renderer != NULL) SDL_DestroyRenderer(viewport->renderer);
        if (viewport->window != NULL) SDL_DestroyWindow(viewport->window);
        free(viewport);
    }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void event_loop(Viewport* viewport) {
    bool quit = false;
    bool collision = false;
    
    SDL_Event event;
    
    int lines = 2*LINES_PER_DIRECTION + 4;
    int side = viewport->height/lines + 1;
    
    while (!quit && !collision) {
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
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
                if (viewport->state == VIEWPORTSTATE_MENU) {
                    switch (event.key.keysym.sym) {
                    case SDLK_RETURN:
                        viewport->state = VIEWPORTSTATE_GAME;
                        break;
                    case SDLK_l:
                        viewport->state = VIEWPORTSTATE_IA;
                        FILE* file = fopen("learning.txt", "r");
                        if (file) {
                            viewport->level->player->q = load_matrix(file);
                            fclose(file);
                        }
                    }
                }
            }
        }
        
        if (viewport->state == VIEWPORTSTATE_IA) {
            Perception p = get_entity_perception(viewport->level, viewport->level->player);
            Action a = e_greedy(viewport->level->player->q, p, 0.);
            make_action(viewport->level, viewport->level->player, a);
            printf("Perception : %u, Action : %u\n", p, a);
        } else {
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
        case VIEWPORTSTATE_IA:
            collision = update_game(viewport->level);

            draw_road(viewport, lines, side);
            draw_cars(viewport, lines-4, side);
            break;
        case VIEWPORTSTATE_MENU:
            draw_menu(viewport);
            break;
        }

        SDL_RenderPresent(viewport->renderer);
        SDL_Delay(1000/FPS);
    }
}
