#include <SDL2/SDL_image.h>

#include "viewport.h"

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
    
    viewport->window = SDL_CreateWindow("MarkovAnt",
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
    viewport->tilesets.vehicles = IMG_LoadTexture(viewport->renderer, "sprites/vehicles.png");
    viewport->tilesets.preview = IMG_LoadTexture(viewport->renderer, "sprites/preview_2.png");
    if (viewport->tilesets.roads == NULL || viewport->tilesets.vehicles == NULL || viewport->tilesets.preview == NULL) {
        SDL_Log("Error IMG");
        close_viewport(viewport);
        return NULL;
    }
    viewport->state = (ViewportState)(TITLE);
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
    SDL_Quit();
}


void draw_viewport(Viewport* viewport, int lines, int side, int pos) { //Voies autoroute horizontales
    SDL_SetRenderDrawColor(viewport->renderer, 0, 191, 255, 255);
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
        SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[6], &dest);
        
        dest.y = (lines-1)*side;
        SDL_RenderCopy(viewport->renderer, viewport->tilesets.roads, &road[7], &dest);
        
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
    
    
    SDL_RenderPresent(viewport->renderer);
}

void draw_viewportTitle(Viewport* viewport, int lines, int pos, int side)
{
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
    SDL_RenderCopyEx(viewport->renderer, viewport->tilesets.preview, &sprites[0], &dest, 0, NULL, SDL_FLIP_NONE); // red bulding

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
            switch(event.type)
            {
                case SDL_QUIT: quit = true;
                break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        viewport->width = event.window.data1;
                        viewport->height = event.window.data2;}
                break;
            }

        }
        switch(viewport->state)
        {
            case ((ViewportState)(GAME)):
                draw_viewport(viewport, lines, side, pos);
                pos = (pos+1) % side;
            break;
            case ((ViewportState)(TITLE)):
                draw_viewportTitle(viewport, lines, side, pos);
            break;
        }

        
        SDL_Delay(5);
    }
}