#include "main_pause_menu.h"

static enum window state = MAIN_MENU;

bool mouse_active;

struct cursor cursor;
struct object background;
struct object button_play;
struct object button_exit;
struct object numbers[12];
struct object pause_menu;
struct object p2_mode;
struct object logo;
struct object mouse;
struct object keyboard;
struct object p1_win;
struct object p2_win;
struct object tie;

void window_state_handler()
{
  switch(state)
  {
    case MAIN_MENU:
    {
      if(cursor.position == LEFT)
      {
        state = CHOOSE_P2_MODE;

        print_p2_mode_frame();
      }
      else state = EXIT;

      break;
    }
    case CHOOSE_P2_MODE:
    {
      if(cursor.position == LEFT)
      {
        mouse_active = true;
      }
      else mouse_active = false;

      state = IN_GAME;

      break;
    }
    case IN_GAME:
    {
      state = PAUSE_MENU;

      break;
    }
    case PAUSE_MENU:
    {
      if(cursor.position == LEFT)
      {
        state = IN_GAME;
      }
      else state = EXIT;

      break;
    }
    case EXIT:
    {
      break;
    }
  }
}

void End_Game()
{
  state = EXIT;
}

bool is_ExitState()
{
  if(state == EXIT)
  {
    return true;
  }
  else return false;
}

void reinitialize_state()
{
  state = MAIN_MENU;
}

void initialize_background()
{
  background.xpm = BACKGROUND;
  background.pixmap = xpm_load(background1_xpm, XPM_5_6_5, &(background.img));
  background.pos_x = 0;
  background.pos_y = 0;
}

void initialize_menu_sprites()
{
  button_play.pixmap = xpm_load(PLAY_button_xpm, XPM_5_6_5, &(button_play.img));
  button_play.pos_x = 156;
  button_play.pos_y = 498;
  
  button_exit.pixmap = xpm_load(EXIT_button_xpm, XPM_5_6_5, &(button_exit.img));
  button_exit.pos_x = 668;
  button_exit.pos_y = 498;

  cursor.cursor.pixmap = xpm_load(button_SELECTOR_xpm, XPM_5_6_5, &(cursor.cursor.img));
  cursor.cursor.pos_x = 206;
  cursor.cursor.pos_y = 637;
  cursor.position = LEFT;

  logo.pixmap = xpm_load(logo_xpm, XPM_5_6_5, &(logo.img));
  logo.pos_x = 391;
  logo.pos_y = 60;

  pause_menu.pixmap = xpm_load(pause_xpm, XPM_5_6_5, &(pause_menu.img));
  pause_menu.pos_x = 212;
  pause_menu.pos_y = 50;

  p2_mode.pixmap = xpm_load(p2_mode_xpm, XPM_5_6_5, &(p2_mode.img));
  p2_mode.pos_x = 213;
  p2_mode.pos_y = 200;

  mouse.pixmap = xpm_load(mouse_pic_xpm, XPM_5_6_5, &(mouse.img));
  mouse.pos_x = 127;
  mouse.pos_y = 380;

  keyboard.pixmap = xpm_load(keyboard_pic_xpm, XPM_5_6_5, &(keyboard.img));
  keyboard.pos_x = 618;
  keyboard.pos_y = 394;

  //numbers[12];
  numbers[0].pixmap = xpm_load(zero_xpm, XPM_5_6_5, &(numbers[0].img));
  numbers[1].pixmap = xpm_load(one_xpm, XPM_5_6_5, &(numbers[1].img));
  numbers[2].pixmap = xpm_load(two_xpm, XPM_5_6_5, &(numbers[2].img));
  numbers[3].pixmap = xpm_load(three_xpm, XPM_5_6_5, &(numbers[3].img));
  numbers[4].pixmap = xpm_load(four_xpm, XPM_5_6_5, &(numbers[4].img));
  numbers[5].pixmap = xpm_load(five_xpm, XPM_5_6_5, &(numbers[5].img));
  numbers[6].pixmap = xpm_load(six_xpm, XPM_5_6_5, &(numbers[6].img));
  numbers[7].pixmap = xpm_load(seven_xpm, XPM_5_6_5, &(numbers[7].img));
  numbers[8].pixmap = xpm_load(eight_xpm, XPM_5_6_5, &(numbers[8].img));
  numbers[9].pixmap = xpm_load(nine_xpm, XPM_5_6_5, &(numbers[9].img));
  numbers[10].pixmap = xpm_load(two_dots_xpm, XPM_5_6_5, &(numbers[10].img));
  numbers[11].pixmap = xpm_load(slash_xpm, XPM_5_6_5, &(numbers[11].img));

  p1_win.pixmap = xpm_load(PLAYER_1_WINS_xpm, XPM_5_6_5, &(p1_win.img));
  p1_win.pos_x = 131;
  p1_win.pos_y = 349;

  p2_win.pixmap = xpm_load(PLAYER_2_WINS_xpm, XPM_5_6_5, &(p2_win.img));
  p2_win.pos_x = 131;
  p2_win.pos_y = 349;

  tie.pixmap = xpm_load(ITS_A_TIE_xpm, XPM_5_6_5, &(tie.img));
  tie.pos_x = 131;
  tie.pos_y = 349;
}

int enable_keyboard_graphics_rtc()
{
  uint8_t kbd_id = KBD_IRQ; //used to identify the interrupt susbcription
  uint8_t rtc_id = RTC_IRQ; 
  uint8_t policy = IRQ_REENABLE|IRQ_EXCLUSIVE;

  if(vg_init(GRAPHICS_MODE) == NULL)
  {
    if(vg_exit() != SUCCESS)
    {
        return FAIL;
    }
    return FAIL;
  }

  if(rtc_subscribe_interrupt(rtc_id) != SUCCESS) 
  {
    return FAIL;
  }

  if(kbd_subscribe_interrupt(policy, &kbd_id) != SUCCESS) //subscribe interrupts and disable interrupts for the keyboard driver
  {
    return FAIL;
  }

  return SUCCESS;
}

int disable_keyboard_graphics_rtc()
{
  if(kbd_unsubscribe_interrupt() != SUCCESS)//unsub interrupts and reenable them for the driver
  {
    return FAIL;
  }

  if(rtc_unsubscribe_interrupt() != SUCCESS)
  {
    return FAIL;
  }

  if(vg_exit() != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

void print_numbers(uint8_t i, uint16_t x, uint16_t y)
{
  numbers[((int) i)].pos_x = x;
  numbers[((int) i)].pos_y = y;

  draw_sprite(&numbers[((int) i)]);
}

void print_date(uint8_t day, uint8_t month, uint8_t year)
{
  uint8_t i;
  uint16_t x = 185;
  uint16_t dx = 60 + 6;
  uint16_t y = 300;
  
  i = ((day & MSBYTE) >> 4);
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);

  x += dx;
  i = (day & LSBYTE);
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);

  x += dx;
  i = 0x11;
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);

  x += dx;
  i = ((month & MSBYTE) >> 4);
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);

  x += dx;
  i = (month & LSBYTE);
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);

  x += dx;
  i = 0x11;
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);

  x += dx;
  i = 0x02;
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);

  x += dx;
  i = 0x00;
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);

  x += dx;
  i = ((year & MSBYTE) >> 4);
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);

  x += dx;
  i = (year & LSBYTE);
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);
}

void print_time(uint8_t hour, uint8_t minute)
{
  uint8_t i;
  uint16_t x = 350;
  uint16_t dx = 60 + 6;
  uint16_t y = 400;
  
  i = ((hour & MSBYTE) >> 4);
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);

  x += dx;
  i = (hour & LSBYTE);
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);

  x += dx;
  i = 0x10;
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);

  x += dx;
  i = ((minute & MSBYTE) >> 4);
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);

  x += dx;
  i = (minute & LSBYTE);
  convert_BCD_decimal(&i);
  print_numbers(i, x, y);
}

void print_frame(uint8_t day, uint8_t month, uint8_t year, uint8_t minute, uint8_t hour)
{
  draw_sprite(&background);

  if(state == PAUSE_MENU)
  {
    draw_sprite(&pause_menu);
  }
  else draw_sprite(&logo);

  convert_decimal_BCD(&day);
  convert_decimal_BCD(&month);
  convert_decimal_BCD(&year);
  print_date(day, month, year);

  convert_decimal_BCD(&hour);
  convert_decimal_BCD(&minute);
  print_time(hour, minute);

  draw_sprite(&button_play);
  draw_sprite(&button_exit);
  draw_sprite(&(cursor.cursor));

  write_to_vram();
}

void print_p2_mode_frame()
{
  draw_sprite(&background);
  draw_sprite(&p2_mode);
  draw_sprite(&mouse);
  draw_sprite(&keyboard);
  draw_sprite(&(cursor.cursor));

  write_to_vram();
}

bool cursor_move(uint8_t cursor_movement)
{
  if(cursor_movement == CURSOR_STOP)
  {
    return false;
  }
  else if(cursor_movement == CURSOR_MOVE_LEFT && cursor.position == LEFT)
  {
    return false;
  }
  else if(cursor_movement == CURSOR_MOVE_LEFT && cursor.position == RIGHT)
  {
    cursor.cursor.pos_x = 206;
    cursor.position = LEFT;
    return true;
  }
  else if(cursor_movement == CURSOR_MOVE_RIGHT && cursor.position == RIGHT)
  {
    return false;
  }
  else if(cursor_movement == CURSOR_MOVE_RIGHT && cursor.position == LEFT)
  {
    cursor.cursor.pos_x = 718;
    cursor.position = RIGHT;
    return true;
  }
  else if(cursor_movement == CONFIRM)
  {
    window_state_handler();

    cursor.cursor.pos_x = 206;
    cursor.position = LEFT;
    return true;
  }

  return false;
}

int Menu()
{
  uint8_t rtc_id = RTC_IRQ;
  uint8_t keyboard_id = KEYBOARD_IRQ;

  uint8_t day, month, year;
  uint8_t hour, minute, minute2, second;

  if(get_date(&day, &month, &year) != SUCCESS)
  {
    return FAIL;
  }

  if(get_time(&second, &minute, &hour) != SUCCESS)
  {
    return FAIL;
  }

  print_frame(day, month, year, minute, hour);

  minute2 = minute;

  if(enable_update() != SUCCESS)
  {
    return FAIL;
  }
  
  if(disable_alarm() != SUCCESS)
  {
    return FAIL;
  }

  if(disable_periodic() != SUCCESS)
  {
    return FAIL;
  }

  int ipc_status, r;
  message msg;
  uint8_t result;
  uint8_t num_bytes = 1; // can be 1 or 2 according to the number of bytes expected to receive as make and break codes
  uint8_t scan_byte[2]; //array that will be used to store the bytes we want to print
  const bool print_scanbyte = true;
  bool kbdRec = false, rtcRec = false;  //indicates if there is a new interruption from the kbd so as to handle the new scancode (equivalent for rtc)
  bool print_rtc, print_kbd;
  uint8_t cursor_movement;
  bool end_loop;

  print_frame(day, month, year, minute, hour);

  while(true)
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
          if (msg.m_notify.interrupts & BIT(rtc_id)) //an interrupt by the rtc if notified
          { 
            result = rtc_ih();
            rtcRec = true;
          }
          else rtcRec = false;

          if (msg.m_notify.interrupts & BIT(keyboard_id)) //if the interrupt comes from the keyboard
          {
            kbc_ih(); //calls the read status (and code) function written in C
            kbdRec = true;
          }
          else kbdRec = false;

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

    if(rtcRec && result == UPDATE && state != PAUSE_MENU)
    {
      if(get_time(&second, &minute, &hour) != SUCCESS)
      {
        return FAIL;
      }

      if(minute != minute2)
      {
        print_rtc = true;

        if(get_date(&day, &month, &year) != SUCCESS)
        {
          return FAIL;
        }
      }
      else print_rtc = false;
    }
    else print_rtc = false;

    if(kbdRec)
    {
      kbd_read_and_display(&num_bytes, scan_byte, &end_loop, !print_scanbyte);

      menu_and_pause_kbc_handler(scan_byte, &cursor_movement);

      if(cursor_move(cursor_movement))
      {
        print_kbd = true;
      }
      else print_kbd = false;
    }
    else print_kbd = false;

    if(state == MAIN_MENU || state == PAUSE_MENU)
    {
      if(print_rtc || print_kbd)
      {
        print_frame(day, month, year, minute, hour);
      }
    }
    else if(state == CHOOSE_P2_MODE && print_kbd)
    {
      print_p2_mode_frame();
    }
    else if(state == EXIT || state == IN_GAME)
    {
      break;
    }
  }

  if(disable_update() != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}
