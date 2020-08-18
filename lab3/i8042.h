#ifndef _LCOM_LAB3MACROS_H_
#define _LCOM_LAB3MACROS_H_

/** @defgroup lab3macros lab3macros
 * @{
 *
 * Set of macros made to improve readability of the functions
*/

#define BIT(n) (0x01<<(n))

/* I/O port addresses */

#define KBD_STAT_REG    0x64 /**< @brief Keyboard Status Register Port */
#define KBD_IN_BUFF_COM 0x64 /**< @brief Keyboard Input Buffer for the Commands port */
#define KBD_IN_BUFF_ARG 0x60 /**< @brief Keyboard Input Buffer for the Arguments port*/
#define KBD_OUT_BUFF    0x60 /**< @brief Keyboard Output BUffer */

/* IRQ */

#define KBD_IRQ	        1    /**< @brief Keyboard IRQ line */
#define KBD_IRQ_MASK    BIT(KBD_IRQ) /** @brief mask to check for keyboard interrupts */

/* Most significant bit operation helpers */
#define MSB_MASK        BIT(7) /** @brief mask to check the MSB */
#define MSB_SET       BIT(7) /** @brief symbolizes that MSB is set to 1 */

/* useful scan codes */
#define LONG_SCANCODE1   0xE0 /** @brief first byte of long scancodes */
#define LONG_SCANCODE2   0xE1

#define ESC_BREAK_CODE  0x81 /** @brief scancode for esc break */

/* Testing the Output buffer*/
#define OBF_MASK        0x01 /**< @brief OBF tester*/
#define OBF_FULL        0x01 /**< @brief OBF Is full*/

/*Working with KBC configuration commands*/
#define INTERRUPT_ENABLE_MASK 0x01 /**< @brief mask used to enable interrupts*/
#define KBC_READ_COMMAND 0x20      /**< @brief Port used to read a command*/
#define KBC_WRITE_COMMAND 0x60     /**< @brief Port used to write a command*/

// time conditioning
#define DELAY_US 20000 

//validity of condition

#define SUCCESS         0
#define FAIL            1


/**@}*/

#endif /* _LCOM_LAB3MACROS */
