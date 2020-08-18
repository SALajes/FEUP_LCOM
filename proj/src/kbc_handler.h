
/** @defgroup kbc_handler kbc_handler
 * @{
 *
 * Module that calls the appropriate functions and alters the appropriate values in the event of a key being pressed or the mouse's state changing.
 */

#include <lcom/lcf.h>

#include "keyboard.h"
#include "mouse.h"
#include "projectile_handler.h"


/**
 * @brief Processes a byte received from a keyboard interrupt
 * 
 * Alters the placement of the cursor in the menus and confirms options depending on the key that was pressed 
 * 
 * @param scan_byte Array that contains one or two bytes which identify the key that was released
 * @param player1 Pointer to player 1's struct, so as to alter values
 * @param player2 Pointer to player 2's struct, so as to alter values
 */
void menu_and_pause_kbc_handler(uint8_t scan_byte[], uint8_t *cursor_movement);

/**
 * @brief Processes a byte received from a keyboard interrupt 
 * 
 * Alters a player's direction of movement, vertical or horizontal, or shoots a projectile according to the key that was released and whether the mouse is active or not
 * 
 * @param scan_byte Array that contains one or two bytes which identify the key that was released
 * @param player1 Pointer to player 1's struct, so as to alter values
 * @param player2 Pointer to player 2's struct, so as to alter values
 * @return int Returns 0 in case of success and 1 otherwise
 */
int handling_scanbyte(uint8_t scan_byte[], struct player *player1, struct player* player2);

/**
 * @brief Processes a packet received from a mouse interrupt
 * 
 * Alters player 2'2 direction of movement, vertical and horizontal, and shoots a projectile
 * according to the mouse's movement and released buttons
 * 
 * @param mouse_packet Struct which contains the information received from the mouse
 * @param player Pointer to player 2's struct, so as to alter values
 * @return int Returns 0 in case of success and 1 otherwise
 */
int handling_mouse_packet(struct packet *mouse_packet, struct player *player);

/** @} */
