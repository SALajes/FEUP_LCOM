#include "mouse.h"

uint8_t mouse_read_byte; //holds the value of the byte read in the output buffer so it can be written in the packet
int mouse_hook_id; //holds the value of the I/O's id, so it can be used, mainly, to subscribe and unsubscribe interruption

int mouse_beggining(uint8_t mouse_id)
{
  uint8_t policy = IRQ_REENABLE|IRQ_EXCLUSIVE; //this policy prevents Minix's default IH from stealing mouse packets

  if(mouse_enable() != SUCCESS) //changes configurations
  {
    return FAIL;
  }

  if(mouse_subscribe_interrupt(policy, &mouse_id) != SUCCESS) //subscribes mouse interrupts
  {
    return FAIL;
  }

  return SUCCESS;
}

int mouse_ending(uint8_t mouse_id)
{
  if(mouse_unsubscribe_interrupt(&mouse_id) != SUCCESS) //unsubscribes mouse interrupts
  {
    return FAIL;
  }

  if(mouse_disable() != SUCCESS) //disables the configurations that were made previously
  {
    return FAIL;
  }

  return SUCCESS;
}

int mouse_enable()
{ 
  if(write_Mouse_command(SET_STREAM) != SUCCESS) //writes a command to set stream mode
  {
    return FAIL;
  }

  if(write_Mouse_command(ENABLE_REPORT) != SUCCESS) //writes a command to enable data report
  {
    return FAIL;
  }

  return SUCCESS;
}

int mouse_disable() 
{
  if(write_Mouse_command(DISABLE_REPORT) != SUCCESS)
  {
    return FAIL;
  }
  else return SUCCESS;
}

int mouse_subscribe_interrupt(uint8_t policy, uint8_t *mouse_id) 
{
  mouse_hook_id = *mouse_id; //the global variable gets the same value as the one pointed by mouse_id 

  if(sys_irqsetpolicy(*mouse_id, policy, &mouse_hook_id) == OK)
  { 
    return 0; 
  }

  return 1;
}

int mouse_unsubscribe_interrupt() 
{
  if(sys_irqrmpolicy(&mouse_hook_id) != OK) //verifies validity of unsubscription
  {
    return 1;
  }

  return 0;
}

int read_outbuffer(uint8_t *response)
{
  uint32_t status;
  uint32_t response32;

  if(sys_inb(KBC_STAT_REG, &status) != SUCCESS) //reads the status from the register
  {
    return FAIL;
  }

  if(status & TIMEOUT_BIT || status & PARITY_BIT) //checks validity of the status(if bits 6 or 7 are set, the status is invalid
  {
    return FAIL;
  }

  if(status & OBF_FULL) //if the status is valid and the output buffer is full, we can read it
  {
    if(sys_inb(KBC_OUT_BUFF, &response32) != SUCCESS)  //reads response from the output buffer
    {
      return FAIL;
    }

    *response = (uint8_t) response32; //converts the response32 (32 bits) to the parameter *response of 8 bits

    return SUCCESS;
  }
  else return FAIL;
} 

int inputbuffer_empty()
{
  uint32_t status;

  if(sys_inb(KBC_STAT_REG, &status) != SUCCESS) //reads the status
  {
    return FAIL;
  }

  if(status & IBF_FULL) //tests if it is full
  {
    return FAIL; //if so, we can not write in it
  }

  return SUCCESS;
}

int write_Mouse_command(uint8_t command) 
{
  uint32_t response;
  int try_counter = 0; 

  do
  {
    if(inputbuffer_empty() != SUCCESS) //verifies if we can write in the input buffer (if it is empty)
    {
      return FAIL;
    }

    if(sys_outb(KBC_IN_BUFF_COM, WRITE_TO_MOUSE) != SUCCESS) //tells the kbc we want to write a mouse command
    {
      return FAIL;
    }

    if(inputbuffer_empty() != SUCCESS) //checks if we can write again
    {
      return FAIL;
    }

    if(sys_outb(KBC_IN_BUFF_ARG, command) != SUCCESS) //sends the command as an argument
    {
      return FAIL;
    }
    
    if(sys_inb(KBC_OUT_BUFF, &response) != SUCCESS) //receives the response has it is going to be a ACK, NACK or M_ERROR
    {
      return FAIL;
    }

    try_counter++;

  }while(response == NACK && try_counter <= NUM_TRIES); //we try to do this until it fails (we achieve a limit number of tries) or we receive a response different than NACK

  if(response == M_ERROR) //if the response is M_ERROR (not ACK), it fails
  {
    return FAIL;
  }

  return SUCCESS;
}

void (mouse_ih)()
{
  uint8_t code_received; //will receive the scan code requested
  int try_counter = 0;

  do
  {
    if(read_outbuffer(&code_received) == SUCCESS)
    {
      mouse_read_byte = code_received; //if valid, we use this code passing it to the global variable mouse_read_byte

      return;
    }  

    try_counter++;
  }
  while(try_counter < 5); //it fails if we dont successfully receive the code before we try a limit number of times

  return;
}

bool mouse_interruption(struct packet *mouse_packet,uint8_t *current_byte)
{
  mouse_ih(); //calls the interruption handler function

  const bool complete_packet = true; //true if a packet has been completed

  if(*current_byte == 1) //if we are working on the first byte of the packet
  {
    if(mouse_read_byte & VALID_PACKET) //if bit(3) is set, than it can be used as the first byte
    {
      mouse_packet->bytes[0] = mouse_read_byte;//we save it in the first position of the packet's array
      *current_byte += 1; //and increment the variable (move on the the second byte)
    }
    else{/* doesn't do anything */}
  }
  else if(*current_byte == 2)//if we are working on the second byte 
  {
    mouse_packet->bytes[1] = mouse_read_byte;//we save it in the second position of the packet's array
    *current_byte += 1;//and move on the the third (and last) byte
  }
  else if(*current_byte == 3) //we are on the third byte
  {
    mouse_packet->bytes[2] = mouse_read_byte; //we save it in the last position of the array, the packet is complete

    mouse_parse_packet(mouse_packet); //we give value to each atribute of the packet

    //mouse_print_packet(mouse_packet); //and finally print the packet

    *current_byte = 1; //and this variable takes the value 1, so the next byte can be used as the first byte of the new packet

    return complete_packet; //packet completed
  }

  return !complete_packet; //packet not completed yet
}

void mouse_parse_packet(struct packet *pp)
{
  pp->lb = pp->bytes[0] & BIT(0);

  pp->rb = pp->bytes[0] & BIT(1);

  pp->mb = pp->bytes[0] & BIT(2);

  //BIT(3) is verified in the OTHER function 

  if(pp->bytes[0] & BIT(4))
  {
    pp->delta_x = ((0xFF << 8) | pp->bytes[1]);
  }
  else
  {
    pp->delta_x = pp->bytes[1];
  }

  if(pp->bytes[0] & BIT(5))
  {
    pp->delta_y =  ((0xFF << 8) | pp->bytes[2]);
  }
  else
  {
    pp->delta_y = pp->bytes[2];
  }

  pp->x_ov = pp->bytes[0] & BIT(6);
  
  pp->y_ov = pp->bytes[0] & BIT(7);
}
/*
enum state_t{INIT, DRAWUP, MIDPOINT, DRAWDOWN, COMP};

bool exit_gesture(struct packet *mouse_packet, uint8_t x_len, uint8_t tolerance)
{
  static enum state_t curr_state = INIT;
  static int x_sum = 0;
  static int y_sum = 0;

  switch (curr_state)
  {
    case INIT:
    {
      if(mouse_packet->lb && !(mouse_packet->mb) && !(mouse_packet->rb))
      {
        curr_state = DRAWUP;
      }

      break;
    }
    case DRAWUP:
    {
      if(mouse_packet->mb || mouse_packet->rb)
      {
        curr_state = INIT;
        x_sum = 0;
        y_sum = 0;
      }
      else if(!(mouse_packet->lb))
      {
        if((y_sum >= x_sum) && (x_sum >= x_len))
        {
          curr_state = MIDPOINT;
          x_sum = 0;
          y_sum = 0;
        }
        else
        {
          curr_state = INIT;
          x_sum = 0;
          y_sum = 0;
          
        }
      }
      else
      {
        if((mouse_packet->delta_x < -1 * tolerance) || (mouse_packet->delta_y < -1 * tolerance))
        {
          curr_state = INIT;
          x_sum = 0;
          y_sum = 0;
        }
        else
        {
          x_sum += mouse_packet->delta_x;
          y_sum += mouse_packet->delta_y;
        }
      }

      break;
    }
    case MIDPOINT:
    {
      if((mouse_packet->delta_x < -1 * tolerance) || (mouse_packet->delta_y < -1 * tolerance) ||
      (mouse_packet->delta_x > tolerance) || (mouse_packet->delta_y > tolerance) || mouse_packet-> mb)
      {
        curr_state = INIT;
      }
      else if(mouse_packet->lb)
      {
        curr_state = DRAWUP;
      }
      else if(mouse_packet->rb)
      {
        curr_state = DRAWDOWN;
      }
      break;
    }
    case DRAWDOWN:
    {
      if(mouse_packet->mb || mouse_packet->lb)
      {
        curr_state = INIT;
        x_sum = 0;
        y_sum = 0;
      }
      else if(!(mouse_packet->rb))
      {
        if(((y_sum * -1) >= x_sum) && (x_sum >= x_len))
        {
          curr_state = COMP;
          x_sum = 0;
          y_sum = 0;
        }
        else
        {
          curr_state = INIT;
          x_sum = 0;
          y_sum = 0;
        }
      }
      else
      {
        if((mouse_packet->delta_x < -1 * tolerance) || (mouse_packet->delta_y > tolerance))
        {
          curr_state = INIT;
          x_sum = 0;
          y_sum = 0;
        }
        else
        {
          x_sum += mouse_packet->delta_x;
          y_sum += mouse_packet->delta_y;
        }
      }
      break;
    }
    case COMP:
    {
      return true;
      break;
    }
  }

  if(curr_state == COMP)
  {
    return true;
  }
  else
  {
  return false;
  }
}
*/

