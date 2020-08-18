#pragma once

/** @defgroup objects objects
 * @{
 *
 * Constants and structs used to represent objects.
 */

#include<lcom/lcf.h>

//nyan cat shape
#include "nyan_cat.h"
#include "nyan_cat_invert.h"

//taco cat shape
#include "taco_cat.h"
#include "taco_cat_invert.h"

//background
#include "background1.h"
#include "background4.h"
#include "background5.h"

//AMMOs shape
#include "cookie.h"
#include "taco.h"
#include "bomb.h"
#include "explosion_small.h"
#include "explosion_medium.h"
#include "explosion_large.h"
#include "star.h"
#include "heart.h"

//numbers and characters
#include "numbers.h"
#include "numbers_2.h"

//menu sprites
#include "PLAY_button.h"
#include "EXIT_button.h"
#include "button_SELECTOR.h"
#include "logo.h"
#include "pause.h"
#include "p2_mode.h"
#include "keyboard_pic.h"
#include "mouse_pic.h"
#include "ITS_A_TIE.h"
#include "PLAYER_1_WINS.h"
#include "PLAYER_2_WINS.h"

//MACROS
#define CONFIRM           3   /**< Value that indicates confirmation in a menu */
#define CURSOR_MOVE_RIGHT 2   /**< Value that indicates movement to the right in a menu */
#define CURSOR_MOVE_LEFT  1   /**< Value that indicates movement to the left in a menu */
#define CURSOR_STOP       0   /**< Value that indicates end of movement in a menu */

#define LEFT    1             /**< Value that indicates left position in a menu */
#define RIGHT   2             /**< Value that indicates right position in a menu */

#define BULLET_HIT_POINTS    1  /**< Value of damage dealt by basic projectiles */
#define BOMB_HIT_POINTS      1  /**< Value of damage dealt by bombs */

#define POWER_UP    5           /**< idk man */


typedef enum {BACKGROUND, NYAN, TACO, BOMB, STAR, HEART} xpm_id;

/** @name Struct Object */
/**
 * @{
 * 
 *  Struct that represents a sprite 
 * 
 */
struct object
{
  xpm_id xpm;         /**< @brief Identifies the type of sprite */
  uint8_t * pixmap;   /**< @brief pointer to the first pixel of the sprite */
  xpm_image_t img;    /**< @brief Struct with information about the sprite */
  uint16_t pos_x;     /**< @brief x Coordinate of the sprite */
  uint16_t pos_y;     /**< @brief y coordinate of the sprite */
};
/** @} */

/** @name Struct Cursor */
/**
 * @{
 * 
 *  Struct that represents a cursor 
 * 
 */
struct cursor
{
  struct object cursor; /**< @brief Sprite of the cursor */
  uint8_t position;     /**< @brief Position of the cursor */
};
/** @} */

/** @name Struct Projectile */
/**
 * @{
 * 
 *  Struct that represents a projectile 
 * 
 */
struct projectile
{
  struct object projectile; /**< @brief Sprite of the projectile */
  uint8_t movement_units;   /**< @brief Speed of the projectile */
  int8_t h_mov;             /**< @brief Horizontal direction of movement of the projectile */ 
  int8_t v_mov;             /**< @brief Vertical direction of movement of the projectile */
  bool active;              /**< @brief Defines if the projectile is active */
  bool exploded;            /**< @brief Defines is the projectile has exploded (in case it's a bomb) */
};
/** @} */

/** @name Struct Player */
/**
 * @{
 * 
 *  Struct that represents a Player 
 * 
 */
struct player
{
  struct object player;     /**< @brief Sprite of the player */
  uint8_t movement_units;   /**< @brief Speed of the player */
  int8_t h_mov;             /**< @brief Horizontal direction of movement of the player */ 
  int8_t v_mov;             /**< @brief Vertical direction of movement of the player */
  bool inverted;            /**< @brief Defines if the player's sprite is inverted */
  int life_points;          /**< @brief Amount of health the player has */
  bool star;                /**< @brief Defines if the player is under the effect of the star */
};
/** @} */

/** @name Struct Explosion */
/**
 * @{
 * 
 *  Struct that represents the explosion animation 
 * 
 */
struct explosion
{
  xpm_id xpm;               /**< @brief Identifies the type of sprite */
  uint8_t * pixmap1;        /**< @brief pointer to the first pixel of the first sprite */
  xpm_image_t img1;         /**< @brief Struct with information about the first sprite */
  uint8_t * pixmap2;        /**< @brief pointer to the first pixel of the second sprite */
  xpm_image_t img2;         /**< @brief Struct with information about the second sprite */
  uint8_t * pixmap3;        /**< @brief pointer to the first pixel of the third sprite */
  xpm_image_t img3;         /**< @brief Struct with information about the third sprite */
};
/** @} */

/** @name Struct Special */
/**
 * @{
 * 
 *  Struct that represents a power up 
 * 
 */
struct special
{
  struct object object;     /**< @brief Sprite of the power up */
  bool active;              /**< @brief Defines if the projectile is active */
  struct player *player;    /**< @brief Pointer to the player under the effect of the power up */
};
/** @} */

/** @} */
