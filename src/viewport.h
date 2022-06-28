#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "level.h"

#define WIDTH 800
#define HEIGHT 600
#define LINES_PER_DIR 3
#define ANIMATION_SIZE_MAX 1
#define BLOCK_TYPES 1
#define TEXTURE_COUNT 2
#define NB_LINES 3
#define FPS 60

typedef struct {
    SDL_Texture* tileset;
    SDL_Rect rectangles[ANIMATION_SIZE_MAX];
    unsigned int frames;
} Animation;

typedef enum {
    TITLE,
    GAME
} ViewportState;

typedef struct {
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
    Level* level;
    SDL_Window* window;
    SDL_Renderer* renderer;
    struct {
        SDL_Texture* roads;
        SDL_Texture* vehicles;
        SDL_Texture* preview;
    } tilesets;
    struct {
        Animation blocks[BLOCK_TYPES];
        Animation entities[ENTITY_TYPES];
    } animations;
    unsigned int animation_loop;
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
 * @brief Copies a texture from an animation to a rectangle.
 * 
 * @param viewport the viewport.
 * @param animation the animation.
 * @param rect the rectangle.
 */
void copy_texture(Viewport* viewport, Animation* animation, SDL_Rect* rect);

/**
 * @brief Draws the road.
 * 
 * @param viewport the viewport to draw.
 * @param lines the number of lines to draw (2 * lines_per_direction + 4 (the borders of the road)).
 * @param side the length of the side of a roadblock.
 * @param pos the position of the road (used to animate the player movement on the road).
 */
void draw_road(Viewport* viewport, int lines, int side, int pos);

/**
 * @brief Draws a car.
 * 
 * @param viewport the viewport to draw.
 * @param entity the car to draw.
 * @param lines_per_dir the number of lines on the road (both directions).
 * @param side the length of a roadblock.
 */
void draw_car(Viewport* viewport, Entity* entity, int road_lines, int side);

/**
 * @brief Draws all the cars on the level.
 * 
 * @param viewport the viewport to draw.
 * @param lines_per_dir the number of lines on the road (both directions).
 * @param side the length of a roadblock.
 */
void draw_cars(Viewport* viewport, int road_lines, int side);

/**
 * @brief Draws the viewport's title.
 * 
 * @param viewport the viewport to draw.
 */
void draw_viewportTitle(Viewport* viewport);



/**
 * @brief The display loop.
 * 
 * @param viewport the viewport to loop.
 */
void event_loop(Viewport* viewport);


#endif
