#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED
#include "scene.h"
#include "../element/element.h"
#include "../element/charater.h"
#include "../element/floor.h"
#include "../element/teleport.h"
#include "../element/tree.h"
#include "../element/projectile.h"
/*
   [game scene object]
*/
typedef enum EleType
{
    Floor_L,
    Teleport_L,
    Tree_L,
    Character_L,
    Projectile_L,
    Ball_L,
    Bullet_L,
    Monster_L,
    Drops_L,
    Player_L,
    PlayerManager_L
}EleType;
typedef struct _GameScene
{   
    ALLEGRO_BITMAP *background;

} GameScene;
Scene *New_GameScene(int label);
void game_scene_update(Scene *const pGameSceneObj);
void game_scene_draw(Scene *const pGameSceneObj);
void game_scene_destroy(Scene *const pGameSceneObj);

#endif
