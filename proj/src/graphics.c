#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "i8254.h"
#include "i8042.h"
#include "graphics.h"
#include "kbc_handler.h"


void* video_mem; /* Process (virtual) address to which VRAM is mapped */ 
unsigned h_res; /* Horizontal resolution in pixels */
unsigned v_res; /* Vertical resolution in pixels */ 
unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
uint8_t memory_model;
vbe_mode_info_t info_box;


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

int video_init() 
{
  //switch to video
  if(vg_init(GRAPHICS_MODE) == NULL)
  {
    if(vg_exit() != SUCCESS)
    {
        return FAIL;
    }
    return FAIL;
  }

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

void printResolutions()
{
  printf("horizontal res: %d \n", h_res);
  printf("vertical res: %d \n", v_res);
  printf("bits per pixel: %d \n", bits_per_pixel);
}

