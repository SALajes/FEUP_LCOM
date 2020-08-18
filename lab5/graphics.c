#include <lcom/lcf.h>

#include "graphics.h"



static void* video_mem; /* Process (virtual) address to which VRAM is mapped */ 
static unsigned h_res; /* Horizontal resolution in pixels */
static unsigned v_res; /* Vertical resolution in pixels */ 
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static uint8_t memory_model;
static vbe_mode_info_t info_box;

int vbe_get_mode (uint16_t mode, vbe_mode_info_t* vmi_p)
{
  phys_bytes buf = 0;
  struct reg86u r;

  memset(&r, 0, sizeof(r));
  
  /*use liblm.a to initialize buf*/
  if(lm_init(0) == NULL)
  {
    return FAIL;
  }

  mmap_t address_map;

  if(lm_alloc(sizeof (vbe_mode_info_t), &address_map) == NULL)
  {
    return FAIL;
  }

  buf = address_map.phys;

  r.u.w.ax = 0x4F01;          /*VBE get mode info*/

  /*translate the buffer linear address to a far pointer*/

  r.u.w.es = PB2BASE(buf);    /*set a segment base*/
  r.u.w.di = PB2OFF(buf);     /*set the offset accordingly*/
  r.u.w.cx = mode;
  r.u.b.intno = 0x10;

  if( sys_int86(&r) != SUCCESS ) 
  { 
    return FAIL;
  }

  *vmi_p = *(vbe_mode_info_t*) address_map.virt;

  lm_free(&address_map);

  return SUCCESS;
}

void* (vg_init)(uint16_t mode)
{
  struct reg86u reg86;

  memset(&reg86, 0, sizeof(reg86));

  reg86.u.b.intno = 0x10;
  reg86.u.b.ah = 0x4F;
  reg86.u.b.al = 0x02;

  reg86.u.w.bx = mode | BIT(14);


  if( sys_int86(&reg86) != SUCCESS)
  {
    printf("\tvg_start(): sys_int86() failed \n");
    return NULL;
  }

  

  if(vbe_get_mode(mode, &info_box) != SUCCESS)
  {
    return NULL;
  }

  

  h_res = info_box.XResolution;
  v_res = info_box.YResolution;
  bits_per_pixel = info_box.BitsPerPixel;
  memory_model = info_box.MemoryModel;

  int r;
  struct minix_mem_range mr;
  /*unsigned int*/ phys_bytes vram_base = info_box.PhysBasePtr; /* VRAM's physical addresss */
  unsigned int vram_size = h_res * v_res * bits_per_pixel / 8; /* VRAM's size, but you can use the frame-buffer size, instead */
  /* Allow memory mapping */ 
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if( (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != SUCCESS ) 
  {
    return NULL;
  }

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED)
  {
    return NULL;
  }

  
  return video_mem; 
}

int (vg_draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
  char* local_video_mem = video_mem;
  uint8_t bytes_ppixel;

  if(bits_per_pixel % 8 > 0)
  {
    bytes_ppixel = (bits_per_pixel / 8) + 1;
  }
  else 
  {
    bytes_ppixel = (bits_per_pixel / 8);
  }

  local_video_mem = local_video_mem + bytes_ppixel*y*h_res + bytes_ppixel*x;

  for(int i = 0; i < len; i++)
  {
    for(int j = 0; j < bytes_ppixel; j++)
    {
    *local_video_mem = (color << j);
    local_video_mem = local_video_mem + 1;
    }
  }

  return SUCCESS;
}

int (vg_draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
  if(height + y - 1 == v_res || width + x - 1 == h_res)
  {
    return FAIL;
  }

  for(int i = 0; i < height ; i++)
  {
    if(vg_draw_hline(x, y + i, width, color) != SUCCESS)
    {
      return FAIL;
    }
  }

  return SUCCESS;
} 	

void determine_RectSize(uint8_t no_rectangles, uint16_t *rect_width, uint16_t *rect_height)
{
  *rect_width = h_res / no_rectangles;
  *rect_height = v_res / no_rectangles;
}

uint32_t determine_Color(uint8_t no_rectangles, uint8_t row, uint8_t col, uint32_t first, uint8_t step, uint8_t nG, uint8_t nB, uint8_t RedScreenMask, uint8_t GreenScreenMask, uint8_t BlueScreenMask, uint8_t R_first, uint8_t G_first, uint8_t B_first)
{
  switch(memory_model)
  {
    case INDEX_COLOR:
    {
      char c = (first + (row * no_rectangles + col) * step) % (1 << bits_per_pixel);
      return (uint32_t) c;
      break;
    }
    case DIRECT_COLOR:
    {
      uint32_t color;

      uint32_t R = (uint32_t) (R_first + col * step) % (1 << RedScreenMask);
      R = (R << (nG + nB)); 

      uint32_t G = (uint32_t) (G_first + row * step) % (1 << GreenScreenMask);
      G = (G << nB);

      uint32_t B = (uint32_t) (B_first + (col + row) * step) % (1 << BlueScreenMask);

      color = (R | (G | B));

      return color;
      break;
    }
  }

  return FAIL;
}

int draw_pattern (uint8_t no_rectangles, uint32_t first, uint8_t step) 
{
  //unsigned int rect_counter = 1;
  uint16_t rect_width, rect_height;
  uint16_t x = 0, y = 0;
  uint8_t row = 1;
  uint8_t col = 1;
  uint32_t color;
  uint8_t nR = info_box.RedMaskSize;
  uint8_t nG = info_box.GreenMaskSize;
  uint8_t nB = info_box.BlueMaskSize;
  uint8_t RedScreenMask =  (1 << nR) - 1;
  uint8_t GreenScreenMask =  (1 << nG) - 1;
  uint8_t BlueScreenMask =  (1 << nB) - 1;
  uint8_t R_first = (uint8_t) ((first >> (nG + nB)) & RedScreenMask);
  uint8_t G_first = (uint8_t) ((first >> nB) & GreenScreenMask);
  uint8_t B_first = (uint8_t) (first & BlueScreenMask);

  determine_RectSize(no_rectangles, &rect_width, &rect_height);

  //while(rect_counter <= no_rectangles /** no_rectangles*/)
  for(unsigned int i = 1; i<= (no_rectangles * no_rectangles); i++)
  {
    color = determine_Color(no_rectangles, row, col, first, step, nG, nB, RedScreenMask, GreenScreenMask, BlueScreenMask, R_first, G_first, B_first);

    if(vg_draw_rectangle(x, y, rect_width, rect_height, color) != SUCCESS)
    {
      return FAIL;
    }

    x = x + rect_width;
    col++;

    if(i % no_rectangles == 0)
    {
      y = y + rect_height;
      x = 0;
      row++;
      col = 1;    
    }
  }

  return SUCCESS;
}

int returnTextMode()
{
  struct reg86u r;

  memset(&r, 0, sizeof(r));

  r.u.b.intno = 0x10;
  r.u.b.ah = 0x00;
  r.u.b.al = 0x03;

  if( sys_int86(&r) != SUCCESS)
  {
    printf("\tvg_start(): sys_int86() failed \n");
    return FAIL;
  }

  return SUCCESS;
}

int draw_sprite(const char *xpm[], uint16_t x, uint16_t y)
{
  int height, width, k = 0;
  char * pixmap;
  pixmap = read_xpm(xpm, &width, &height);
  if(pixmap == NULL)
  {
    return FAIL;
  }
  if(((unsigned int)(x + width)) >= h_res || ((unsigned int)(y + height)) >= v_res)
  {
    return FAIL;
  }

  for(int i = 0; i < height ; i++)
  {
    char* local_video_mem = video_mem;

    local_video_mem = local_video_mem + (y+i)*h_res + x;

    for(int j = 0; j < width; j++)
    {
    
      *local_video_mem = pixmap[k];
      local_video_mem = local_video_mem + 1;
      k++;
    }
  }
  return SUCCESS;

}
