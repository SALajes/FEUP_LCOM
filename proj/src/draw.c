
#include "draw.h"

extern void* video_mem; /* Process (virtual) address to which VRAM is mapped */ 
extern unsigned h_res; /* Horizontal resolution in pixels */
extern unsigned v_res; /* Vertical resolution in pixels */ 
extern unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
extern uint8_t memory_model;
extern vbe_mode_info_t info_box;

uint8_t buffer[1024 * 768 * 2];

void draw_sprite(struct object *obj)
{
  uint8_t stage = 1;
  bool print = true;
  uint16_t currColor;
	int  k = 0;

	for(int i = 0; i < obj->img.height; i++)
	{
		unsigned int buffer_pos = ((obj->pos_y+i)*h_res * 2) + (obj->pos_x * 2);

		for(int j = 0; j < obj->img.width * 2; j++)
		{
			if(stage == 1)
			{
				currColor = obj->pixmap[k + 1];
				currColor = (currColor << 8) | obj->pixmap[k];
				
				print = currColor != CHROMA_KEY_GREEN_565;
			
				stage = 2;
			}
			else
			{
				stage = 1;
			}

			if(print == true)
			{
				buffer[buffer_pos] = obj->pixmap[k];
			}
			buffer_pos++;
			k++;

		}
	}
}

void draw_life_bars(struct player * player1, struct player *player2)
{
	int length = player1->life_points / 2;
	int width = 20;
	int buffer_pos;

	uint16_t bar_color;

	if(player1->star)
	{
		bar_color = GOLD_BAR;
	}
	else bar_color = NYAN_BAR;

	for(int i = 0; i < width; i++)
  {
		buffer_pos = ((39 + i)*h_res + 39)*2;

    for(int j = 0; j < length; j++)
    {
    	buffer[buffer_pos] = (bar_color & 0x00FF);
			buffer_pos++;
    	buffer[buffer_pos] = ((bar_color & 0xFF00) >> 8);
			buffer_pos++;
    }
  }

	length = player2->life_points / 2;

	if(player2->star)
	{
		bar_color = GOLD_BAR;
	}
	else bar_color = TACO_BAR;

	for(int i = 0; i < width; i++)
  {
		buffer_pos = ((39 + i)*h_res + 984)*2 + 1;

    for(int j = 0; j < length; j++)
    {
    	buffer[buffer_pos] = ((bar_color & 0xFF00) >> 8);
			buffer_pos--;
    	buffer[buffer_pos] = (bar_color & 0x00FF);
			buffer_pos--;
    }
  }
}

void write_to_vram()
{
  memcpy(video_mem, buffer, h_res * v_res * (bits_per_pixel / 8));
}
