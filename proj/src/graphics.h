
/** @defgroup graphics graphics
 * @{
 *
 * Module that interfaces with the video card.
 */

#include <lcom/lcf.h>
#include "i8042.h"
#include "vbe_macros.h"

/**
 * @brief Fetches information about a specific display mode
 * 
 * 
 * 
 * @param mode Mode whose information we want to obtain
 * @param vmi_p Pointer to a struct that stores all the necessary information of a display mode
 * @return int Returns 0 in case of success and 1 otherwise
 */
int vbe_get_mode (uint16_t mode, vbe_mode_info_t* vmi_p);
/**
 * @brief Unused function to test a graphical mode
 * 
 * @return int Returns 0 in case of success and 1 otherwise
 */
int video_init();
/**
 * @brief Unused function to return to MINIX's standard text mode
 * 
 * @return int Returns 0 in case of success and 1 otherwise
 */
int returnTextMode();
/**
 * @brief Debugging function that prints the resolution of a graphical mode
 * 
 */
void printResolutions();

/** @} */
