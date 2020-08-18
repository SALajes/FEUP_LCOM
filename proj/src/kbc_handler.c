
#include "kbc_handler.h"


extern uint32_t timer_global_counter;
extern bool mouse_active;

void menu_and_pause_kbc_handler(uint8_t scan_byte[], uint8_t *cursor_movement)
{
  if(scan_byte[0] == LONG_SCANCODE1)
  {      
    switch (scan_byte[1])
    {
      case LEFT_ARROW_MAKE:  //left arrow make
      {
        *cursor_movement = CURSOR_MOVE_LEFT;
        break;
      }
      case RIGHT_ARROW_MAKE:      //right arrow make
      {
        *cursor_movement = CURSOR_MOVE_RIGHT;
        break;
      }
      default:
      {
        *cursor_movement = CURSOR_STOP;
        break;
      }
    }
  }
  else if(scan_byte[0] == ENTER_KEY_MAKE)
  {
    *cursor_movement = CONFIRM;
  }
  else
  {
    *cursor_movement = CURSOR_STOP;
  }
}

int handling_scanbyte(uint8_t scan_byte[], struct player *player1, struct player* player2)
{
  switch(scan_byte[0])
  {
    case A_KEY_MAKE:      //a make
    {
      player1->h_mov = -1; 
      break;
    }
    case W_KEY_MAKE:      //w make
    {
      player1->v_mov = -1;
      break;
    }
    case S_KEY_MAKE:      //s make
    { player1->v_mov = 1;
      break;
    }
    case D_KEY_MAKE:      //d make
    { player1->h_mov = 1;
      break;
    }
    case A_KEY_BREAK:      //a break
    case D_KEY_BREAK:      //d break
    {
      player1->h_mov = 0;
      break;
    }
    case S_KEY_BREAK:      //s break
    case W_KEY_BREAK:      //w break
    {
      player1->v_mov = 0;
      break;
    }
    case G_KEY_BREAK:      //g break
    {
      if(player1->inverted)
      {
        if(player1->player.pos_x >= 50)
        shoot_projectile(-1, player1->v_mov, player1->player.pos_x - 50, player1->player.pos_y + 25, player1->player.xpm);  
      }
      else
      {
        if(player1->player.pos_x + 250 < 1024)
        shoot_projectile(1, player1->v_mov, player1->player.pos_x + 250, player1->player.pos_y + 25, player1->player.xpm);
      }
      break;
    }
    case H_KEY_BREAK:      //h break
    {
      if(player1->inverted)
        {
          if(player1->player.pos_x + 250 + 45 < 1024)
          drop_bomb(player1->player.pos_x + 250, player1->player.pos_y + 25);
        }
        else
        {
          if(player1->player.pos_x >= 45)
          drop_bomb(player1->player.pos_x - 45, player1->player.pos_y + 25);
        }
      break;
    }
    case DOT_KEY_BREAK:      //. break
    {
      if(!mouse_active)
      {
        if(player2->inverted)
        {
          if(player2->player.pos_x + 250 < 1024)
          shoot_projectile(1, player2->v_mov, player2->player.pos_x + 250, player2->player.pos_y + 25, player2->player.xpm);
        }
        else
        {
          if(player2->player.pos_x >= 60)
          shoot_projectile(-1, player2->v_mov, player2->player.pos_x - 60, player2->player.pos_y + 25, player2->player.xpm);
        }
      }
      break;
    }
    case HIPHEN_KEY_BREAK:      //- break
    {
      if(!mouse_active)
      {
        if(player2->inverted)
        {
          if(player2->player.pos_x >= 45)
          drop_bomb(player2->player.pos_x - 45, player2->player.pos_y + 25);
        }
        else
        {
          if(player2->player.pos_x + 250 + 45 < 1024)
          drop_bomb(player2->player.pos_x + 250, player2->player.pos_y + 25);
        }
      }
      break;
    }
    case LONG_SCANCODE1:
    {
      if(!mouse_active)
      {
        switch (scan_byte[1])
        {
          case LEFT_ARROW_MAKE:  //left arrow make
          {
            player2->h_mov = -1;
            break;
          }
          case UP_ARROW_MAKE:      //up arrow make
          {
            player2->v_mov = -1;
            break;
          }
          case DOWN_ARROW_MAKE:      //down arrow make
          { 
            player2->v_mov = 1;
            break;
          }
          case RIGHT_ARROW_MAKE:      //right arrow make
          {
            player2->h_mov = 1;
            break;
          }
          case LEFT_ARROW_BREAK:      //left arrow break
          case RIGHT_ARROW_BREAK:      //right arrow break
          {
            player2->h_mov = 0;
            break;
          }
          case UP_ARROW_BREAK:      //up arrow break
          case DOWN_ARROW_BREAK:      //down arrow break
          {
            player2->v_mov = 0;
            break;
          }
          default:
          {break;}
        }
      }
    }
    default:
    {break;}
  }
  return SUCCESS;
}

int handling_mouse_packet(struct packet *mouse_packet, struct player *player)
{
  static bool previousLB = false;
  static bool previousRB = false;
  
  if(mouse_packet->delta_x < 0)
  {
    player->h_mov = -1;
  }
  else if(mouse_packet->delta_x > 0)
  {
    player->h_mov = 1;
  }

  if(mouse_packet->delta_y < 0)
  {
    player->v_mov = 1;
  }
  else if(mouse_packet->delta_y > 0)
  {
    player->v_mov = -1;
  }

  if(previousLB && !mouse_packet->lb)
  {
    if(player->inverted)
        {
          if(player->player.pos_x + 250 < 1024)
          shoot_projectile(1, player->v_mov, player->player.pos_x + 250, player->player.pos_y + 25, player->player.xpm);
        }
        else
        {
          if(player->player.pos_x >= 60)
          shoot_projectile(-1, player->v_mov, player->player.pos_x - 60, player->player.pos_y + 25, player->player.xpm);
        }
  }
  previousLB = mouse_packet->lb;
  if(previousRB && !mouse_packet->rb)
  {
    if(player->inverted)
        {
          if(player->player.pos_x >= 45)
          drop_bomb(player->player.pos_x - 45, player->player.pos_y + 25);
        }
        else
        {
          if(player->player.pos_x + 250 + 45 < 1024)
          drop_bomb(player->player.pos_x + 250, player->player.pos_y + 25);
        }
  }
  previousRB = mouse_packet->rb;
  return SUCCESS;
}
