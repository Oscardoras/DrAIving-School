#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "game.h"

#define FPS 60


typedef enum {
    VIEWPORTSTATE_MENU,
    VIEWPORTSTATE_GAME,
    VIEWPORTSTATE_IA,
    VIEWPORTSTATE_VICTORY,
    VIEWPORTSTATE_DEFEAT
} ViewportState;

/**
 * @brief A viewport.
 * 
 */
typedef struct {
    int width;
    int height;
    Level* level;
    SDL_Window* window;
    SDL_Renderer* renderer;
    struct {
        SDL_Texture* roads;
        SDL_Texture* vehicles;
        SDL_Texture* menu;
    } tilesets;
    TTF_Font* font;
    ViewportState state;
    bool display_hitboxes;
} Viewport;


/**
 * @brief Creates a new viewport and its components.
 * 
 * @param width the width of the window.
 * @param height the height of the window.
 * @param level the level displayed in the window.
 * @return the created viewport.
 */
Viewport* create_viewport(int width, int height, Level* level);

/**
 * @brief Frees a viewport.
 * 
 * @param viewport the viewport to free.
 */
void close_viewport(Viewport* viewport);

/**
 * @brief The display loop.
 * 
 * @param viewport the viewport to loop.
 */
void event_loop(Viewport* viewport);


#endif
