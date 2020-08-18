#include "colisions.h"


void verify_colisions()
{
  bool result1, result2;

  if(star.active)
  {
    result1 = isFrameColision(&(player1.player), &(star.object));
    result2 = isFrameColision(&(player2.player), &(star.object));

    if(result1 && result2)
    {
      deactivate_star();
    }
    else if(result1)
    {
      activate_powerUP(&player1);
      deactivate_star();
    }
    else if(result2)
    {
      activate_powerUP(&player2);
      deactivate_star();
    }
  }

  if(heart.active)
  {
    result1 = isFrameColision(&(player1.player), &(heart.object));
    result2 = isFrameColision(&(player2.player), &(heart.object));

    if(result1 && result2)
    {
      increase_health(&player1);
      increase_health(&player2);
      deactivate_heart();
    }
    else if(result1)
    {
      increase_health(&player1);
      deactivate_heart();
    }
    else if(result2)
    {
      increase_health(&player2);
      deactivate_heart();
    }
  }

  for(int i = 0; i < 20; i++)
  {
    if(get_projectile(i)->active == true)
    {
      if(get_projectile(i)->projectile.xpm == TACO)
      {
        if(isFrameColision(&(player1.player), &(get_projectile(i)->projectile)))
        {
          reducePlayer_lifePoints(&player1, ((int) BULLET_HIT_POINTS));
          get_projectile(i)->active = false;
        }
      }
      else if(get_projectile(i)->projectile.xpm == NYAN)
      {
        if(isFrameColision(&(player2.player), &(get_projectile(i)->projectile)))
        {
          reducePlayer_lifePoints(&player2, ((int) BULLET_HIT_POINTS));
          get_projectile(i)->active = false;
        }
      }
      else if(get_projectile(i)->projectile.xpm == BOMB && get_projectile(i)->exploded == true)
      {
        if(isFrameColision(&(player1.player), &(get_projectile(i)->projectile)))
        {
          reducePlayer_lifePoints(&player1, ((int) BOMB_HIT_POINTS));
        }

        if(isFrameColision(&(player2.player), &(get_projectile(i)->projectile)))
        {
          reducePlayer_lifePoints(&player2, ((int) BOMB_HIT_POINTS));
        }
      }
    }
  }
}

bool isFrameColision(struct object* o1, struct object* o2)
{
  bool x_colision_condition = !((o1->pos_x < o2->pos_x && (o1->pos_x + o1->img.width) < o2->pos_x) || (o1->pos_x > (o2->pos_x + o2->img.width) && (o1->pos_x + o1->img.width) > (o2->pos_x + o2->img.width)));

  if(x_colision_condition)
  {
    bool y_colision_condition = !((o1->pos_y < o2->pos_y && (o1->pos_y + o1->img.height) < o2->pos_y) || (o1->pos_y > (o2->pos_y + o2->img.height) && (o1->pos_y + o1->img.height) > (o2->pos_y + o2->img.height)));

    if(y_colision_condition)
    {
      return isObjectColision(o1, o2);
    }
  }

  return !COLISION;
}

bool isObjectColision(struct object* o1, struct object* o2)
{
  uint16_t x0, x1, y0, y1;

  if(o1->pos_x < o2->pos_x)
  {
    x0 = o2->pos_x;
  }
  else
  {
    x0 = o2->pos_x  + o2->img.width - o1->pos_x;
  } 

  if(o1->pos_x  + o1->img.width  < o2->pos_x + o2->img.width)
  {
    x1 = o1->pos_x  + o1->img.width;
  }
  else x1 = o2->pos_x  + o2->img.width;

  if(o1->pos_y < o2->pos_y)
  {    
    y0 = o2->pos_y;
  }
  else
  {
    y0 = o1->pos_y;
  } 

  if(o1->pos_y  + o1->img.height  < o2->pos_y + o2->img.height)
  {
    y1 = o1->pos_y  + o1->img.height;
  }
  else y1 = o2->pos_y  + o2->img.height;

  uint8_t *buffer = malloc((x1 - x0) * (y1 - y0) * 2);

  drawColisionBuffer(o1, x0, x1, y0, y1, buffer, false);

  bool result = drawColisionBuffer(o2, x0, x1, y0, y1, buffer, true);

  free(buffer);

  return result;
}

bool drawColisionBuffer(struct object *obj, uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1,uint8_t buffer[], bool secondObject)
{
  int k = abs(y0 - obj->pos_y) * (obj->img.width) * 2;
  size_t buffer_pos = 0;
  uint16_t currColor_buf, currColor_obj;

  for(int line = 0; line < y1 - y0; line++)
  {
    k = k + abs(x0 - obj->pos_x) * 2;

    for(int column = 0; column < x1 - x0; column++)
    {
      if(secondObject)
      {
        currColor_buf = buffer[buffer_pos + 1];
        currColor_buf = ((currColor_buf << 8) | buffer[buffer_pos]);

        currColor_obj = obj->pixmap[k + 1];
        currColor_obj = (currColor_obj << 8) | obj->pixmap[k];
        
        if(currColor_buf != CHROMA_KEY_GREEN_565 && currColor_obj != CHROMA_KEY_GREEN_565)
        {
          return COLISION;
        }
      }

      buffer[buffer_pos] = obj->pixmap[k];
      buffer_pos++;
      k++;

      buffer[buffer_pos] = obj->pixmap[k];
      buffer_pos++;
      k++;
    }

    k = k + abs(obj->pos_x + obj->img.width - x1) * 2;
  }

  return !COLISION;
}
