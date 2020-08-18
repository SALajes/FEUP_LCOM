#ifndef _KEYBOARD_H
#define _KEYBOARD_H

/** @defgroup keyboard keyboard
 * @{
 *
 * Module that interfaces with the keyboard.
 */

#include <lcom/lcf.h>
#include <minix/sysutil.h>

#include "i8042.h"

/**
 * @brief Subscribes interrupts from the keyboard
 * 
 * 
 * @param policy Policy used to subscribe the interrupts
 * @param io_id Value used to identify a kbd interrupt when received
 * @return int Returns 0 in case of success and 1 otherwise
 */
int kbd_subscribe_interrupt(uint8_t policy, uint8_t *io_id);

/**
 * @brief Removes a previous subscription of keyboard interrupts and resets the keyboard 
 * 
 * @return int Returns 0 in case of success and 1 otherwise
 */
int kbd_unsubscribe_interrupt();

/**
 * @brief Reads a byte received from the keyboard
 * 
 * 
 * @param num_bytes Number of bytes received per interrupt, altered depending on the first byte
 * @param scan_byte Array that contains one or two bytes
 * @param end_loop Value that is set to true when the break code of the ESC key is received
 * @param print If true, the value of each byte is printed on the screen
 * @return int Returns 0 in case of success and 1 otherwise
 */
int kbd_read_and_display(uint8_t *num_bytes, uint8_t scan_byte[], bool *end_loop, bool print);

/**
 * @brief Reenables interrupts to the standard driver
 * 
 * @return int Returns 0 in case of success and 1 otherwise
 */
int reenable_interrupts();

/** @} */

#endif
