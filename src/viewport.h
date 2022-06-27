#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include <SDL2/SDL.h>

#include "level.h"

#define ANIMATION_SIZE_MAX 1
#define BLOCK_TYPES 1
#define ENTITY_TYPES 1
#define TEXTURE_COUNT 2

typedef struct {
    SDL_Texture* tileset;
    SDL_Rect rectangles[ANIMATION_SIZE_MAX];
    unsigned int frames;
} Animation;

/*
char* TEXTURE_NAMES[] = {
"sprites/outdoors.png", 
"sprites/vehicles.png"
};
*/


// SDL_Texture* textures[TEXTURE_COUNT];
typedef struct
{
	unsigned int count;
	SDL_Rect rects[10];
	unsigned int sizex;
	unsigned int sizey;
	unsigned int textureid;
} sprite;

/**
 * @brief A viewport.
 * 
 */
typedef struct {
    int width;
    int height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Level* level;
    struct {
        SDL_Texture* road;
        SDL_Texture* vehicles;
    } tilesets;
    struct {
        Animation blocks[BLOCK_TYPES];
        Animation *entities[ENTITY_TYPES];
    } animations;
    unsigned int animation_loop;
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
 * @brief Copies a texture from an animation to a rectangle.
 * 
 * @param viewport the viewport.
 * @param animation the animation.
 * @param rect the rectangle.
 */
void copy_texture(Viewport* viewport, Animation* animation, SDL_Rect* rect);

/**
 * @brief Draws a viewport.
 * 
 * @param viewport the viewport to draw.
 */
void draw_viewport(Viewport* viewport, int lines, int pos, int side);

/**
 * @brief The display loop.
 * 
 * @param viewport the viewport to loop.
 */
void event_loop(Viewport* viewport);

#endif
