#ifndef _MOUSE_H
#define _MOUSE_H

/** @defgroup mouse mouse
 * @{
 *
 * Module that interfaces with the mouse.
 */

#include <lcom/lcf.h>

#include "i8042.h"

/**
 * @brief Configures the mouse and subscribes interrupts
 * 
 * @param mouse_id Value used to identify a mouse interrupt when received
 * @return int Returns 0 in case of success and 1 otherwise
 */
int mouse_beggining(uint8_t mouse_id); //makes the necessary arrangements to correctly use the P2 mouse

/**
 * @brief Unsubscribes interrupts and disables the mouse
 * 
 * @param mouse_id Value used to identify a mouse interrupt when received
 * @return int Returns 0 in case of success and 1 otherwise
 */
int mouse_ending(uint8_t mouse_id); //makes the adjustments so the mouse returns to it's original configuration

/**
 * @brief Enables streaming and data reporting from the mouse
 * 
 * @return int Returns 0 in case of success and 1 otherwise
 */
int mouse_enable(); //sets the configuration to stream mode and enables data reports

/**
 * @brief Disables data reporting from the mouse
 * 
 * @return int Returns 0 in case of success and 1 otherwise
 */
int mouse_disable(); //writes a command to disable data report

/**
 * @brief Subscribes interrupts from the mouse
 * 
 * @param policy Policy used to subscribe the interrupts
 * @param io_id Value used to identify a mouse interrupt when received
 * @return int Returns 0 in case of success and 1 otherwise
 */
int mouse_subscribe_interrupt(uint8_t policy, uint8_t *io_id); //subscribes interrupts based on the id and policy requested

/**
 * @brief Removes a previous subscription of mouse interrupts
 * 
 * @return int Returns 0 in case of success and 1 otherwise
 */
int mouse_unsubscribe_interrupt(); //this function unsubscribes the interruption calls of the global variable mouse_hook_id

/**
 * @brief Reads the output buffer and saves the information
 * 
 * @param response Value read from the output buffer
 * @return int Returns 0 in case of success and 1 otherwise
 */
int read_outbuffer(uint8_t *response); //reads the output buffer, testing it's validity

/**
 * @brief Checks if the input buffer is empty
 * 
 * @return int Returns 0 in case of success and 1 otherwise
 */
int inputbuffer_empty(); //tests if input buffer is empty

/**
 * @brief Writes a mouse command to the input buffer
 * 
 * @param command Command to be written
 * @return int Returns 0 in case of success and 1 otherwise
 */
int write_Mouse_command(uint8_t command); //writes a mouse command to the kbd input buffer

/**
 * @brief Handles a mouse interrupt depending on which of the 3 bytes of the packet is being received
 * 
 * @param mouse_packet Struct which stores information about a mouse packet
 * @param current_byte Identifies which of the 3 bytes of a packet is being processed
 * @return true Returned when a packet is complete
 * @return false Returned when a packet is still imcoplete
 */
bool mouse_interruption(struct packet *mouse_packet,uint8_t *current_byte); //handles and saves a byte in the packet and returns true if a packet has been filled completely

/**
 * @brief Divides the information of the packet packet througout the packet struct
 * 
 * @param pp Struct which stores information about a mouse packet
 */
void mouse_parse_packet(struct packet *pp);

/*
bool exit_gesture(struct packet *mouse_packet, uint8_t x_len, uint8_t tolerance);
*/

/** @} */

#endif
