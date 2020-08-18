#include <lcom/lcf.h>

#include <lcom/lab2.h>
#include <lcom/timer.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8254.h"

extern u32_t globalCounter; //counter located in timer.c, used in timer_test_int

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) //this function reads and displays the the configuration of the timer
{
  if (timer > 2) //verifies validaty of timer
  {
    return 1;
  }
  
  uint8_t st; //status will be saved in this variable

  if(timer_get_conf(timer, &st) != OK) //reads the status
  {
    return 1;
  }

  if(timer_display_conf(timer, st, field) != OK) //displays the configuration (or chosen part)
  {       
    return 1;
  }

  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) //this function alters the timer's frequency
{
  if (timer > 2) //verifies validaty of timer
  {
    return 1;
  }

  return timer_set_frequency(timer, freq); //calls the function and returns its return value
}

int(timer_test_int)(uint8_t time) //this function counts seconds using interrupts
{
  uint8_t bit_no = TIMER0_IRQ; //concedes the I\O with a unique, specific id. In this case, because it is the timer, we gave it the value of TIMER0_IRQ, since IRQ's are unique for each I\O

  if (timer_subscribe_int(&bit_no) != OK) //subscribes an interruption call for a I\O (from timer 0)
  {
    return 1;
  }

  int r; //receives the return value of driver_receive function
  int ipc_status; //receives notification of interruption
  message msg;
    
    while(globalCounter < (time*60)) //In 1 second there are 60 timer interrupts 
    { 
      // Get a request message.
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) 
        {
          printf("driver_receive failed with: %d", r);
          continue;
        }
      if (is_ipc_notify(ipc_status)) 
        { // received notification
          switch (_ENDPOINT_P(msg.m_source)) 
          {
            case HARDWARE: /* hardware interrupt notification */
              if (msg.m_notify.interrupts & BIT(bit_no)) 
              { /* subscribed interrupt */             
                  timer_int_handler(); //increments globalCounter

                if ((globalCounter % 60) == 0) //if a second has passed, globalCounter is a multiple of 60
                  {
                  timer_print_elapsed_time(); //so, it prints a message
                  }
              }
              break;
            default:
              break; // no other notifications expected: do nothing 
          }
        }  
      else
        { 
          /* received a standard message, not a notification
          no standard messages expected: do nothing */
        }
    }

  if (timer_unsubscribe_int() != OK) //ends subscription and disables interrupts
  { 
    return 1; 
  }

  return 0;
}

int(util_get_LSB)(uint16_t val, uint8_t *lsb) //returns 8 least significant bits of 16 bit variable val
{
  *lsb = (uint8_t) val;

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) //returns 8 most significant bits of 16 bit variable val
{
  *msb = (uint8_t) (val >> 8);

  return 0;
}
