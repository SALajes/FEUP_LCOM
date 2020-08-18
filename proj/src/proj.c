// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you

#include "dispatcher.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

bool spectator = false;

int (proj_main_loop)(int UNUSED(argc), char *UNUSED(argv[])) 
{
  enable_keyboard_graphics_rtc();

  initialize_background();

  initialize_menu_sprites();

  initializePlayer1();
  initializePlayer2();

  initialize_projectile_bases();

  while(true)
  {
    reinitialize_state();

    if(Menu() != SUCCESS)
    {
      return FAIL;
    }

    if(is_ExitState())
    {
      break;
    }

    set_players();
    set_projectiles();

    if(play() != SUCCESS)
    {
      return FAIL;
    }
  }

 /* if(!spectator)
  {
    if(play(&player1, &player2) != SUCCESS)
    {
      //vg_exit();
      return FAIL;
    }
  }
  else
  {
    if(spectate(&player1, &player2) != SUCCESS)
    {
      //vg_exit();
      return FAIL;
    }
  }*/

  disable_keyboard_graphics_rtc();

  return 0;
}
