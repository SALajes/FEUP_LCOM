#include "player_handler.h"

extern unsigned h_res; /* Horizontal resolution in pixels */
extern unsigned v_res; /* Vertical resolution in pixels */ 

uint8_t *nyan_invert_pixmap;
uint8_t *taco_invert_pixmap;
uint8_t *nyan_pixmap;
uint8_t *taco_pixmap;
struct player player1;
struct player player2;

void initializePlayer1()
{
  nyan_invert_pixmap = xpm_load(nyan_cat_invert_xpm, XPM_5_6_5, &(player1.player.img));
  nyan_pixmap = xpm_load(nyan_cat_xpm, XPM_5_6_5, &(player1.player.img));
  
  player1.player.xpm = NYAN;
  player1.player.pixmap = nyan_pixmap;
}

void initializePlayer2()
{
  taco_invert_pixmap = xpm_load(taco_cat_invert_xpm, XPM_5_6_5, &(player2.player.img));
  taco_pixmap = xpm_load(taco_cat_xpm, XPM_5_6_5, &(player2.player.img));

  player2.player.xpm = TACO;
  player2.player.pixmap = taco_pixmap;
}

void set_players()
{
  player1.player.pos_x = 20;
  player1.player.pos_y = 384;
  player1.movement_units = 8;
  player1.h_mov = 0;
  player1.v_mov = 0;
  player1.inverted = true;
  player1.life_points = MAX_HEALTH;
  player1.star = false;

  player2.player.pos_x = 750;
  player2.player.pos_y = 384;
  player2.movement_units = 8;
  player2.h_mov = 0;
  player2.v_mov = 0;
  player2.inverted = false;
  player2.life_points = MAX_HEALTH;
  player2.star = false;
}

void checkInvertion(struct player *player1, struct player *player2)
{
  if(player1->player.pos_x > player2->player.pos_x)
  {
    player1->player.pixmap = nyan_invert_pixmap;
    player2->player.pixmap = taco_invert_pixmap;

    player1->inverted = true;
    player2->inverted = true;
  }
  else
  {
    player1->player.pixmap = nyan_pixmap;
    player2->player.pixmap = taco_pixmap;

    player1->inverted = false;
    player2->inverted = false;
  }
}

void move_player(struct player *player)
{
  if(player->h_mov == -1)
    {
      if(player->movement_units > player->player.pos_x)
      {
        player->player.pos_x = 0;
      }
      else
      {
        player->player.pos_x -= player->movement_units;
      }
    } 
    else if(player->h_mov == 1)
    {
      player->player.pos_x += player->movement_units;
    }
    if(player->v_mov == -1)
    {
      if(player->movement_units > player->player.pos_y)
      {
        player->player.pos_y = 0;
      } 
      else
      {
        player->player.pos_y -= player->movement_units;
      }
    }
    else if(player->v_mov == 1)
    {
      player->player.pos_y += player->movement_units;
    }
  
    if(((unsigned int)(player->player.pos_x + player->player.img.width)) > h_res)
    {
      player->player.pos_x = h_res - player->player.img.width;
    }
    if(((unsigned int)(player->player.pos_y + player->player.img.height)) > v_res)
    {
      player->player.pos_y = v_res - player->player.img.height;
    }
}

void stop_mouse_player_mov(struct player *player)
{
  player->h_mov = 0;
  player->v_mov = 0;
}

void reducePlayer_lifePoints(struct player *player, int hit_points)
{
  if(player->star == false)
  {
    player->life_points -= hit_points;
  }
}

int check_GameOver()
{
  if(player1.life_points <= 0 && player2.life_points <= 0)
  {
    return TIE;
  }
  else if(player1.life_points <= 0)
  {
    return P2_WIN;
  }
  else if(player2.life_points <= 0)
  {
    return P1_WIN;
  }
  else return GAME_NOT_OVER;
}

void increase_health(struct player *player)
{
  if(player->life_points != MAX_HEALTH)
  {
    player->life_points += 100;

    if(player->life_points > MAX_HEALTH)
    {
      player->life_points = MAX_HEALTH;
    }
  }
}
