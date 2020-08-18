#pragma once

/** @defgroup colisions colisions
 * @{
 *
 * Module that deals with colisions.
 */

#include "objects.h"
#include "projectile_handler.h"
#include "player_handler.h"

extern unsigned h_res; /* Horizontal resolution in pixels */
extern unsigned v_res; /* Vertical resolution in pixels */ 

extern struct player player1;
extern struct player player2;
extern struct special star;
extern struct special heart;

#define COLISION    1

/**
 * @brief Verifies and deals with every possible colision in the game
 * 
 */
void verify_colisions();
/**
 * @brief If two objects' frames (rectangles) intersect, we will proceed to the next test (isObjectColision), otherwise, the colision is not possible at all
 * 
 * @param o1 First of the two objects we want to verify if there is a colision between
 * @param o2 Second of the two objects we want to verify if there is a colision between
 * 
 * @return bool Returns the result of isObjectColision() or !COLISION (false) if it didn't pass the first colision test
*/
bool isFrameColision(struct object* o1, struct object* o2);
/**
 * @brief Determines the colision's coordinates and a auxiliar buffer to be sent to the second test (drawColisionBuffer())
 * 
 * @param o1 First of the two objects we want to verify if there is a colision between
 * @param o2 Second of the two objects we want to verify if there is a colision between
 * 
 * @return bool Returns the result of drawColisionBuffer()
 */
bool isObjectColision(struct object* o1, struct object* o2);
/**
 * @brief It draws only one object at a time in the buffer, when drawing the secondvit verifies if the pixel we are drawing is already drawn with a visible color, if so, we have a colision
 * 
 * @param obj Object we want to draw in the secondary buffer
 * @param x0 Coordinate value in the x oriented axis were the frame intersection starts
 * @param x1 Coordinate value in the x oriented axis were the frame intersection ends
 * @param y0 Coordinate value in the y oriented axis were the frame intersection starts
 * @param y1 Coordinate value in the y oriented axis were the frame intersection ends
 * @param buffer Array of pixels that compose the auxiliar buffer
 * @param secondObject Boolean value that indicates if we are drawing the first or the second object
 * 
 * @return bool Returns COLISION if the two objects colided  and !COLISION otherwise
 */
bool drawColisionBuffer(struct object *obj, uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1,uint8_t buffer[], bool secondObject);

/** @} */
