#include "projectile_handler.h"

void initialize_projectile_bases()
{
  star.object.pixmap = xpm_load(star_xpm, XPM_5_6_5, &(star.object.img));
  star.object.xpm = STAR;
  star.active = false;
  star.player = NULL;

  heart.object.pixmap = xpm_load(heart_xpm, XPM_5_6_5, &(heart.object.img));
  heart.object.xpm = HEART;
  heart.active = false;
  heart.player = NULL;
  
  cookieProjectile.projectile.xpm = NYAN;
  cookieProjectile.projectile.pixmap = xpm_load(cookie_xpm, XPM_5_6_5, &(cookieProjectile.projectile.img));
  cookieProjectile.projectile.pos_x = 0;
  cookieProjectile.projectile.pos_y = 0;
  cookieProjectile.movement_units = 15;
  cookieProjectile.h_mov = 0;
  cookieProjectile.v_mov = 0;
  cookieProjectile.active = false;

  tacoProjectile.projectile.xpm = TACO;
  tacoProjectile.projectile.pixmap = xpm_load(taco_xpm, XPM_5_6_5, &(tacoProjectile.projectile.img));
  tacoProjectile.projectile.pos_x = 0;
  tacoProjectile.projectile.pos_y = 0;
  tacoProjectile.movement_units = 15;
  tacoProjectile.h_mov = 0;
  tacoProjectile.v_mov = 0;
  tacoProjectile.active = false;

  bombProjectile.projectile.xpm = BOMB;
  bombProjectile.projectile.pixmap = xpm_load(bomb_xpm, XPM_5_6_5, &(bombProjectile.projectile.img));
  bombProjectile.projectile.pos_x = 0;
  bombProjectile.projectile.pos_y = 0;
  bombProjectile.movement_units = 30;   //used as a fuse, number of frames until explosion
  bombProjectile.h_mov = 0;
  bombProjectile.v_mov = 0;
  bombProjectile.active = false;

  bombExplosionAnimation.pixmap1 = xpm_load(explosion_small_xpm, XPM_5_6_5, &(bombExplosionAnimation.img1));
  bombExplosionAnimation.pixmap2 = xpm_load(explosion_medium_xpm, XPM_5_6_5, &(bombExplosionAnimation.img2));
  bombExplosionAnimation.pixmap3 = xpm_load(explosion_large_xpm, XPM_5_6_5, &(bombExplosionAnimation.img3));

  for(int i = 0; i < 20; i++)
  {
    projectile_array[i] = cookieProjectile;
  }
}

void shoot_projectile(int8_t h_dir, int8_t v_dir, uint16_t x_pos, uint16_t y_pos, xpm_id projType)
{
  int index = -1;
  for(int i = 0; i < 20; i++)
  {
    if(projectile_array[i].active == false)
    {
      index = i;
      break;
    }
  }
  if(index != -1)
  {
    if(projType == NYAN)
    {
      projectile_array[index] = cookieProjectile;
    }
    else if(projType == TACO)
    {
      projectile_array[index] = tacoProjectile;
    }
    
    projectile_array[index].projectile.pos_x = x_pos;
    projectile_array[index].projectile.pos_y = y_pos;
    projectile_array[index].h_mov = h_dir;
    projectile_array[index].v_mov = v_dir;
    projectile_array[index].active = true;
  }
}

void set_projectiles()
{
  for(int i = 0; i < 20; i++)
  {
    projectile_array[i].active = false;
  }
}

void draw_projectiles()
{
  for(int i = 0; i < 20; i++)
  {
    if(projectile_array[i].active == true)
    { 
      if(projectile_array[i].projectile.xpm == BOMB)
      { //bomb fuse and explosion processing
        if(projectile_array[i].movement_units > 0)
        {
          projectile_array[i].movement_units--;
        }
        else if(projectile_array[i].projectile.pixmap == bombProjectile.projectile.pixmap)
        {
          projectile_array[i].movement_units = 1;
          if(projectile_array[i].projectile.pos_x < 8)
            {projectile_array[i].projectile.pos_x = 0;}
          else if(projectile_array[i].projectile.pos_x + 54 > 1024)
            {projectile_array[i].projectile.pos_x = 1024 - 62;}
          else
            {projectile_array[i].projectile.pos_x -= 8;}
          projectile_array[i].projectile.pixmap = bombExplosionAnimation.pixmap1;
          projectile_array[i].projectile.img = bombExplosionAnimation.img1;
          projectile_array[i].exploded = true;
        }
        else if(projectile_array[i].projectile.pixmap == bombExplosionAnimation.pixmap1)
        {
          projectile_array[i].movement_units = 1;
          if(projectile_array[i].projectile.pos_x < 31)
            {projectile_array[i].projectile.pos_x = 0;}
          else if(projectile_array[i].projectile.pos_x + 94 > 1024)
            {projectile_array[i].projectile.pos_x = 1024 - 125;}
          else
            {projectile_array[i].projectile.pos_x -= 31;}
          if(projectile_array[i].projectile.pos_y < 31)
            {projectile_array[i].projectile.pos_y = 0;}
          else if(projectile_array[i].projectile.pos_y + 94 > 768)
            {projectile_array[i].projectile.pos_y = 768 - 125;}  
          else
            {projectile_array[i].projectile.pos_y -= 31;}
          projectile_array[i].projectile.pixmap = bombExplosionAnimation.pixmap2;
          projectile_array[i].projectile.img = bombExplosionAnimation.img2;
        }
        else if(projectile_array[i].projectile.pixmap == bombExplosionAnimation.pixmap2)
        {
          projectile_array[i].movement_units = 1;
          if(projectile_array[i].projectile.pos_x < 62)
            {projectile_array[i].projectile.pos_x = 0;}
          else if(projectile_array[i].projectile.pos_x + 188 > 1024)
            {projectile_array[i].projectile.pos_x = 1024 - 250;}
          else
            {projectile_array[i].projectile.pos_x -= 62;}
          if(projectile_array[i].projectile.pos_y < 62)
            {projectile_array[i].projectile.pos_y = 0;}
          else if(projectile_array[i].projectile.pos_y + 188 > 768)
            {projectile_array[i].projectile.pos_y = 768 - 250;}
          else
            {projectile_array[i].projectile.pos_y -= 62;}
          projectile_array[i].projectile.pixmap = bombExplosionAnimation.pixmap3;
          projectile_array[i].projectile.img = bombExplosionAnimation.img3;
        }
        else
        {
          projectile_array[i].active = false;
          projectile_array[i].exploded = false;
        }
      }
      else
      {
      //movement section of the projectiles
        if(projectile_array[i].v_mov == 0)
        {
          if(projectile_array[i].h_mov == 1)
          {
            if(projectile_array[i].projectile.pos_x + projectile_array[i].projectile.img.width + 15 >= h_res)
            {
              projectile_array[i].active = false;
            }
            else
            {
              projectile_array[i].projectile.pos_x += 15;
            }
          }
        else if(projectile_array[i].h_mov == -1)
          {
            if(projectile_array[i].projectile.pos_x < 15)
            {
              projectile_array[i].active = false;
            }
            else
            {
              projectile_array[i].projectile.pos_x -= 15;
            }
          }
        }
        else if(projectile_array[i].v_mov == 1)
        {
          if(projectile_array[i].h_mov == 1)
          {
            if(projectile_array[i].projectile.pos_x + projectile_array[i].projectile.img.width + 12 >= h_res)
            {
              projectile_array[i].active = false;
            }
            else if(projectile_array[i].projectile.pos_y + projectile_array[i].projectile.img.height + 9 >= v_res)
            {
              projectile_array[i].v_mov = -1;
              projectile_array[i].projectile.pos_x += 12;
              projectile_array[i].projectile.pos_y -= 9;
            }
            else
            {
              projectile_array[i].projectile.pos_x += 12;
              projectile_array[i].projectile.pos_y += 9;
            }
          }
          else if(projectile_array[i].h_mov == -1)
          {
            if(projectile_array[i].projectile.pos_x < 12)
            {
              projectile_array[i].active = false;
            }
            else if(projectile_array[i].projectile.pos_y + projectile_array[i].projectile.img.height + 9 >= v_res)
            {
              projectile_array[i].v_mov = -1;
              projectile_array[i].projectile.pos_x -= 12;
              projectile_array[i].projectile.pos_y -= 9;
            }
            else
            {
              projectile_array[i].projectile.pos_x -= 12;
              projectile_array[i].projectile.pos_y += 9;
            }
          }
        }
        else if(projectile_array[i].v_mov == -1)
        {
          if(projectile_array[i].h_mov == 1)
          {
            if(projectile_array[i].projectile.pos_x + projectile_array[i].projectile.img.width + 12 >= h_res)
            {
              projectile_array[i].active = false;
            }
            else if(projectile_array[i].projectile.pos_y < 9)
            {
              projectile_array[i].v_mov = 1;
              projectile_array[i].projectile.pos_x += 12;
              projectile_array[i].projectile.pos_y += 9;
            }
            else
            {
              projectile_array[i].projectile.pos_x += 12;
              projectile_array[i].projectile.pos_y -= 9;
            }
          }
          else if(projectile_array[i].h_mov == -1)
          {
            if(projectile_array[i].projectile.pos_x < 12)
            {
              projectile_array[i].active = false;
            }
            else if(projectile_array[i].projectile.pos_y < 9)
            {
              projectile_array[i].v_mov = 1;
              projectile_array[i].projectile.pos_x -= 12;
              projectile_array[i].projectile.pos_y += 9;
            } 
            else
            {
              projectile_array[i].projectile.pos_x -= 12;
              projectile_array[i].projectile.pos_y -= 9;
            }
          }
        }
      }
      //drawing section
      if(projectile_array[i].active == true) //the value can be changed in the movement section
      {
        draw_sprite(&(projectile_array[i].projectile));
      }
    }
  }
}

void drop_bomb(uint16_t x_pos, uint16_t y_pos)
{
  int index = -1;
  for(int i = 0; i < 20; i++)
  {
    if(projectile_array[i].active == false)
    {
      index = i;
      break;
    }
  }
  if(index != -1)
  {

    projectile_array[index] = bombProjectile;
    projectile_array[index].projectile.xpm = BOMB;
    projectile_array[index].projectile.pos_x = x_pos;
    projectile_array[index].projectile.pos_y = y_pos;
    projectile_array[index].active = true;
  }
}

struct projectile * get_projectile(int index)
{
  return &(projectile_array[index]);
}

void schedule_star_appearance(uint8_t *time)
{
  srand(get_seconds()); //a seed is generated

  *time = (rand() % 4) + 8; //the time between the moment a star is caught and the next one is generated is between 2 and 10 seconds

  star.object.pos_x = rand() % 973;
  star.object.pos_y = rand() % 719;
}

void activate_star()
{
  star.active = true;
}

void deactivate_star()
{
  star.active = false;
}

void activate_powerUP(struct player *player)
{
  star.player = player;
  star.player->star = true;
  set_alarm(POWER_UP);
}

void deactivate_powerUP()
{
  star.player->star = false;
  star.player = NULL;
}

void schedule_heart_appearance(uint8_t *time)
{
  srand(get_seconds()); //a seed is generated

  *time = (rand() % 5) + 5; //the time between the moment a star is caught and the next one is generated is between 2 and 10 seconds

  heart.object.pos_x = rand() % 973;
  heart.object.pos_y = rand() % 719;
}

void activate_heart()
{
  heart.active = true;
}

void deactivate_heart()
{
  heart.active = false;
}
