#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "game.h"

#define WIDTH 800
#define HEIGHT 600
#define LINES_PER_DIR 3
#define BLOCK_TYPES 1
#define TEXTURE_COUNT 2
#define NB_LINES 3
#define FPS 60


typedef enum {
    TITLE,
    GAME,
    GAMEIA
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
        SDL_Texture* preview;
    } tilesets;
    ViewportState state;
    TTF_Font* font;
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
