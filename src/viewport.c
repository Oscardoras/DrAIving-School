#include <SDL2/SDL.h>

#include "level.h"
#include "viewport.h"

#define WIDTH 800
#define HEIGHT 600

#define NB_LINES 3

Viewport* create_viewport(int width, int height, Level* level) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error SDL - %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_Window* window = SDL_CreateWindow("Jeu",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        SDL_Log("Error window - %s", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        SDL_Log("Error renderer - %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    Viewport* viewport = malloc(sizeof(Viewport));
    if (!viewport) {
        printf("Error malloc viewport.\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    else {
        viewport->width = width;
        viewport->height = height;
        viewport->window = window;
        viewport->renderer = renderer;
        viewport->level = level;
        viewport->animation_loop = 0;
        //tilesets et animations à initialiser !
    }
    
    return viewport;
}

void close_viewport(Viewport* viewport) {
    SDL_DestroyRenderer(viewport->renderer);
    SDL_DestroyWindow(viewport->window);
    //Tilesets et animations à libérer !
    free(viewport);
    
    SDL_Quit();
}

void draw_viewport(Viewport* viewport, int lines, int side, int pos) { //Voies autoroute horizontales
    SDL_SetRenderDrawColor(viewport->renderer, 0, 191, 255, 255);
    SDL_RenderClear(viewport->renderer);
    
    SDL_Rect rect_up, rect_down;
    rect_up.w = rect_down.w = rect_up.h = rect_down.h = side;
    
    
    for(int x=-pos; x<viewport->width; x += side) {
        rect_up.x = rect_down.x = x;
        for(int i=1; i<lines/2; i++) {
            rect_up.y = i*side;
            rect_down.y = (lines-1-i)*side;
        
            (i == 1) ?
                SDL_SetRenderDrawColor(viewport->renderer, 100, 75, 0, 255) :
                SDL_SetRenderDrawColor(viewport->renderer, 100, 100, 100, 255);
            
            SDL_RenderFillRect(viewport->renderer, &rect_up);
            SDL_RenderFillRect(viewport->renderer, &rect_down);
        }
    }
    
    
    SDL_RenderPresent(viewport->renderer);
}

void event_loop(Viewport* viewport) {
    bool quit = false;
    
    SDL_Event event;
    
    int lines = 2*NB_LINES + 4;
    int side = viewport->height / lines;
    int pos = 0;
    
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;
        }
        
        draw_viewport(viewport, lines, side, pos);
        
        pos = (pos+1) % side;
        
        SDL_Delay(10);
    }
}