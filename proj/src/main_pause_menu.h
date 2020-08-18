
/** @defgroup main_pause_menu main_pause_menu
 * @{
 *
 * Module that operates the menus of the game.
 */

#include "lcom/lcf.h"

#include "rtc.h"
#include "draw.h"
#include "kbc_handler.h"

enum window {MAIN_MENU, CHOOSE_P2_MODE, IN_GAME, PAUSE_MENU, EXIT};

/**
 * @brief State machine that moves the game between menus
 * 
 */
void window_state_handler();
/**
 * @brief Function that sets the game state to EXIT
 * 
 */
void End_Game();
/**
 * @brief Function that checks wether the game is in EXIT state
 * 
 * @return true Game is in EXIT state
 * @return false Game is not in EXIT state
 */
bool is_ExitState();
/**
 * @brief Function that sets the game state to the main menu
 * 
 */
void reinitialize_state();
/**
 * @brief Function that initializes the background
 * 
 */
void initialize_background();
/**
 * @brief Function that initializes the sprites used in the menus
 * 
 */
void initialize_menu_sprites();
/**
 * @brief Functions that configures the keyboard, graphics and rtc for use in the menus
 * 
 * @return int Returns 0 in case of success and 1 otherwise
 */
int enable_keyboard_graphics_rtc();
/**
 * @brief Functions that prevents events occurring in the menu from "leaking" into the game
 * 
 * @return int Returns 0 in case of success and 1 otherwise
 */
int disable_keyboard_graphics_rtc();
/**
 * @brief Function that prints a number on a specified location of the screen
 * 
 * @param i Number to print
 * @param x X coordinate for the placement
 * @param y Y coordinate for the placement
 */
void print_numbers(uint8_t i, uint16_t x, uint16_t y);
/**
 * @brief Function that prints the current date on the screen
 * 
 * @param day Number of the current day
 * @param month Number of the current month
 * @param year Number of the current year (last two digits)
 */
void print_date(uint8_t day, uint8_t month, uint8_t year);
/**
 * @brief Function that prints the current time on the screen
 * 
 * @param hour Number of the current hour
 * @param minute Number of the current minute
 */
void print_time(uint8_t hour, uint8_t minute);
/**
 * @brief Prints a single frame of the main menu
 * 
 * @param day Current day
 * @param month Current month
 * @param year Current year (last two digits)
 * @param minute Current minute
 * @param hour Current hour
 */
void print_frame(uint8_t day, uint8_t month, uint8_t year, uint8_t minute, uint8_t hour);
/**
 * @brief prints a single frame of the control scheme choice menu
 * 
 */
void print_p2_mode_frame();
/**
 * @brief Moves the cursor between buttons on menus
 * 
 * @param cursor_movement Value that defines where the cursor is moving towards
 * @return true Returned upon successful movement
 * @return false Returned upon unsuccessful movement
 */
bool cursor_move(uint8_t cursor_movement);
/**
 * @brief Main function used to operate menus
 * 
 * This function includes a driver_receive() loop and is responsible for handling the game while inside menus
 * 
 * @return int Returns 0 in case of success and 1 otherwise
 */
int Menu();

/** @} */
