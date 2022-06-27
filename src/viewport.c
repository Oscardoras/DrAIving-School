#include <SDL2/SDL.h>

#include "viewport.h"

#define WIDTH 800
#define HEIGHT 600

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
    free_level(viewport->level);
    SDL_DestroyRenderer(viewport->renderer);
    SDL_DestroyWindow(viewport->window);
    //Tilesets et animations à libérer !
    free(viewport);
    
    SDL_Quit();
}