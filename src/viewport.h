#ifndef __LEARNING_H__
#define __LEARNING_H__

#include "level.h"


typedef struct {
    SDL_Texture* tileset;
    SDL_Rect rectangles[ANIMATION_SIZE_MAX];
    unsigned int frames;
} Animation;

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
        SDL_Texture* blocks;
        SDL_Texture* entities;
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
 * @brief The display loop.
 * 
 * @param viewport the viewport to loop.
 */
void event_loop(Viewport* viewport);

/**
 * @brief Copies a texture from an animation to a rectangle.
 * 
 * @param viewport the viewport.
 * @param animation the animation.
 * @param rect the rectangle.
 */
void copy_texture(Viewport* viewport, Animation* animation, SDL_Rect* rect, Direction direction);

/**
 * @brief Draws a viewport.
 * 
 * @param viewport the viewport to draw.
 */
void draw_viewport(Viewport* viewport);


#endif
