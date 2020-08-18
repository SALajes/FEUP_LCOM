#ifndef _MOUSE_H
#define _MOUSE_H

int mouse_beggining(uint8_t mouse_id); //makes the necessary arrangements to correctly use the P2 mouse

int mouse_ending(uint8_t mouse_id); //makes the adjustments so the mouse returns to it's original configuration

int mouse_enable(); //sets the configuration to stream mode and enables data reports

int mouse_disable(); //writes a command to disable data report

int mouse_subscribe_interrupt(uint8_t policy, uint8_t *io_id); //subscribes interrupts based on the id and policy requested

int mouse_unsubscribe_interrupt(); //this function unsubscribes the interruption calls of the global variable mouse_hook_id

int read_outbuffer(uint8_t *response); //reads the output buffer, testing it's validity

int inputbuffer_empty(); //tests if input buffer is empty

int write_Mouse_command(uint8_t command); //writes a mouse command to the kbd input buffer

bool mouse_interruption(struct packet *mouse_packet,uint8_t *current_byte); //handles and saves a byte in the packet and returns true if a packet has been filled completely

void mouse_parse_packet(struct packet *pp); //receives a packet which contains only the 3 bytes from the output buffer and divides the information throughout the variables of the struct

bool exit_gesture(struct packet *mouse_packet, uint8_t x_len, uint8_t tolerance); //receives a packet and additional arguments and processes them along with its own static variables to detect the specified gesture, returning true if the gesture has been completed and false otherwise

#endif
