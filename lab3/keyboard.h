#ifndef _KEYBOARD_H
#define _KEYBOARD_H


int kbd_subscribe_interrupt(uint8_t policy, uint8_t *io_id);

int kbd_unsubscribe_interrupt();

int kbd_read_and_display(uint8_t *num_bytes, uint8_t scan_byte[], bool *end_loop);

int reenable_interrupts();

int print_sys_inb_counter();

#endif
