// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "i8254.h"
#include "i8042.h"
#include "graphics.h"
#include "keyboard.h"
#include "kbd_graphics.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (video_test_init)(uint16_t mode, uint8_t delay) 
{
  //switch to video
  if(vg_init(mode) == NULL)
  {
    if(vg_exit() != SUCCESS)
  {
    printf("init fucked up\n");
    return FAIL;
  }
    return FAIL;
  }

  sleep(delay);

  if(vg_exit() != SUCCESS)
  {
    return FAIL;
  }

  if(returnTextMode() != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color) 
{
  if(vg_init(mode) == NULL)
  {
    return FAIL;
  }

  if(vg_draw_rectangle(x, y, width, height, color) != SUCCESS)
  {
    return FAIL;
  }

  if(end_function_byESC () != SUCCESS)
  {
    return FAIL;
  }

  if(vg_exit() != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) 
{
  if(vg_init(mode) == NULL)
  {
    return FAIL;
  }

  if(draw_pattern(no_rectangles, first, step) != SUCCESS)
  {
    return FAIL;
  }

  if(end_function_byESC () != SUCCESS)
  {
    return FAIL;
  }

  if(vg_exit() != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

int (video_test_xpm)(const char *xpm[], uint16_t x, uint16_t y) {
  
  if(vg_init(0x105) == NULL)
  {
    return FAIL;
  }


  if(draw_sprite(xpm, x, y) != SUCCESS)
  {
    return FAIL;
  }

  if(end_function_byESC () != SUCCESS)
  {
    return FAIL;
  }

  if(vg_exit() != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

int (video_test_move)(const char *xpm[], uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n", __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int (video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}  
