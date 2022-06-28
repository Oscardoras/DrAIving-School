#ifndef __DRAW_H__
#define __DRAW_H__

#include "viewport.h"


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


#endif
