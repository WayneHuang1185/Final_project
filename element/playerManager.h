#ifndef player_Manager_H_INCLUDED
#define player_Manager_H_INCLUDED
#include "element.h"
#include "../scene/gamescene.h"
#include "player.h"
#include "monster.h"
/*
   [player_Manager object]
*/
typedef struct _PlayerManager
{ double pl_x, pl_y;
  int store_exp;
  bool update_stage;
  int stage;
  int total_monster;
} PlayerManager;
Elements *New_player_Manager(int label);
void playerManager_update(Elements *const self);
void playerManager_interact(Elements *const self, Elements *const target);
void playerManager_draw(Elements *const self);
void playerManager_destory(Elements *const self);

#endif
