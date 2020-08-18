#ifndef _LCOM_LAB3MACROS_H_
#define _LCOM_LAB3MACROS_H_

/** @defgroup lab3macros lab3macros
 * @{
 *
 * Set of macros made to improve readability of the functions
*/

#include "i8254.h"

////////////////
///BIT MACROS///
////////////////

#define MSB_MASK        BIT(7)    /**< @brief mask to check the MSB */
#define MSB_SET       BIT(7)      /**< @brief symbolizes that MSB is set to 1 */

////////////////////////
///I/O port addresses///
////////////////////////

#define KBC_STAT_REG    0x64 /**< @brief Keyboard Status Register Port */
#define KBC_IN_BUFF_COM 0x64 /**< @brief Keyboard Input Buffer for the Commands port */
#define KBC_IN_BUFF_ARG 0x60 /**< @brief Keyboard Input Buffer for the Arguments port*/
#define KBC_OUT_BUFF    0x60 /**< @brief Keyboard Output BUffer */

//////////
///IRQs///
//////////

#define KBD_IRQ	        1    /**< @brief Keyboard IRQ line */

#define MOUSE_IRQ       12   /**< @brief Mouse IRQ line */

///////////////////////
///useful scan codes///
///////////////////////

#define LONG_SCANCODE1   0xE0     /**< @brief first byte of long scancodes */
#define LONG_SCANCODE2   0xE1     /**< @brief second byte of long scancodes */

#define ESC_BREAK_CODE  0x81      /**< @brief scancode for esc break */

#define ENTER_KEY_MAKE       0X1C /**< @brief scancode for enter make */

#define LEFT_ARROW_MAKE      0x4B /**< @brief scancode for left arrow make */
#define RIGHT_ARROW_MAKE     0x4D /**< @brief scancode for right arrow make */
#define UP_ARROW_MAKE        0x48 /**< @brief scancode for up arrow make */
#define DOWN_ARROW_MAKE      0x50 /**< @brief scancode for down arrow make */

#define LEFT_ARROW_BREAK     0xCB /**< @brief scancode for left arrow break */
#define RIGHT_ARROW_BREAK    0xCD /**< @brief scancode for right arrow break */
#define UP_ARROW_BREAK       0xC8 /**< @brief scancode for up arrow break */
#define DOWN_ARROW_BREAK     0xD0 /**< @brief scancode for down arrow break */

#define W_KEY_MAKE           0x11 /**< @brief scancode for 'w' make */
#define A_KEY_MAKE           0x1E /**< @brief scancode for 'a' make */
#define S_KEY_MAKE           0x1F /**< @brief scancode for 's' make */
#define D_KEY_MAKE           0x20 /**< @brief scancode for 'd' make */

#define W_KEY_BREAK          0x91 /**< @brief scancode for 'w' break */
#define A_KEY_BREAK          0x9E /**< @brief scancode for 'a' break */
#define S_KEY_BREAK          0x9F /**< @brief scancode for 's' break */
#define D_KEY_BREAK          0xA0 /**< @brief scancode for 'd' break */

#define G_KEY_BREAK          0xA2 /**< @brief scancode for 'g' break */
#define H_KEY_BREAK          0xA3 /**< @brief scancode for 'h' break */
#define DOT_KEY_BREAK        0xB4 /**< @brief scancode for '.' break */
#define HIPHEN_KEY_BREAK     0xB5 /**< @brief scancode for '-' break */

///////////////
//status bits//
///////////////

#define PARITY_BIT      BIT(7)    /**< @brief Parity bit is set*/
#define TIMEOUT_BIT     BIT(6)     /**< @brief Timeout bit is set*/

#define OBF_FULL        0x01      /**< @brief Output Buffer Is full*/

#define IBF_FULL        BIT(1)    /**< @brief Input Buffer Is full*/

/////////////////
//////MOUSE//////
/////////////////

#define ACK            0xFA         /**< @brief symbolizes a functions fail as a return 1 */
#define NACK           0xFE         /**< @brief symbolizes a functions fail as a return 1 */
#define M_ERROR        0xFC         /**< @brief symbolizes a functions fail as a return 1 */

#define VALID_PACKET   BIT(3)       /**< @brief uses BIT(3) as byte validation (to be used in the packet)*/

//mouse related kbc commands
#define KBC_READ_COMMAND 0x20       /**< @brief Port used to Read Command Byte,expects arguments*/
#define KBC_WRITE_COMMAND 0x60      /**< @brief Port used to Write Command Byte, expects arguments*/
#define INTERRUPT_ENABLE_MASK 0x47  /**< @brief Mask to enable mouse interrupts*/
#define DISABLE_MOUSE  0xA7         /**< @brief command to Disable Mouse */
#define ENABLE_MOUSE   0xA8         /**< @brief command to Enable Mouse */
#define WRITE_TO_MOUSE 0xD4         /**< @brief command to Write Byte to Mouse, expects arguments */

//arguments

#define ENABLE_REPORT  0xF4         /**< @brief command to Enable (Data Reporting) */
#define DISABLE_REPORT 0xF5         /**< @brief command to Disable (Data Reporting) */
#define REMOTE_MODE    0xF0         /**< @brief command to set Remote mode (send data on request only*/
#define READ_DATA      0xEB         /**< @brief command to Read Data (send data packet request)*/
#define SET_STREAM     0xEA         /**< @brief command to Set Stream Mode (send data on events)*/

/////////
//OTHER//
/////////

#define DELAY_US 20000             /**< @brief represents constant of 20ms (20000 micro seconds) */

#define SUCCESS         0          /**< @brief symbolizes a functions success as a return 0 */
#define FAIL            1          /**< @brief symbolizes a functions fail as a return 1 */

#define NUM_TRIES      5           /**< @brief symbolizes a limit number of tries */


/**@}*/

#endif /* _LCOM_LAB3MACROS */
