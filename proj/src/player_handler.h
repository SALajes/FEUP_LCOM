
/** @defgroup player_handler player_handler
 * @{
 *
 * Module that handles player movement and health values.
 */

#include <lcom/lcf.h>

#include "objects.h"

#define MAX_HEALTH     500  /**< @brief Maximum health of a player */

#define GAME_NOT_OVER   0   /**< @brief Value that represents an ongoing game */
#define P1_WIN          1   /**< @brief Value that represents a victory for player 1 */
#define P2_WIN          2   /**< @brief Value that represents a victory for player 2 */
#define TIE             3   /**< @brief Value that represents a tie */

/**
 * @brief Initializes the values of player 1's struct
 * 
 */
void initializePlayer1();
/**
 * @brief Initializes the values of player 2's struct
 * 
 */
void initializePlayer2();
/**
 * @brief Prepares the players for the game by reseting them to the start conditions
 * 
 */
void set_players();
/**
 * @brief Checks whether the players should be inverted or not
 * 
 * @param player1 Pointer to player 1's struct
 * @param player2 Pointer to player 2's struct
 */
void checkInvertion(struct player *player1, struct player *player2);
/**
 * @brief Alters a player's position according to their movement direction
 * 
 * @param player Pointer to the player's struct
 */
void move_player(struct player *player);
/**
 * @brief Alters the player's direction of movement to 0 if the mouse is active
 * 
 * @param player Pointer to the player's struct
 */
void stop_mouse_player_mov(struct player *player);
/**
 * @brief Reduces the player's life points by a number of hit points if this player is not under the effect of a star
 * 
 * @param player Pointer to the player who took damage
 * @param hit_points number of points to be reduced in player's life points
 */
void reducePlayer_lifePoints(struct player *player, int hit_points);
/**
 * @brief  Function that checks the current state of the game and returns the corresponding value
 *  
 * @return int Value that indicates the state of the game
 */
int check_GameOver();
/**
 * @brief  Increses the life points of the player who caught the heart
 * 
 * @param player Pointer to player who caught the heart
 */
void increase_health(struct player *player);

/** @} */
