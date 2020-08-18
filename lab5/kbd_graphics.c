#include "kbd_graphics.h"

int end_function_byESC ()
{
  uint8_t kbd_id = KBD_IRQ; //used to identify the interrupt susbcription
  uint8_t policy = IRQ_REENABLE|IRQ_EXCLUSIVE;

  if(kbd_subscribe_interrupt(policy, &kbd_id) != SUCCESS) //subscribe interrupts and disable interrupts for the keyboard driver
  {
    return FAIL;
  }

  int ipc_status, r; 
  uint8_t num_bytes = 1; // can be 1 or 2 according to the number of bytes expected to receive as make and break codes
  bool end_loop = false; 
  uint8_t scan_byte[2]; //array that will be used to store the bytes we want to print
  const bool print_scanbyte = true;

  message msg;
  while( 1 ) 
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
          if (msg.m_notify.interrupts & BIT(kbd_id)) 
          { /* subscribed interrupt */
            kbc_ih(); //calls the read status (and code) function written in C

            kbd_read_and_display(&num_bytes, scan_byte, &end_loop, !print_scanbyte);
          }

          break;
        }
        default:
          break; /* no other notifications expected: do nothing */
      }

      if(end_loop)
      {
        break; //the user has pressed ESC key
      }
    }  
    else 
    { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
  }

  if(kbd_unsubscribe_interrupt() != SUCCESS)//unsub interrupts and reenable them for the driver
  {
    return FAIL;
  }

  return SUCCESS;
}
