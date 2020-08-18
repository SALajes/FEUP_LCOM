// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include "keyboard.h"
#include "i8042.h" //keyboard macros header file
#include "i8254.h" //timer macros header file

extern int sys_inb_counter;
extern uint32_t scan_code;
extern u32_t timer_global_counter; //timer

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (kbd_test_scan)(bool assembly)  //this function reads and displays the scan code of a keyboard key
{	
  uint8_t kbd_id = KBD_IRQ; //used to identify the interrupt susbcription
  uint8_t policy = IRQ_REENABLE|IRQ_EXCLUSIVE;

  kbd_subscribe_interrupt(policy, &kbd_id); //subscribe interrupts and disable interrupts for the keyboard driver

  int ipc_status, r; 
  uint8_t num_bytes = 1; // can be 1 or 2 according to the number of bytes expected to receive as make and break codes
  bool end_loop = false; 
  uint8_t scan_byte[2]; //array that will be used to store the bytes we want to print

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
          if (msg.m_notify.interrupts & KBD_IRQ_MASK) 
          { /* subscribed interrupt */
            if(assembly)
            {
              kbc_asm_ih(); //calls the read status (and code) function written in assembly 
            }
            else
            {
              kbc_ih(); //calls the read status (and code) function written in C
            }

            kbd_read_and_display(&num_bytes, scan_byte, &end_loop);
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

  kbd_unsubscribe_interrupt();//unsub interrupts and reenable them for the driver

  if(!assembly)
  {
    print_sys_inb_counter();
  }

  return 0;
}

int (kbd_test_poll)() //this function reads and displays scan codes but by polling method
{
  uint8_t num_bytes = 1; // can be 1 or 2 according to the number of bytes expected to receive as make and break codes
  uint8_t scan_byte[2]; //array that will be used to store the bytes we want to print
  uint32_t status;
  bool end_loop = false;

  do
  {
    sys_inb(KBD_STAT_REG, &status); //holds the status of the register
    status &= OBF_MASK; //does an AND bitwise operation so we can compare

    if(status == OBF_FULL) //if the buffer is full, there is something to read
    {
      kbc_ih(); 

      kbd_read_and_display(&num_bytes, scan_byte, &end_loop);
    }
    
    tickdelay(micros_to_ticks(DELAY_US)); //creates a delay 
  }
  while(!end_loop);

  print_sys_inb_counter();
  reenable_interrupts();
  
  return 0;
}

int (kbd_test_timed_scan)(uint8_t idle) 
{
  uint8_t kbd_id = KBD_IRQ; //used to identify the interrupt susbcription
  uint8_t policy = IRQ_REENABLE|IRQ_EXCLUSIVE;
  uint8_t timer_id = TIMER0_IRQ;

  kbd_subscribe_interrupt(policy, &kbd_id); //subscribe interrupts and disable interrupts for the keyboard driver

  timer_subscribe_int(&timer_id);

  int ipc_status, r;
  uint8_t num_bytes = 1;
  bool end_loop = false;
  uint8_t scan_byte[2];

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
          if (msg.m_notify.interrupts & KBD_IRQ_MASK) 
          { /* subscribed interrupt */
            
            timer_global_counter = 0;

            kbc_ih();

            kbd_read_and_display(&num_bytes, scan_byte, &end_loop);

          }
          else if (msg.m_notify.interrupts & TIMER0_IRQ_MASK) 
              { /* subscribed interrupt */             
                timer_int_handler();               

                if ((timer_global_counter / 60) == idle) //if a second has passed, globalCounter is a multiple of 60
                  {
                    end_loop = true; 
                  }
              }

          break;
        }
        default:
          break; /* no other notifications expected: do nothing */
      }

      if(end_loop)
      {
        break;
      }
    }  
    else 
    { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
  }

  kbd_unsubscribe_interrupt();//unsub interrupts and reenable them for the driver

  timer_unsubscribe_int();

  return 0;
}
