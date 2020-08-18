#include "dispatcher.h"


int play() /*, struct object* p2*/
{
  uint8_t timer_id = TIMER0_IRQ; 
  uint8_t kbd_id = KBD_IRQ; //used to identify the interrupt susbcription
  uint8_t mouse_id = MOUSE_IRQ;
  uint8_t rtc_id = RTC_IRQ;

  if(disable_periodic() != SUCCESS)
  {
    return FAIL;
  }

  if(disable_update() != SUCCESS)
  {
    return FAIL;
  }

  if(enable_necessary_interrupts(&timer_id, &mouse_id) != SUCCESS) 
  {
    return FAIL;
  }

  struct packet mouse_packet;
  uint8_t current_byte = 1;
  int ipc_status, r; 
  uint8_t num_bytes = 1; // can be 1 or 2 according to the number of bytes expected to receive as make and break codes
  uint8_t scan_byte[2]; //array that will be used to store the bytes we want to print
  const bool print_scanbyte = true;
  bool kbdRec = false, mouseRec = false, timerRec = false, rtcRec = false;  //indicates if there is a new interruption from the kbd so as to handle the new scancode (equivalent for mouse and timer)
  bool pause_request = false;
  uint8_t time_4special;
  int heart_alarm;

  int game_state = GAME_NOT_OVER;
  
  schedule_star_appearance(&time_4special);
  set_alarm(time_4special);

  schedule_heart_appearance(&time_4special);
  heart_alarm = time_4special * 60;

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
            kbdRec = true;
          }
          else
          {
            kbdRec = false;
          }
          if ((msg.m_notify.interrupts & BIT(mouse_id)) && mouse_active) //a mouse interruption has been notified
          { 
            if(mouse_interruption(&mouse_packet, &current_byte)) //we analize it like in test_packet
            {              
              mouseRec = true;
              current_byte = 1; //if not, we keep reading the next packet
            }
          }
          else
          {
            mouseRec = false;
          }
          if (msg.m_notify.interrupts & BIT(timer_id)) //if the interrupt comes from the timer
          {
            timer_int_handler(); //we increment the timer_global_counter (keeps track of how much time has passed)
            timerRec = true;
          }
          else
          {
            timerRec = false;
          }

          if (msg.m_notify.interrupts & BIT(rtc_id)) 
          { /* subscribed interrupt */
            if(rtc_ih() == ALARM_RING)
            {
              rtcRec = true;
            }
            else rtcRec = false;
          }
          else
          {
            rtcRec = false;
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

    if(game_state == GAME_NOT_OVER)
    {
      checkInvertion(&player1, &player2);

      if(timer_global_counter % heart_alarm == 0)
      {
        if(!heart.active)
        {
          heart.active = true;
          
          schedule_heart_appearance(&time_4special);
          heart_alarm = time_4special * 60;
        }
      }

      if(rtcRec)
      {
        if(star.player != NULL && star.player->star)
        {
          deactivate_powerUP();

          schedule_star_appearance(&time_4special);
          set_alarm(time_4special);
        }
        else
        {
          activate_star();
        }
      }

      if(kbdRec)
      {
        kbd_read_and_display(&num_bytes, scan_byte, &pause_request, !print_scanbyte);

        handling_scanbyte(scan_byte, &player1, &player2);
      }

      if(mouseRec)
      {
        handling_mouse_packet(&mouse_packet, &player2);
      }

      if((timer_global_counter % 2) == 0 && timerRec)
      {
        draw();
      }

      verify_colisions();

      game_state = check_GameOver();

      if(game_state != GAME_NOT_OVER)
      {
        disable_alarm();
        enable_alarm();
        End_Game_Screen(game_state);
        set_alarm(4);
      }

      if(pause_request)
      {
        if(pause_game(&timer_id, &mouse_id) != SUCCESS)
        {
          return FAIL;
        }

        if(is_ExitState())
        {
          break;
        }
        else
        {
          pause_request = false;
          draw();
        }
      }
    }
    else
    {
      if(rtcRec)
      {
        break;
      }
    }
  }

  if(disable_necessary_interrupts(&mouse_id) != SUCCESS) 
  {
    return FAIL;
  }

  return SUCCESS;
}

int pause_game(uint8_t *timer_id, uint8_t *mouse_id)
{
  if(disable_necessary_interrupts(mouse_id) != SUCCESS) 
  {
    return FAIL;
  }

  window_state_handler();

  Menu();
    
  if(enable_necessary_interrupts(timer_id, mouse_id) != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

void draw()
{
  draw_sprite(&background);

  move_player(&player1);

  draw_sprite(&(player1.player));

  move_player(&player2);

  if(mouse_active)
  {
    stop_mouse_player_mov(&player2);
  }
  
  draw_sprite(&(player2.player));

  draw_projectiles();

  if(star.active)
  {
    draw_sprite(&(star.object));
  }

  if(heart.active)
  {
    draw_sprite(&(heart.object));
  }

  draw_life_bars(&player1, &player2);

  write_to_vram();
}

void End_Game_Screen(int game_state)
{
  End_Game();

  if(game_state == P1_WIN)
  {
    draw_sprite(&background);
    draw_sprite(&p1_win);
    write_to_vram();
  }
  else if(P2_WIN)
  {
    draw_sprite(&background);
    draw_sprite(&p2_win);
    write_to_vram();
  }
  else
  {
    draw_sprite(&background);
    draw_sprite(&tie);
    write_to_vram();
  }
}

int enable_necessary_interrupts(uint8_t *timer_id, uint8_t *mouse_id)
{
  if(enable_alarm() != SUCCESS)
  {
    return FAIL;
  }

  if(mouse_active)
  {
    if(mouse_beggining(*mouse_id) != SUCCESS)
    {
      return FAIL;
    }
  }

  if(timer_subscribe_int(timer_id) != SUCCESS) //subscribes timer interrupts
  {
    return FAIL;
  }

  return SUCCESS;
}

int disable_necessary_interrupts(uint8_t *mouse_id)
{
  if(timer_unsubscribe_int() != SUCCESS) //unsubscribe the timer's interrupt calls
  {
    return FAIL;
  }

  if(mouse_active)
  {
    if(mouse_ending(*mouse_id) != SUCCESS)
    {
      return FAIL;
    }
  }

  if(disable_alarm() != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}
