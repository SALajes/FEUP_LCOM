
#include <lcom/lcf.h>
#include "i8042.h"
#include "graphics_macros.h"

int returnTextMode();
void determine_RectSize(uint8_t no_rectangles, uint16_t *rect_width, uint16_t *rect_height);
uint32_t determine_Color(uint8_t no_rectangles, uint8_t row, uint8_t col, uint32_t first, uint8_t step, uint8_t nG, uint8_t nB, uint8_t RedScreenMask, uint8_t GreenScreenMask, uint8_t BlueScreenMask, uint8_t R_first, uint8_t G_first, uint8_t B_first);
int draw_pattern (uint8_t no_rectangles, uint32_t first, uint8_t step);
int draw_sprite(const char *xpm[], uint16_t x, uint16_t y);
