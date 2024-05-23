#ifndef Monster_H_INCLUDED
#define Monster_H_INCLUDED
#include "element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
/*
   [Monster object]
*/

typedef struct _Monster
{
    int x, y;
    int velocity;
    int heart;  
    int width,height;
    int damage;
    ALLEGRO_BITMAP *img;
        // the position of image
    Shape *hitbox; // the hitbox of object
} Monster;
Elements *New_Monster(int label,int x,int y,int velocity,int damage,int stage);
void Monster_update(Elements *const ele);
void Monster_interact(Elements *const, Elements *const);
void Monster_draw(Elements *const ele);
void Monster_destory(Elements *const ele);

#endif
