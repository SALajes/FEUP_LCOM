#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

static int io_interrupt_id; //preserves the I\O's id, it's declared globally so it can be used by all functions, specially the one's that have no parameters 
u32_t timer_global_counter = 0; //counter used in the cycle of timer_test_int (lab2.c)

int (timer_set_frequency)(uint8_t timer, uint32_t freq) //alters the frequency of the chosen timer
{
  uint8_t status;

  if( timer_get_conf(timer, &status) != OK ) //fetches the status of the chosen timer
  {
    return 1; //function failed
  }

  uint32_t control_word = status & CLEAN_4MSB_UINT8; //extends the status value to 32 bits

  control_word |= TIMER_SELECT(timer); //sets the desired timer

  control_word |= TIMER_LSB_MSB; //sets initialization mode to MSB after LSB

  if (sys_outb(TIMER_CTRL, control_word) != OK) //inserts the control word 
  {
    return 1; //failed to insert
  }

  uint16_t divisor = TIMER_FREQ/freq; //it defines how much we have to multiply the TIMER_FREQ for, to obtain the frequency we want the timer to run to

  uint8_t lsb, msb;

  if(util_get_LSB(divisor, &lsb) != OK) //lsb keeps the 8 least significant bits of variable divisor
  {
    return 1;
  }

  if(util_get_MSB(divisor, &msb) != OK) //msb keeps the 8 most significant bits of variable divisor
  {
    return 1;
  }

  //convert a 8 bit variable into a 32 bit variable because the functions sys_outb and sys_inb's parameters type is 32 bit
  uint32_t lsb_32 = lsb; 
  uint32_t msb_32 = msb;

  //as specified in the control word, we want to send first the lsb and then the msb of the divisor
  if(sys_outb(TIMER_0 + timer, lsb_32) != OK) 
  {
    return 1;
  }

  if(sys_outb(TIMER_0 + timer, msb_32) != OK)
  {
    return 1;
  }

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) //this function subscribes interruption calls to an I/O with name bit_no
{
  io_interrupt_id = *bit_no;

  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &io_interrupt_id) == OK)
  { 
    return 0; 
  }

  return 1;
}

int (timer_unsubscribe_int)() //this function unsubscribes the interruption calls requested for the I/O identified by io_interrupt_id
{
  if(sys_irqrmpolicy(&io_interrupt_id) != OK)
  {
    return 1;
  }

  return 0;
}

void (timer_int_handler)() //increments the timer_global_counter
{
  timer_global_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) //this function saves the selected timer's configuration in the position pointed by *st
{
  uint8_t conf = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer); //creates a control word calling read back command, count mode and selected timer

  if (sys_outb(TIMER_CTRL, conf) != OK) // asks the system for the status of the chosen timer
  {
    return 1;
  }

  uint32_t status;

  if (sys_inb(TIMER_0 + timer, &status) != OK) //receives the status 4 bytes of the chosen timer, although we only want the 8 LSB
  {
    return 1;
  }

  *st = (uint8_t) status; //get the 8 least significant bytes

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,enum timer_status_field field) //this function displays the configuration or part of it
{
  int valid = 1;
  union timer_status_field_val val; //variable where the value to be shown is written in according to the field

  switch(field)
  {
    case all:
    {
      val.byte = st;  

      valid = timer_print_config(timer, field, val); //displays the whole status byte

      break;
    }
    case initial:
    {
      enum timer_init in_mode;//initialization mode, depends on bits 5,4 of the status byte
      uint8_t in_mode_mask = BIT(5) | BIT(4); //bit mask used to select bits 5 and 4
      uint8_t in_mode_byte = in_mode_mask & st; 

      //it is now going to be evaluated accordingly
      if (in_mode_byte == TIMER_LSB) 
      {
        in_mode = LSB_only;
      }
      else if (in_mode_byte == TIMER_MSB)
      {
        in_mode = MSB_only;
      }
      else if (in_mode_byte == TIMER_LSB_MSB)
      {
        in_mode = MSB_after_LSB;
      }
      else 
      {
        in_mode = INVAL_val;  //by default, the initialization mode is invalid value
      }

      val.in_mode = in_mode;

      valid = timer_print_config(timer, field, val); 

      break;
    }
    case mode:
    {
      uint8_t timer_mode_mask = BIT(3) | BIT(2) | BIT(1); //selects bits correspondant to count mode
      uint8_t count_mode = (timer_mode_mask & st) >> 1; //we want to evaluate the number written in those 3 bits but bit 0 is not part of it, so we get "rid" of it, per say

      if (count_mode == 6) //if count mode is 6 we make it mode 2 (rate generator)
      { 
        count_mode = TIMER_RATE_GEN;
      }

      if (count_mode == 7) //if count mode is 7 we make it mode 3 (square wave generator)
      { 
        count_mode = TIMER_SQR_WAVE;
      }

      val.count_mode = count_mode;

      valid = timer_print_config(timer, field, val);
      
      break;
    }
    case base:
    {
      bool bcd;//counting base, true if BCD, depends on bit 0 of the status byte

      if ((TIMER_COUTING_MODE_BIT & st) != 0) //selects bit 0 of the status and checks if it's 0 or 1
      {
        bcd = TIMER_BCD;
      }
      else 
      {
        bcd = TIMER_BIN; 
      }

      val.bcd = bcd;

      valid = timer_print_config(timer, field, val);

      break;
    }
  }

  return valid;
}
