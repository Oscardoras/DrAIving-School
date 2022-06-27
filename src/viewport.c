#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
        
        viewport->tilesets.road = IMG_LoadTexture(viewport->renderer, "sprites/sprites_road.png");
        viewport->tilesets.vehicles = IMG_LoadTexture(viewport->renderer, "sprites/vehicles.png");
        
        if (!viewport->tilesets.road | !viewport->tilesets.vehicles) {
            SDL_Log("Error Texture init - %s", SDL_GetError());
            
            if (!viewport->tilesets.road) SDL_DestroyTexture(viewport->tilesets.road);
            else SDL_DestroyTexture(viewport->tilesets.vehicles);
            
            SDL_DestroyRenderer(viewport->renderer);
            SDL_DestroyWindow(viewport->window);
            SDL_Quit();
            
            free(viewport);
            exit(EXIT_FAILURE);
        }
        
        /*
        for(unsigned int it = 0; it < TEXTURE_COUNT; ++it)
	    {
		    printf("%s", TEXTURE_NAMES[it]);
		    textures[it] = IMG_LoadTexture(renderer,TEXTURE_NAMES[it]);
		    if(!textures[it])
		    {
			    SDL_Log("Erreur creation texture");
			    exit(EXIT_FAILURE);
		    }
	    }
        */

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

void draw_rect(Viewport* viewport, int x, int y1, int y2, int side, int lines, int type) {
    SDL_Rect rect1, rect2, rect3;
    
    if (type == 1) {
        rect1.x = x;
        rect1.y = y1;
        rect1.w = rect1.h = side;
        rect2.x = x + 0.1*side;
        rect2.y = y1 + 0.1*side;
        rect2.w = rect2.h = 0.8*side;
        SDL_SetRenderDrawColor(viewport->renderer, 100, 75, 0, 255);
        SDL_RenderFillRect(viewport->renderer, &rect1);
        rect1.y = y2;
        SDL_RenderFillRect(viewport->renderer, &rect1);
        SDL_SetRenderDrawColor(viewport->renderer, 67, 50, 0, 255);
        SDL_RenderFillRect(viewport->renderer, &rect2);
        rect2.y = y2 + 0.1*side;
        SDL_RenderFillRect(viewport->renderer, &rect2);
    }
    else {
        if (type == 2) {
            rect1.x = x;
            rect1.y = y1;
            rect1.h = rect1.w = side;
            rect2.x = x;
            rect2.y = y1 + 0.1*side;
            rect2.w = side,
            rect2.h = 0.8*side;
            rect3.x = x + 0.2*side;
            rect3.y = y1 + 0.9*side;
            rect3.w = 0.6*side;
            rect3.h = 0.1*side;
            SDL_SetRenderDrawColor(viewport->renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(viewport->renderer, &rect1);
            rect1.y = y2;
            SDL_RenderFillRect(viewport->renderer, &rect1);
            SDL_SetRenderDrawColor(viewport->renderer, 75, 75, 75, 255);
            SDL_RenderFillRect(viewport->renderer, &rect2);
            SDL_RenderFillRect(viewport->renderer, &rect3);
            rect2.y = y2 + 0.1*side;
            rect3.y = y2;
            SDL_RenderFillRect(viewport->renderer, &rect2);
            SDL_RenderFillRect(viewport->renderer, &rect3);
        }
        else {
            if (type == (lines/2 - 1)) {
                rect1.x = x;
                rect1.y = y1;
                rect1.h = rect1.w = side;
                rect2.x = x;
                rect2.y = y1 + 0.1*side;
                rect2.w = side,
                rect2.h = 0.8*side;
                rect3.x = x + 0.2*side;
                rect3.y = y1;
                rect3.w = 0.6*side;
                rect3.h = 0.1*side;
                SDL_SetRenderDrawColor(viewport->renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(viewport->renderer, &rect1);
                rect1.y = y2;
                SDL_RenderFillRect(viewport->renderer, &rect1);
                SDL_SetRenderDrawColor(viewport->renderer, 75, 75, 75, 255);
                SDL_RenderFillRect(viewport->renderer, &rect2);
                SDL_RenderFillRect(viewport->renderer, &rect3);
                rect2.y = y2 + 0.1*side;
                rect3.y = y2 + 0.9*side;
                SDL_RenderFillRect(viewport->renderer, &rect2);
                SDL_RenderFillRect(viewport->renderer, &rect3);
            }
            else {
                rect1.x = x;
                rect1.y = y1;
                rect1.h = rect1.w = side;
                rect2.x = x;
                rect2.y = y1 + 0.1*side;
                rect2.w = side,
                rect2.h = 0.8*side;
                rect3.x = x + 0.2*side;
                rect3.y = y1;
                rect3.w = 0.6*side;
                rect3.h = side;
                SDL_SetRenderDrawColor(viewport->renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(viewport->renderer, &rect1);
                rect1.y = y2;
                SDL_RenderFillRect(viewport->renderer, &rect1);
                SDL_SetRenderDrawColor(viewport->renderer, 75, 75, 75, 255);
                SDL_RenderFillRect(viewport->renderer, &rect2);
                SDL_RenderFillRect(viewport->renderer, &rect3);
                rect2.y = y2 + 0.1*side;
                rect3.y = y2;
                SDL_RenderFillRect(viewport->renderer, &rect2);
                SDL_RenderFillRect(viewport->renderer, &rect3);
            }
        }
    }
}


void draw_viewport(Viewport* viewport, int lines, int side, int pos) { //Voies autoroute horizontales
    SDL_SetRenderDrawColor(viewport->renderer, 0, 191, 255, 255);
    SDL_RenderClear(viewport->renderer);
    
    int x, y1, y2;
    
    for(x=-pos; x<viewport->width; x += side) {
        for(int i=1; i<lines/2; i++) {
            y1 = i*side;
            y2 = (lines-1-i)*side;
            draw_rect(viewport, x, y1, y2, side, lines, i);
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
        
        SDL_Delay(5);
    }
}