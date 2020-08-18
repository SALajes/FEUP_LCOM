
/** @defgroup draw draw
 * @{
 *
 * Module that writes to the vram and possibilitates double buffering.
 */

#include <lcom/lcf.h>

#include "graphics.h"
#include "objects.h"

#define TACO_BAR   0x0588 /**< @brief Color value of player 1's health bar */
#define NYAN_BAR   0xE267 /**< @brief Color value of player 2's health bar */
#define GOLD_BAR   0xFEEE /**< @brief Color value of an invulnerable player's health bar */

/**
 * @brief Draws an sprite on the buffer
 * 
 * Copies a pixmap to the buffer on the specified coordinates, ignoring any bytes that represent a specific shade of green, which is considered transparent
 * 
 * @param obj Pointer to an object struct which contains the pointer to the pixmap of the sprite, its dimensions and the coordinates to start drawing
 * @param invert If value is set to true the sprite is drawn horizontally inverted
 */
void draw_sprite(struct object *obj);
/**
 * @brief Draws the life bars for both players
 * 
 * @param player1 Pointer to player 1's struct 
 * @param player2 Pointer to player 2's struct
 */
void draw_life_bars(struct player * player1, struct player *player2);
/**
 * @brief Copies the frame "built" on the buffer to the physical memory that is used to print on the screen
 * 
 */
void write_to_vram();

/** @} */
