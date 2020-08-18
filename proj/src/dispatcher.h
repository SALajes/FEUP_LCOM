
/** @defgroup dispatcher dispatcher
 * @{
 *
 * Module that bridges the connection between all modules.
 */

#include <lcom/lcf.h>

#include "kbc_handler.h"
#include "player_handler.h"
#include "main_pause_menu.h"
#include "colisions.h"
#include "rtc.h"

extern uint32_t timer_global_counter;
extern uint32_t scan_code; //scan code of a keyboard key
extern bool mouse_active;
extern struct object background;
extern struct player player1;
extern struct player player2;
extern struct object p1_win;
extern struct object p2_win;
extern struct object tie;
extern struct special star;
extern struct special heart;

/**
 * @brief Function which contains the main loop of the program, connecting all the separate modules
 * 
 * @param player1 Pointer to player 1's struct
 * @param player2 Pointer to player 2's struct
 * @return int Returns SUCCESS (0) in case of success and FAIL(1) otherwise
 */
int play();

/**
 * @brief Function which contains the main loop of the program, connecting all the separate modules
 * 
 * @param player1 Pointer to player 1's struct
 * @param player2 Pointer to player 2's struct
 * 
 * @return int Returns SUCCESS (0) in case of success and FAIL (1) otherwise
 */
int pause_game(uint8_t *timer_id, uint8_t *mouse_id);

/**
 * @brief Function called by play() to handle the drawing functions of every sprite
 * 
 */
void draw();

/**
 * @brief Function used to show the multiple different end-game screens, depending on the outcome
 * 
 * @param game_state Current state of the game
 */
void End_Game_Screen(int game_state);

/**
 * @brief Function that subscribes interrupts that are necessary for the game to function correctly that were not yet subscribed (timer, mouse and rtc's alarms)
 * 
 * @param timer_id Pointer to timer's identification value
 * @param mouse_id Pointer to mouse's identification value
 * 
 * @return int Returns SUCCESS (0) in case of success and FAIL (1) otherwise
 */
int enable_necessary_interrupts(uint8_t *timer_id, uint8_t *mouse_id);

/**
 * @brief Function that unsubscribes interrupts that are necessary for the game to function but not for the menus (timer, mouse and rtc's alarms)
 * 
 * @param mouse_id Pointer to mouse's identification value
 * 
 * @return int Returns SUCCESS (0) in case of success and FAIL (1) otherwise
 */
int disable_necessary_interrupts(uint8_t *mouse_id);

/** @} */
