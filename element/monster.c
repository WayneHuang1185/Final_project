#include"Monster.h"
#include"../global.h"
#include"../shapes/Circle.h"
#include "../shapes/Shape.h"
#include "../shapes/Rectangle.h"
#include <allegro5/allegro_primitives.h>
#include<math.h>
Elements *New_Monster(int label,int x,int y,int velocity,int damage,int stage){
    Monster *pDerivedObj = (Monster *)malloc(sizeof(Monster));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->x = x;
    pDerivedObj->y = y;
    pDerivedObj->velocity=velocity;
    pDerivedObj->damage=damage;
    char buf[50];
    sprintf(buf,"assets/image/Monster%d.png",stage);
    pDerivedObj->img = al_load_bitmap(buf);
    pDerivedObj->height=al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->width=al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x,
                                        pDerivedObj->y,
                                        pDerivedObj->x + pDerivedObj->width,
                                        pDerivedObj->y + pDerivedObj->height);
    pObj->inter_obj[pObj->inter_len++] = Bullet_L;
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Monster_update;
    pObj->Interact = Monster_interact;
    pObj->Draw = Monster_draw;
    pObj->Destroy = Monster_destory;
    return pObj;
    
}
void _Monster_update_postion(Elements *const self,int dx, int dy){
     
}
void Monster_update(Elements *const self)
{
    Monster *Obj = ((Monster*)(self->pDerivedObj));

    Shape *hitbox = Obj->hitbox;
}
void Monster_interact(Elements *const self, Elements *const ele){

    Monster *Obj = ((Monster *)(self->pDerivedObj));
    
   // printf("-2\n");
    
}
void Monster_draw(Elements *const ele){
    Monster *Obj = ((Monster *)(ele->pDerivedObj));
}
void Monster_destory(Elements *const ele){
    Monster *Obj = ((Monster *)(ele->pDerivedObj));
    free(Obj->hitbox);
    free(Obj);
    free(ele);
}