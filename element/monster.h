#ifndef Monster_H_INCLUDED
#define Monster_H_INCLUDED
#include "element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
#include <allegro5/allegro_image.h>
/*
   [Monster object]
*/

typedef struct _Monster
{
    double x, y;
    int id;
    double move_speed;
    int hp;  
    int hp_max;
    int atk_timer;
    bool hurt;
    int hurt_timer;
    int recovery;
    int hp_timer;
    int hp_recovery;
    int atk_frequency;
    int width,height;
    int damage;
    int exp;
    ALLEGRO_BITMAP *img[2];
        // the position of image
    Shape *hitbox; // the hitbox of object
} Monster;
Elements *New_Monster(int label,int id,double x, double y);
void Monster_update(Elements *const ele);
void Monster_interact(Elements *const, Elements *const);
void Monster_draw(Elements *const ele);
void Monster_destory(Elements *const ele);

#endif
