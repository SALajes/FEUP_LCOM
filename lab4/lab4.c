// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "i8042.h"
#include "i8254.h"
#include "mouse.h"

extern uint8_t mouse_read_byte; //declared in mouse.c
extern uint32_t timer_global_counter; //declared in timer.c

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) 
{    
  uint8_t mouse_id = MOUSE_IRQ; //atribute a unique id to subscribe the mouse interrupts

  if(mouse_beggining(mouse_id) != SUCCESS) //makes the necessary arrangements to correctly use
  {
    return FAIL;
  }

  struct packet mouse_packet;
  uint8_t current_byte = 1; //variable containing the byte number to associate in the packet

  int ipc_status, r;
  message msg;

  while(cnt > 0)  //cicle ends when we have read the number of packets we wanted to
  { 
    
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
    {
      printf("driver_receive failed with: %d", r);
      continue; 
    }
    
    if (is_ipc_notify(ipc_status)) 
    { /* received notification */ 
      switch (_ENDPOINT_P(msg.m_source)) 
      {
        case HARDWARE: /* hardware interrupt notification */
        { 
          if (msg.m_notify.interrupts & BIT(mouse_id)) //an interrupt by the mouse if notified
          { 
            if(mouse_interruption(&mouse_packet, &current_byte)) //analize the interruption
            { //the next steps are taken if the condition above is true (a packet has been finished)

              current_byte = 1; //if a packet has been completed, this variable takes the value 1, so the next byte, if valid, can be used as the first byte of the new packet

              cnt--; //the number of packets to be read decrements
            }
          }

          break; //iterates to the next cicle
        }
        default:
          break; /* no other notifications expected: do nothing */
      }

    }  
    else 
    { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
  }

  if(mouse_ending(mouse_id) != SUCCESS) //makes the adjustments so the mouse returns to it's original configuration
  {
    return FAIL;
  }

  return SUCCESS;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) 
{
  if(write_Mouse_command(REMOTE_MODE) != SUCCESS) //activates remote mode
  {
    return FAIL;
  }

  struct packet mouse_packet;
  uint8_t current_byte = 1;
  bool new_packet = true; //identifies if we are working on a new packet

  while(cnt > 0) //cicle ends when we have read the number of packets we wanted to
  {
    if(new_packet)
    {
      if(write_Mouse_command(READ_DATA) != SUCCESS) //we only ask to read data once for each packet (at the beggining)
      {
        return FAIL;
      }

      new_packet = false; //we are going to start working on a new packet so this packet stops being "new"
    }

    if(mouse_interruption(&mouse_packet, &current_byte))
    {
      cnt--; //the number of packets to be read decrements
      new_packet = true; //move on to a new packet

      tickdelay(micros_to_ticks(period * 1000)); //if a packet was completed and printed, waits period ms until it searches for the next packet
    }
  }

  if(write_Mouse_command(SET_STREAM) != SUCCESS) //sets to stream mode
  {
    return FAIL;
  }

  if(write_Mouse_command(DISABLE_REPORT) != SUCCESS) //disables data report
  {
    return FAIL;
  }

  uint8_t command = minix_get_dflt_kbc_cmd_byte(); //receives the command to return the mouse as minix default

  if(sys_outb(KBC_IN_BUFF_COM, KBC_WRITE_COMMAND) != SUCCESS) //ask to write command to the kbc
  {
    return FAIL;
  }

  if(sys_outb(KBC_IN_BUFF_ARG, command) != SUCCESS) //sends the desired command as argument
  {
    return FAIL;
  }

  return SUCCESS;
}

int (mouse_test_async)(uint8_t idle_time) 
{  
  uint8_t mouse_id = MOUSE_IRQ; 
  uint8_t timer_id = TIMER0_IRQ; //atribute a unique id to subscribe the timer interrupts
  uint32_t frequency = sys_hz(); //read the frequency of the timer

  if(mouse_beggining(mouse_id) != SUCCESS)
  {
    return FAIL;
  }

  if(timer_subscribe_int(&timer_id) != SUCCESS) //subscribes timer interrupts
  {
    return FAIL;
  }


  struct packet mouse_packet;
  uint8_t current_byte = 1;

  int ipc_status, r;
  message msg;

  while((timer_global_counter / frequency) < idle_time) //it stops when the (idle_time) seconds have passed
  { 
    
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
    {
      printf("driver_receive failed with: %d", r);
      continue; 
    }
    
    if (is_ipc_notify(ipc_status)) 
    { /* received notification */ 
      switch (_ENDPOINT_P(msg.m_source)) 
      {
        case HARDWARE: /* hardware interrupt notification */
        { 
          if (msg.m_notify.interrupts & BIT(mouse_id)) //if the interrupt comes from the mouse
          { /* subscribed interrupt */
            if(mouse_interruption(&mouse_packet, &current_byte)) //analize it like in the test_packet function
            {
              current_byte = 1;
              timer_global_counter = 0; //however, when we finish a packet, the time that has passed is reset to zero
            }
          }
          else if (msg.m_notify.interrupts & BIT(timer_id)) //if the interrupt comes from the timer
          {
            timer_int_handler(); //we increment the timer_global_counter (keeps track of how much time has passed)
          }

          break; //finish the case, go to the next iteration
        }
        default:
          break; /* no other notifications expected: do nothing */
      }

    }  
    else 
    { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
  }

  if(timer_unsubscribe_int() != SUCCESS) //unsubscribe the timer's interrupt calls
  {
    return FAIL;
  }

  if(mouse_ending(mouse_id) != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) 
{
  uint8_t mouse_id = MOUSE_IRQ;

  if(mouse_beggining(mouse_id) != SUCCESS)
  {
    return FAIL;
  }

  struct packet mouse_packet;
  uint8_t current_byte = 1;
  bool end_loop = false;

  int ipc_status, r;
  message msg;
  while(true) 
  { /* You may want to use a different condition */
    
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
    {
      printf("driver_receive failed with: %d", r);
      continue; 
    }
    
    if (is_ipc_notify(ipc_status)) 
    { /* received notification */ 
      switch (_ENDPOINT_P(msg.m_source)) 
      {
        case HARDWARE: /* hardware interrupt notification */
        { 
          if (msg.m_notify.interrupts & BIT(mouse_id)) //a mouse interruption has been notified
          { 
            if(mouse_interruption(&mouse_packet, &current_byte)) //we analize it like in test_packet
            {

              if(exit_gesture(&mouse_packet, x_len, tolerance)) //if the gesture (invert V) was taken, we give permission to end the cicle
              {
                end_loop = true;
              }
              
              current_byte = 1; //if not, we keep reading the next packet
            }

          }

          break;
        }
        default:
          break; /* no other notifications expected: do nothing */
      }

    }  
    else 
    { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }

    if(end_loop) //if the gesture was taken we exit the cicle
    {
      break;
    }
  }

  if(mouse_ending(mouse_id) != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

