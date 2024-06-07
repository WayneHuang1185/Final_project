#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED
#include "scene.h"
#include "../element/element.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
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
    double Title_x, Title_y;
    ALLEGRO_FONT *font;
    ALLEGRO_BITMAP *background;
    

} GameScene;
Scene *New_GameScene(int label);
void game_scene_update(Scene *const pGameSceneObj);
void game_scene_draw(Scene *const pGameSceneObj);
void game_scene_destroy(Scene *const pGameSceneObj);

#endif
