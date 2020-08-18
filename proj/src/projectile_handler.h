
/** @defgroup projectile_handler projectile_handler
 * @{
 *
 * Module that handles projectile movement and generation.
 */

#include "graphics.h"
#include "draw.h"
#include "rtc.h"

extern unsigned h_res;
extern unsigned v_res;

/**
 * @brief Template for the cookie projectile
 * 
 */
static struct projectile cookieProjectile;
/**
 * @brief Template for the taco projectile
 * 
 */
static struct projectile tacoProjectile;
/**
 * @brief Template for the bomb projectile
 * 
 */
static struct projectile bombProjectile;
/**
 * @brief Struct containing the pixmaps wich make up the explosion animation 
 * 
 */
static struct explosion bombExplosionAnimation;

/**
 * @brief Array that contains all existing projectiles
 * 
 */
static struct projectile projectile_array[20];

/**
 * @brief Declaration of a star object (there is only one)
 * 
 */
struct special star;

/**
 * @brief Declaration of a heart object (there is only one)
 * 
 */
struct special heart;

/**
 * @brief Initializes the global projectile template structs
 * 
 */
void initialize_projectile_bases();
/**
 * @brief Prepares projectiles for the start of the game by inactivating all 
 * 
 */
void set_projectiles();
/**
 * @brief Moves and draws each active projectile in the global array
 * 
 */
void draw_projectiles();
/**
 * @brief Generates a moving projetile, if there is an available position
 * 
 * @param h_dir Horizontal movement direction of the projectile
 * @param v_dir Vertical movement direction of the projectile
 * @param x_pos x coordinate of the starting point of the projectile
 * @param y_pos y coordinate of the starting point of the projectile
 * @param projType Type of projectile being shot
 */
void shoot_projectile(int8_t h_dir, int8_t v_dir, uint16_t x_pos, uint16_t y_pos, xpm_id projType);
/**
 * @brief Generates a static projectile, if there is an available position
 * 
 * @param x_pos x coordinate of the starting point of the projectile
 * @param y_pos y coordinate of the starting point of the projectile
 */
void drop_bomb(uint16_t x_pos, uint16_t y_pos);

struct projectile * get_projectile(int index);
/**
 * @brief Upon it's call, this function sets a time for the next appearance of the star
 * 
 * @param time Value to be modified by this function
 */
void schedule_star_appearance(uint8_t *time);
/**
 * @brief Activates star appearance
 * 
 */
void activate_star();
/**
 * @brief Deactivates the star appearance
 * 
 */
void deactivate_star();
/**
 * @brief The star disappears when it is caught by one or two players, activates the atribute star of player's struct as to indicate this player has caught the star
 * 
* @param player Pointer to the player who caught the star
 */
void activate_powerUP(struct player *player);
/**
 * @brief The power goes away, so we have to modify the player and the star
 * 
 */
void deactivate_powerUP();
/**
 * @brief Upon it's call, this function sets a time for the next appearance of the heart
 * 
 * @param time Value to be modified by this function
 */
void schedule_heart_appearance(uint8_t *time);
/**
 * @brief Activates the appearance of object heart
 * 
 */
void activate_heart();
/**
 * @brief A player has caught the heart, so it disappears
 * 
 */
void deactivate_heart();

/** @} */
